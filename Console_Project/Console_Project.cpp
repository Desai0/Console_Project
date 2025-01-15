#include <iostream>
#include "sqlite3.h"

int main()
{
    setlocale(0, "");
    sqlite3* db;
    int rc;
    rc = sqlite3_open("database", &db);

    if (rc) {
        std::cerr << "Невозможно открыть бд: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    else {
        std::cout << "Успешно открыта БД " << std::endl;
    }

    sqlite3_close(db);

    return 0;
}
