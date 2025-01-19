#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include <format>
#include <chrono>

int tableColumns = 7; //Костыль, Количество колонок в основной таблице

/*
Нет времени всё расписывать напише здесь:

У данной структуры есть следующие методы:

start_session() - Принимает путь к файлу, начинает сессию
end_sessioin() - Заканчивает сессию

request_login() - Возвращаяет информацию по login (Слегка кривой)
request_names() - Возвращает все имена
get_role_by_id() - Принимет id, возвращает роль

Мне лень писать дальше комментарии, большенство методов делают то что на них написано, я спать

*/

enum Status
{
    Inactive = 0,
    Active = 1,
    Clarify = 2,
    Deleted = 3

};

enum Role
{
    Administrator = 1,
    Manager = 2,
    User = 3
};

size_t hashing(std::string password) {
    std::hash<std::string> hash_fn;
    size_t hashed_password = hash_fn(password);
    //std::cout << "Хэш пароля: " << hashed_password << std::endl; // это просто для проверки, можно удалить, оно работает
    return hashed_password;
}

struct db {

    bool open = false;
    sqlite3* base;

    void start_session(const char* file) {

        int out;
        out = sqlite3_open(file, &base);

        if (out) {
            std::cerr << "Error: " << sqlite3_errmsg(base) << std::endl;
        }
        else {
            std::cout << "Data base was opened succesufully" << std::endl;
            open = true;
        }
    }

    std::vector<std::string> request_login(std::string name) {
        int out;
        std::string req = std::format("select * from users where login == '{}'", (name));
        sqlite3_stmt* statement;
        std::vector<std::string> vec = {};
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        if (out == SQLITE_OK) {
            while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
                for (int i = 0; i < tableColumns; i++) {
                    vec.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, i))));
                }
            }
            sqlite3_finalize(statement);
        }
        else {
            std::cerr << "Request error:" << sqlite3_errmsg(base) << std::endl;
        }
        return vec;
    }

    std::vector<std::vector<std::string>> request_names() {
        int out;
        std::string req = "select username, login from users";
        sqlite3_stmt* statement;
        std::vector<std::vector<std::string>> vec = {};
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        if (out == SQLITE_OK) {
            std::cout << "Request complete" << std::endl;
            while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
                std::vector<std::string> push = {};
                  push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0))));
                  push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1))));
                  vec.push_back(push);
            }
            
            sqlite3_finalize(statement);
        }
        else {
            std::cerr << "Request error: " << sqlite3_errmsg(base) << std::endl;
        }
        return vec;
    }


    // TESTING
    std::vector<std::vector<std::string>> request_user_details() {
        int out;
        std::string req = "select user_id, username, login, user_mail, live_status, role_id from users";
        sqlite3_stmt* statement;
        std::vector<std::vector<std::string>> vec = {};
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        if (out == SQLITE_OK) {
            /*std::cout << "Request complete" << std::endl;*/
            while ((out = sqlite3_step(statement)) == SQLITE_ROW) {
                std::vector<std::string> push = {};
                push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)))); // id
                push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)))); // username
                push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 2)))); // login
                push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 3)))); // mail
                push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 4)))); // live_status

                // Предполагаем, что role_id может быть как текстом, так и числом.
                // Если это точно число, используйте sqlite3_column_int
                const unsigned char* role_id_text = sqlite3_column_text(statement, 5);
                push.push_back(std::to_string(sqlite3_column_int(statement, 5)));
                /*if (role_id_text) {
                    push.push_back(std::string(reinterpret_cast<const char*>(role_id_text)));
                }
                else {
                    push.push_back(std::to_string(sqlite3_column_int(statement, 5)));
                }*/

                vec.push_back(push);
            }

            sqlite3_finalize(statement);
        }
        else {
            std::cerr << "Request error: " << sqlite3_errmsg(base) << std::endl;
        }
        return vec;
    }
    // TESTING


   

    std::vector<std::vector<std::string>> request_users() {
        int out;
        std::string req = "select username, login from users where role_id = 3";
        sqlite3_stmt* statement;
        std::vector<std::vector<std::string>> vec = {};
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        if (out == SQLITE_OK) {
            std::cout << "Request complete" << std::endl;
            while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
                std::vector<std::string> push = {};
                push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0))));
                push.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1))));
                vec.push_back(push);
            }

            sqlite3_finalize(statement);
        }
        else {
            std::cerr << "Request error: " << sqlite3_errmsg(base) << std::endl;
        }
        return vec;
    }

    std::string get_role_by_id(int id) {
        int out;
        std::string req = std::format("select roles.role_name from roles join users on users.role_id = roles.role_id where users.user_id = {}", id);
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
            return std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        }
        sqlite3_finalize(statement);
    }

    // 0 - inactive, 1 - active, 2 - clarify, 3 - deleted
    void set_status(int user_id, Status status_id) { 

        std::string req;
        switch (status_id)
        {
        case Inactive:
            req = std::format("UPDATE Users set live_status = 'Inactive' WHERE user_id = {}", user_id);
            break;
        case Active:
            req = std::format("UPDATE Users set live_status = 'Active' WHERE user_id = {}", user_id);
            break;
        case Clarify:
            req = std::format("UPDATE Users set live_status = 'Clarify' WHERE user_id = {}", user_id);
            break;
        case Deleted:
            req = std::format("UPDATE Users set live_status = 'Deleted' WHERE user_id = {}", user_id);
            break;

        default:
            std::cout << "Wrong Status" << std::endl;
            return;
        }

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        sqlite3_step(statement);

        sqlite3_finalize(statement);
    }

    void cin_set_status(int user_id) {
        int status;
        std::cin >> status;
        set_status(user_id, Status(status));
    }

    void set_username(int user_id, std::string name) {
        std::string req = std::format("update users set username = '{}' where user_id = {}", name, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void cin_set_username(int user_id) {
        std::string name;
        std::cin >> name;
        set_username(user_id, name);
    }

    void set_login(int user_id, std::string login) {
        std::string req = std::format("update users set login = '{}' where user_id = {}", login, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void cin_set_login(int user_id) {
        std::string log;
        std::cin >> log;
        set_login(user_id, log);
    }

    void set_mail(int user_id, std::string mail) {
        std::string req = std::format("update users set user_mail = '{}' where user_id = {}", mail, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void cin_set_mail(int user_id) {
        std::string mail;
        std::cin >> mail;
        set_mail(user_id, mail);
    }

    void set_password(int user_id, std::string password) {
        std::string req = std::format("update users set user_hashed_password = '{}' where user_id = {}", std::to_string(hashing(password)), user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void cin_set_password(int user_id) {
        std::string pass;
        std::cin >> pass;
        set_password(user_id, pass);
    }

    void set_role(int user_id, Role role) {

        std::string trueRole;

        switch (role)
        {
        case Administrator: 
            trueRole = "Administrator";
            break;
        case Manager:
            trueRole = "Manager";
            break;
        case User:
            trueRole = "User";
            break;
        default:
            std::cout << "Role error" << std::endl;
            return;
        }

        std::string req = std::format("update users set username = '{}' where user_id = {}", trueRole, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void cin_set_role(int user_id) {
        int role;
        std::cin >> role;
        set_role(user_id, Role(role));
    }


    std::string get_status_by_id(int id) {
        int out;
        std::string req = std::format("select live_status from users where user_id = {}", id);
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
            return std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
        }
        sqlite3_finalize(statement);
    }

    int get_id_by_login(std::string login) {
        int out;
        std::string req = std::format("select user_id from users where login = '{}'", login);
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
            return sqlite3_column_int(statement, 0);
        }
        sqlite3_finalize(statement);
    }

    std::string get_password_by_login(std::string login) {
        int out;
        std::string req = std::format("select user_hashed_password from users where login = '{}'", login);
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        
        sqlite3_step(statement);
        std::string pass = (std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0))));
        
        sqlite3_finalize(statement);

        return pass;
    }

    void add_user(std::string username, std::string login, std::string mail, std::string hashPassword, Status liveStatus,  Role role) {

        std::string trueStatus;

        switch (liveStatus)
        {
        case Inactive:
            trueStatus = "Inactive";
            break;
        case Active:
            trueStatus = "Active";
            break;
        case Clarify:
            trueStatus = "Clarify";
            break;
        case Deleted:
            trueStatus = "Deleted";
            break;
        default:
            std::cout << "Status error" << std::endl;
            return;
        }


        int out;
        std::string req = std::format("insert into users (user_id, username, login, user_mail, user_hashed_password, live_status, role_id) values ((select MAX(user_id) from Users) + 1, '{}', '{}', '{}', '{}', '{}', {})", username, login, mail, hashPassword, trueStatus, int(role));
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void set_expiry(int id) {

        auto nowe = std::chrono::system_clock::now();
        auto nextWeek = nowe + std::chrono::hours(24 * 14);
        std::time_t nextWeek_time_t = std::chrono::system_clock::to_time_t(nextWeek);

        std::tm tim;
        localtime_s(&tim, &nextWeek_time_t);

        std::string cur;
        cur.append(std::to_string(tim.tm_year+1900));
        if (tim.tm_mon+1 < 10) cur.append("0");
        cur.append(std::to_string(tim.tm_mon+1));
        if (tim.tm_mday < 10) cur.append("0");
        cur.append(std::to_string(tim.tm_mday));


        int out;
        std::string req = std::format("insert into expiry values({}, {})", id, cur);
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void delete_id(int id) {

        int out;
        std::string req = std::format("delete from users where user_id = {}", id);
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        sqlite3_step(statement);
        sqlite3_finalize(statement);
        delete statement;

        req = std::format("delete from expiry where user_id = {}", id);
        sqlite3_stmt* statement1;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement1, 0);

        sqlite3_step(statement1);
        sqlite3_finalize(statement1);
        delete statement1;

    }

    void update_expiry() {

        int out;
        std::string req = "select * from expiry";
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        std::vector<std::vector<int>> ids;

        while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
            std::vector<int> push = {};
            push.push_back(std::stoi(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)))));
            push.push_back(std::stoi(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, 1)))));

            ids.push_back(push);
        }


        if (ids.size() > 0) {

            time_t now = time(0);
            std::tm tim;
            localtime_s(&tim, &now);

            int numericDate = tim.tm_mday + ((tim.tm_mon + 1) * 100) + ((tim.tm_year + 1900) * 10000);

            for (std::vector<int> vec : ids) {
                if (vec[1] <= numericDate) {
                    delete_id(vec[0]);
                }
            }
        }
    }

    void end_session() {
        sqlite3_close(base);
        open = false;
    }
};