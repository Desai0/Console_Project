#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include <format>

int tableColumns = 7; //�������, ���������� ������� � �������� �������

/*
��� ������� �� ����������� ������ �����:

� ������ ��������� ���� ��������� ������:

start_session() - ��������� ���� � �����, �������� ������
end_sessioin() - ����������� ������

request_login() - ����������� ���������� �� login (������ ������)
request_names() - ���������� ��� �����
get_role_by_id() - �������� id, ���������� ����

��� ���� ������ ������ �����������, ����������� ������� ������ �� ��� �� ��� ��������, � �����

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

struct db {

    sqlite3* base;

    void start_session(const char* file) {

        int out;
        out = sqlite3_open(file, &base);

        if (out) {
            std::cerr << "Error: " << sqlite3_errmsg(base) << std::endl;
        }
        else {
            std::cout << "Data base was opened succesufully" << std::endl;
        }
    }

    std::vector<std::string> request_login(const char* name) {
        int out;
        std::string req = std::format("select * from users where login == '{}'", (name));
        sqlite3_stmt* statement;
        std::vector<std::string> vec = {};
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        if (out == SQLITE_OK) {
            std::cout << "Request complete" << std::endl;
            while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
                for (int i = 0; i < tableColumns-3; i++) {
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

    void set_username(int user_id, std::string name) {
        std::string req = std::format("update users set username = '{}' where user_id = {}", name, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void set_login(int user_id, std::string login) {
        std::string req = std::format("update users set login = '{}' where user_id = {}", login, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void set_mail(int user_id, std::string mail) {
        std::string req = std::format("update users set user_mail = '{}' where user_id = {}", mail, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
    }

    void set_password(int user_id, std::string password) {
        std::string req = std::format("update users set user_hashed_password = '{}' where user_id = {}", password, user_id);

        int out;
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);
        sqlite3_step(statement);
        sqlite3_finalize(statement);
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

    int get_id_by_login(const char* login) {
        int out;
        std::string req = std::format("select user_id from users where login = 'altometer'", login);
        sqlite3_stmt* statement;
        out = sqlite3_prepare_v2(base, req.c_str(), -1, &statement, 0);

        while ((out = sqlite3_step(statement) == SQLITE_ROW)) {
            return sqlite3_column_int(statement, 0);
        }
        sqlite3_finalize(statement);
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

    void end_session() {
        sqlite3_close(base);
    }
};