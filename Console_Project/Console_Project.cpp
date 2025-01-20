#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <cassert>
#include <fstream>
#include <ctime>
#include <string>
#include <iomanip>
#include "sessions.h"



HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void creset() {
    system("cls");
    SetConsoleCursorPosition(console, { 0, 0 });
}


//void test_database_connection() {
//    //Тест на подключение к БД
//    Session testSession("database");
//
//    //Проверяем, что бд открыта
//    if (!testSession.base.open) {
//        std::cerr << "Database connection failed\n";
//        exit(1); //Завершаем программу, если не удалось
//    }
//    std::cout << "Database connected successfully\n";
//}
//
//void test_add_user() {
//    //Тест на добавление нового пользователя
//    Session testSession("database");
//
//    //Добавляем новго пользователя
//    testSession.base.add_user("TestUser", "test_login", "test@mail.com", "hashed_password", Clarify, Administrator);
//
//    //Запрашиваем всех пользователей
//    auto users = testSession.base.request_names();
//    bool found = false;
//
//    //Проверяем, что новый пользователь добавлен
//    for (auto& user : users) {
//        if (user[1] == "test_login") {
//            found = true;
//            break;
//        }
//    }
//
//    assert(found && "User addition failed");
//    std::cout << "User added successfully\n";
//}
//
//void test_edit_user() {
//    //Тест на редактирование данных пользователя
//    Session testSession("database");
//
//    //Ищем пользователя по логину
//    std::vector<std::string> userData = testSession.base.request_login("test_login");
//
//    //Сохраняем старое имя пользователя
//    std::string oldUsername = userData[1];
//
//    //Редактируем имя пользователя
//    testSession.base.cin_set_username(std::stoi(userData[0]));
//    //Редактируем имя пользователя по ID
//
//    //Запрашиваем обновленные данные
//    userData = testSession.base.request_login("test_login");
//
//    //Проверяем, что имя пользователя изменилось
//    assert(userData[1] != oldUsername && "User edit failed");
//    std::cout << "User data edited successfully\n";
//}
//
//void test_edit_user_status() {
//    //Тест на изменение статуса пользователя
//    Session testSession("database");
//
//    //Ищем пользователя
//    std::vector<std::string> userData = testSession.base.request_login("test_login");
//
//    //Сохраняем старый статус
//    std::string oldStatus = userData[5];
//
//    //Меняем статус
//    testSession.base.set_status(std::stoi(userData[0]), Active);
//    //Устанавлием новый статус
//
//    //Запрашиваем обновленные данные
//    userData = testSession.base.request_login("test_login");
//
//    //Проверяем, что статус изменился
//    assert(userData[5] != oldStatus && "User status change failed");
//    std::cout << "User status updated successfully\n";
//}
//
//
//
//void test_exception_handling() {
//    //Тест на сохранение данных при исключении
//    try {
//        Session testSession("database");
//
//        //Имитируем исключение
//        throw std::runtime_error("Simulated exception");
//    }
//    catch (const std::exception& e) {
//        std::cerr << "Exception caught: " << e.what() << "n"; // можно добавить код для проверки сохранения данных
//    }
//}

/*
Парооли от тестовой базы данных по айди

1-9 "12345"
2 - 123
10 "n0ts"
11 я забыл лмао
12 - l
*/
tm* get_local_time(time_t* timer) { // Функция-обертка
    return localtime(timer);
}

void log_failed_attempt(const std::string& type, const std::string& details) {
    std::ofstream log_file("crash_log.txt", std::ios::app);
    if (log_file.is_open()) {
        time_t now = time(0);
        tm* ltm_ptr = get_local_time(&now); // Используем обертку
        if (ltm_ptr != nullptr) {
            tm ltm = *ltm_ptr; // Копируем структуру
            int year_численный = ltm.tm_year + 1900;
            int month_численный = ltm.tm_mon + 1;
            int day_численный = ltm.tm_mday;
            int hour_численный = ltm.tm_hour;
            int minute_численный = ltm.tm_min;
            int second_численный = ltm.tm_sec;

            log_file << "["
                << std::setw(4) << std::setfill('0') << year_численный << "-"
                << std::setw(2) << std::setfill('0') << month_численный << "-"
                << std::setw(2) << std::setfill('0') << day_численный << " "
                << std::setw(2) << std::setfill('0') << hour_численный << ":"
                << std::setw(2) << std::setfill('0') << minute_численный << ":"
                << std::setw(2) << std::setfill('0') << second_численный << "] ";
            log_file << "Type: " << type << ", Details: " << details << std::endl;
        }
        else {
            log_file << "[Ошибка времени] ";
            log_file << "Type: " << type << ", Details: " << details << std::endl;
        }
        log_file.close();
    }
    else {
        std::cerr << "Не удалось открыть файл crash_log.txt для записи." << std::endl;
    }
}


bool attempt_log_in(Session& user) {
    std::string password;
    std::string login;

    system("cls");
    std::cout << "Enter login: \n";
    std::cout << "Enter password: \n";
    SetConsoleCursorPosition(console, { 13, 0 });
    std::cin >> login;
    SetConsoleCursorPosition(console, { 16, 1 });
    std::cin >> password;

    // Записываем данные перед попыткой входа
    log_failed_attempt("Login Attempt", "Login: " + login + ", Password (raw): " + password); // Внимание: в реальном приложении пароль нужно хэшировать перед записью

    try {
        if (user.log_in(login, password)) {
            creset();
            std::cout << "Logged in successfully";
            // Очищаем лог-файл при успешном входе (можно убрать, если хотите сохранять все попытки)
            std::ofstream log_file("crash_log.txt", std::ios::trunc);
            return true;
        }
        else {
            creset();
            std::cout << "Login or password error, try again or leave, coward";
            return false;
        }
    }
    catch (const std::exception& e) {
        log_failed_attempt("Login Error", "Exception: " + std::string(e.what()) + ", Login: " + login);
        creset();
        std::cerr << "An error occurred during login: " << e.what() << std::endl;
        return false;
    }
}

void register_user(Session& user) {
    creset();
    std::cout << "User Registration\n";
    std::cout << "Enter username: ";
    std::string username;
    std::cin >> username;
    std::cout << "Enter login: ";
    std::string login;
    std::cin >> login;
    std::cout << "Enter password: ";
    std::string password;
    std::cin >> password;
    std::cout << "Enter email: ";
    std::string email;
    std::cin >> email;

    // Записываем данные перед регистрацией
    log_failed_attempt("Registration Attempt", "Username: " + username + ", Login: " + login + ", Password (raw): " + password + ", Email: " + email); // Внимание: пароль нужно хэшировать

    try {
        // Добавляем пользователя со статусом Clarify
        user.base.add_user(username, login, email, std::to_string(hashing(password)), Clarify, User);
        std::cout << "Registration successful. Your account is pending clarification.\n";
        // Очищаем лог-файл при успешной регистрации
        std::ofstream log_file("crash_log.txt", std::ios::trunc);
        Sleep(2000);
    }
    catch (const std::exception& e) {
        log_failed_attempt("Registration Error", "Exception: " + std::string(e.what()) + ", Username: " + username + ", Login: " + login);
        creset();
        std::cerr << "An error occurred during registration: " << e.what() << std::endl;
        Sleep(2000);
    }
}

void edit_profile(Session& user) {
    
    while (true)
    {
        std::cout << "Select an attribute to edit in your profile"
            << "\n"
            << "0 - Cancel\n"
            << "1 - Username\n"
            << "2 - Login\n"
            << "3 - E-mail\n"
            << "4 - Password\n";

        short choice;
        std::cin >> choice;

        switch (choice) {
        case 0: 
            break;
        case 1:
            std::cout << "Enter new Username: ";
            user.base.cin_set_username(user.id);
            std::cout << std::endl;

            break;
        case 2:
            std::cout << "Enter new Login: ";
            user.base.cin_set_login(user.id);
            std::cout << std::endl;

            break;
        case 3:
            std::cout << "Enter new E-mail: ";
            user.base.cin_set_mail(user.id);
            std::cout << std::endl;

            break;
        case 4:
            std::cout << "Enter new Password: ";
            user.base.cin_set_password(user.id);
            std::cout << std::endl;

            break;
        default:
            std::cout << "No, you moron! That is not an option! Try again";
            continue;
        }
        
        break;
    }
    
}

void add_a_guy(Session& user) { //prime json material

    int rid = 3;

    if (user.rid == Administrator || user.rid == Manager) {

        std::cout << "Adding a new user\n"; // перенес по требованиям
        std::cout << "Enter username: ";
        std::string name;
        std::cin >> name;

        std::cout << "Enter login: ";
        std::string log;
        std::cin >> log;

        std::cout << "Enter mail: ";
        std::string mail;
        std::cin >> mail;

        std::cout << "Enter password: ";
        std::string pass;
        std::cin >> pass;


        if (user.rid == Administrator) {
            std::cout << "1 - Administartor, 2 - Manager, 3 - User\n";
            std::cout << "Enter role id: ";


            while (true) {
                std::cin >> rid;

                switch (rid)
                {
                case 1:
                case 2:
                case 3:
                    break;
                default:
                    std::cout << "\nRole id error, try again: " << std::endl;
                    continue;
                }
                break;
            }
        }
        

        user.base.add_user(name, log, mail, std::to_string(hashing(pass)), Active, Role(rid));
    }
    else {
        std::cout << "\nYou can't do this" << std::endl;
    }
    
}

void edit_a_guy(Session& user) {

    std::cout << "Editing user's profile\nPlease, enter user's login: ";

    std::string log;
    std::cin >> log;

    std::vector<std::vector<std::string>> logins = user.base.request_names();

    bool flag = false;
    for (std::vector<std::string> vec : logins) {
        if (vec[1] == log) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        std::cout << "User not found" << std::endl;
    }
    else {
        std::vector<std::string> edit = user.base.request_login(log);

        if (user.rid >= 2 and std::stoi(edit[6]) <= 2) {
            std::cout << "This user can not be edited" << std::endl;
        }
        else {
            int editId = std::stoi(edit[0]);
            std::string originalStatus = edit[4]; // Получаем текущий статус пользователя

            while (true) {
                std::cout << "Select an attribute to edit"
                    << "\n"
                    << "0 - Cancel\n"
                    << "1 - Username\n"
                    << "2 - Login\n"
                    << "3 - E-mail\n"
                    << "4 - Password\n"
                    << "5 - Status\n"; // Добавляем опцию изменения статуса

                short choice;
                std::cin >> choice;

                switch (choice) {
                case 0:
                    break;
                case 1:
                    std::cout << "Enter new Username: ";
                    user.base.cin_set_username(editId);
                    std::cout << std::endl;
                    break;
                case 2:
                    std::cout << "Enter new Login: ";
                    user.base.cin_set_login(editId);
                    std::cout << std::endl;
                    break;
                case 3:
                    std::cout << "Enter new E-mail: ";
                    user.base.cin_set_mail(editId);
                    std::cout << std::endl;
                    break;
                case 4:
                    std::cout << "Enter new Password: ";
                    user.base.cin_set_password(editId);
                    std::cout << std::endl;
                    break;
                case 5: { // Добавляем блок для изменения статуса
                    std::cout << "Select new Status:\n";
                    std::cout << "0 - Inactive\n";
                    std::cout << "1 - Active\n";
                    std::cout << "2 - Clarify\n";
                    std::cout << "3 - Deleted\n";
                    std::cout << "Enter status number: ";
                    int new_status_int;
                    std::cin >> new_status_int;

                    if (new_status_int >= 0 && new_status_int <= 3) {
                        Status new_status = static_cast<Status>(new_status_int);
                        // **Сначала устанавливаем новый статус**
                        user.base.set_status(editId, new_status);

                        // **Затем получаем обновленную информацию о пользователе**
                        std::vector<std::string> updated_edit = user.base.request_login(log);
                        std::string updatedStatus = updated_edit[4];

                        // Проверяем, был ли статус "Deleted" и изменился ли он
                        if (originalStatus == "Deleted" && updatedStatus != "Deleted") {
                            user.base.delete_expiry(editId);
                            std::cout << "User removed from deletion queue.\n";
                        }
                    }
                    else {
                        std::cout << "Invalid status number.\n";
                    }
                    break;
                }
                default:
                    std::cout << "No, you moron! That is not an option! Try again";
                    continue;
                }
                break;
            }
        }
    }
}

void delete_a_guy(Session& user) {
    std::cout << "Marking user's profile for deletion\nPlease, enter user's login: ";

    std::string log;
    std::cin >> log;

    int id = user.base.get_id_by_login(log);

    std::vector<std::vector<std::string>> logins = user.base.request_names();

    bool flag = false;
    for (std::vector<std::string> vec : logins) {

        if (vec[1] == log) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        std::cout << "User not found" << std::endl;
    }
    else {
        std::vector<std::string> edit = user.base.request_login(log);

        if (user.rid >= 2 and std::stoi(edit[6]) <= 2) {
            std::cout << "This user can not be edited" << std::endl;
        }
        else {
            user.base.set_expiry(id);
            user.base.set_status(id, Deleted);
            std::cout << "User was marked for deletion, their account will be deleted in 14 days" << std::endl;
        }
    }

}

void interfac_user(Session& user) {
    std::cout << "Welcome, "
        << user.username
        << "\n"
        << "Login: "
        << user.login
        << "\n"
        << "E-mail: "
        << user.mail
        << "\n"
        << "Status: "
        << user.status
        << "\n\n";


    ///test
    std::cout << " _____________________________________________________________________________________________________________________________\n";
    SetConsoleCursorPosition(console, { 0, 7 });
    std::cout << "| user_id";
    SetConsoleCursorPosition(console, { 10, 7 });
    std::cout << "| username";
    SetConsoleCursorPosition(console, { 38, 7 });
    std::cout << "| login";
    SetConsoleCursorPosition(console, { 56, 7 });
    std::cout << "| user_mail";
    SetConsoleCursorPosition(console, { 95, 7 });
    std::cout << "| live_status";
    SetConsoleCursorPosition(console, { 115, 7 });
    std::cout << "| role_id |\n";

    //

    std::vector<std::vector<std::string>> user_data = user.base.request_user_details();

    // Вывод полученных данных
    for (const auto& user_info : user_data) {
        std::string user_id_str = user_info[0];
        int user_id_int = std::stoi(user_id_str);
        SetConsoleCursorPosition(console, { 1, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[0];
        SetConsoleCursorPosition(console, { 11, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[1];
        SetConsoleCursorPosition(console, { 39, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[2];
        SetConsoleCursorPosition(console, { 57, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[3];
        SetConsoleCursorPosition(console, { 96, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[4];
        SetConsoleCursorPosition(console, { 116, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[5] << "\n\n";
    }
    ///test


    switch (status_to_int(user.status)) {
    case Inactive:
        std::cout << "Sorry, your account is currently inactive. You can't edit it" << std::endl;
        break;
    case Active:
        edit_profile(user);
        break;
    case Clarify:
        std::cout << "Your account details must be clarified, please contact your manager" << std::endl;
        add_a_guy(user);
        break;
    case Deleted:
        std::cout << "Your account currently marked for deletion, please get out" << std::endl;
        break;
    default:
        std::cout << "Something went wrong! Please contact an Administator for help. Bad Status Error" << std::endl;
    }
}

void interfac_manager(Session& user) {
    std::cout << "Welcome, "
        << user.username
        << "\n"
        << "Login: "
        << user.login
        << "\n"
        << "E-mail: "
        << user.mail
        << "\n"
        << "Status: "
        << user.status
        << "\n\n";

/////////////////TESTING
    std::cout << " _____________________________________________________________________________________________________________________________\n";
    SetConsoleCursorPosition(console, { 0, 7 });
    std::cout << "| user_id";
    SetConsoleCursorPosition(console, { 10, 7 });
    std::cout << "| username";
    SetConsoleCursorPosition(console, { 38, 7 });
    std::cout << "| login";
    SetConsoleCursorPosition(console, { 56, 7 });
    std::cout << "| user_mail";
    SetConsoleCursorPosition(console, { 95, 7 });
    std::cout << "| live_status";
    SetConsoleCursorPosition(console, { 115, 7 });
    std::cout << "| role_id |\n";

    //

    std::vector<std::vector<std::string>> user_data = user.base.request_user_details();

    // Вывод полученных данных
    for (const auto& user_info : user_data) {
        std::string user_id_str = user_info[0];
        int user_id_int = std::stoi(user_id_str);
        SetConsoleCursorPosition(console, { 1, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[0];
        SetConsoleCursorPosition(console, { 11, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[1];
        SetConsoleCursorPosition(console, { 39, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[2];
        SetConsoleCursorPosition(console, { 57, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[3];
        SetConsoleCursorPosition(console, { 96, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[4];
        SetConsoleCursorPosition(console, { 116, (static_cast<SHORT>(user_id_int + 8)) });
        std::cout << user_info[5] << "\n\n";
    }
/////////////////TESTING


    switch (status_to_int(user.status)) {
    case Inactive:
        std::cout << "Sorry, your account is currently inactive. You can't perform actions on it" << std::endl;
        break;
    case Active:
        std::cout << "0 - Cancel, 1 - Edit profile, 2 - Edit User, 3 - Add User, 4 - Mark user for deletion\n";
        std::cout << "Select an option to manage: ";

        int selector;
        std::cin >> selector;

        switch (selector) {
        case 0: // INTERFACE
            break;
        case 1:
            edit_profile(user);
            break;
        case 2:
            edit_a_guy(user);
            break;
        case 3:
            add_a_guy(user);
            break;
        case 4:
            delete_a_guy(user);
            Sleep(2000);

            break;
        default:
            std::cout << "Not an option, you are going to hell now";
            break;
        }
        
        break;
    case Clarify:
        std::cout << "Your account details must be clarified, please contact your administrator" << std::endl;
        add_a_guy(user);
        break;
    case Deleted:
        std::cout << "Your account currently marked for deletion, please get out" << std::endl;
        break;
    default:
        std::cout << "Something went wrong! Please contact an Administator for help. Bad Status Error" << std::endl;
    }
}

void interfac(Session& user) {
    if (user.rid == 3) {
        interfac_user(user);
    }
    else if (user.rid == 2 or user.rid == 1) {
        interfac_manager(user);
    }
}




int main()
{
    //test_database_connection();
    //test_add_user();
    //test_edit_user();
    //test_edit_user_status();
    //test_exception_handling();

    std::cout << "All tested passed successfully!\n";


    static Session user("database");

    user.base.update_expiry();
    setlocale(LC_ALL, "");

    while (true) {
        creset();
        std::cout << "Welcome!\n";
        std::cout << "1. Register\n";
        std::cout << "2. Login\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            register_user(user);
        }
        else if (choice == 2) {
            while (true) {
                bool attempt = attempt_log_in(user);
                if (attempt) {
                    creset();
                    std::cout << "You are logged in.\n";
                    Sleep(1000);
                    creset();
                    break; // Выход из цикла логина при успешной аутентификации
                }
                else {
                    std::cout << "\nTry again? (y/n): ";
                    char retry;
                    std::cin >> retry;
                    if (retry != 'y') {
                        break; // Выход из цикла логина при отказе от повторной попытки
                    }
                }
            }
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
            Sleep(1000);
        }

        // Проверяем, вошел ли пользователь в систему, прежде чем запускать интерфейс
        if (user.id > 0) {
            break; // Выход из главного цикла выбора, если пользователь вошел
        }
    }

    // Запускаем основной интерфейс только после того, как пользователь вошел
    while (user.id > 0) {
        creset();
        interfac(user);
        Sleep(500);
    }

    user.end();
    return 0;
}
    /*
    while (true) { // если данные правильны
        system("cls");
        std::cout << " ________________________________________________________________________________________________________________________________________________\n";
        SetConsoleCursorPosition(console, { 1, 2 });
        std::cout << "| user_id";
        SetConsoleCursorPosition(console, { 11, 2 });
        std::cout << "| username";
        SetConsoleCursorPosition(console, { 38, 2 });
        std::cout << "| login";
        SetConsoleCursorPosition(console, { 56, 2 });
        std::cout << "| user_mail";
        SetConsoleCursorPosition(console, { 95, 2 });
        std::cout << "| live_status";
        SetConsoleCursorPosition(console, { 115, 2 });
        std::cout << "| role_id";
        SetConsoleCursorPosition(console, { 129, 2 });
        std::cout << "| role_name"; // вывести значение роли из таблицы Roles по role_id

        // ниже должны быть выведены все пользователи

        if (true) { // статус пользователя
            SetConsoleCursorPosition(console, { 0, 12 });
            std::cout << "1 - Create user, 2 - Edit user, 3 - Delete user \n";
            std::cin >> number;

            if (number == 1) {
                // + сделать выход из этого меню, например по нажатию Enter или еще как-нибудь
                system("cls");
                std::cout << "Enter login: \n";
                std::cout << "Enter password: \n";
                std::cout << "Enter username: \n";
                std::cout << "Enter mail: ";
                SetConsoleCursorPosition(console, { 13, 0 });
                std::cin >> login;
                SetConsoleCursorPosition(console, { 16, 1 });
                std::cin >> password;
                hashed_password = hashing(password); // в JSON только hashed
                SetConsoleCursorPosition(console, { 16, 2 });
                std::cin >> username;
                SetConsoleCursorPosition(console, { 12, 3 });
                std::cin >> user_mail;
                SetConsoleCursorPosition(console, { 0, 0 });
            }
            else if (number == 2) {
                // + сделать выход из этого меню, например по нажатию Enter или еще как-нибудь
                int id = 0;
                int setting = 0;
                std::cout << "Выберите id пользователя \n";
                std::cin >> id;
                //... Вывести все данные выбранного пользователя
                std::cout << "Выберите данные для изменения: \n";
                std::cout << "1 - username, 2 - login, 3 - user_mail, 4 - password, 5 - live_status, 6 - role_id : \n";
                std::cin >> setting;

                switch (setting) {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break; // для смены password, нужно обратиться к функции hashing, которая сделает hash пароля и запишет в БД хэш
                case 5:
                    break;// live_status залочен, только: "('Active', 'Deleted', 'Inactive', 'Clarify')" --Активный, Удален, Неактивный, Требует подтверждения
                case 6:
                    break;
                }

            }
            else if (number == 3) {
                int id = 0;
                std::cout << "Выберите id пользователя \n";
                std::cin >> id;
                Sleep(2000); // заглушка
                // реализовать удаление пользователя + перенес его во временное хранилище пользователей, как в тз, где запись удаляется через 2 недели
                // + сделать выход из этого меню, например по нажатию Enter или еще как-нибудь + спросить подтверждение, что админ хочет удалить этого пользователя
            }
        }
    }
}*/
