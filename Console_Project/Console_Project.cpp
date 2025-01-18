#include <iostream>
#include <string>
#include <windows.h>

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

auto hashing(std::string password) {
    std::hash<std::string> hash_fn;
    size_t hashed_password = hash_fn(password);
    std::cout << "Хэш пароля: " << hashed_password << std::endl; // это просто для проверки, можно удалить, оно работает
    return hashed_password;
}


int main()
{
    setlocale(LC_ALL, "");
    int number = 0; // просто хрень для хранения
    std::string login;
    std::string password;
    std::string username;
    std::string user_mail;
    std::string hashed_password;
    int truefalse = 0; // тоже просто по фану

    std::cout << "1 - User registration" << "\n" << "2 - Login" << "\n";
    std::cin >> number;

    if (number == 1) {
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
    } // по мере заполнения должно заноситься в json, по хорошему, надо чтобы при аварийном завершении 
    //оно подставляло сохраненные данные автоматически, но забей, я если че это потом сделаю или не сделаю


    else {
        while (truefalse == 0) {
            system("cls");
            std::cout << "Enter login: \n";
            std::cout << "Enter password: ";
            SetConsoleCursorPosition(console, { 13, 0 });
            std::cin >> login;
            SetConsoleCursorPosition(console, { 16, 1 });
            std::cin >> password;

            if (true) { // если данные правильны
                system("cls");
                truefalse = 1;
                std::cout << "U logged in";
                Sleep(2000);
                system("cls");
            }
            else { // если данные не правильны
                system("cls");
                std::cout << "Wrong password/login";
                Sleep(2000);
            }
        }
        truefalse = 0;
        SetConsoleCursorPosition(console, { 0, 0 });
    }
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
}