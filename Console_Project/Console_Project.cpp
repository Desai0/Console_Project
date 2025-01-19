#include <iostream>
#include <windows.h>
#include "sessions.h"
#include <typeinfo>


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void creset() {
    system("cls");
    SetConsoleCursorPosition(console, { 0, 0 });
}

/*
Парооли от тестовой базы данных по айди

1-9 "12345"
2 - 123
10 "n0ts"
11 я забыл лмао
12 - l
*/
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

    if (user.log_in(login, password)) {
        creset();
        std::cout << "Logged in succesfully";
        return true;
    }
    else {
        creset();
        std::cout << "Login or password error, try again or leave, coward";
        return false;
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

            while (true)
            {
                std::cout << "Select an attribute to edit"
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
                default:
                    std::cout << "No, you moron! That is not an option! Try again";
                    continue;
                }

                break;
            }


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
        std::cout << "0 - Cancel, 1 - Edit profile, 2 - Edit User, 3 - Add User\n";
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
    static Session user("database");

    setlocale(LC_ALL, "");
    int number = 0; // просто хрень для хранения

    while (true) {

        bool attempt = attempt_log_in(user);

        if (true) { // если данные правильны
            system("cls");
            std::cout << "U logged in";
            Sleep(2000);
            system("cls");
            break;
        }
        else { // если данные не правильны
            system("cls");
            std::cout << "Wrong password/login";
            Sleep(2000);
        }
    }
    SetConsoleCursorPosition(console, { 0, 0 });

    
    while (true) {
        creset();
        interfac(user);
        Sleep(500);
    }

    user.end();
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
    }*/
}
