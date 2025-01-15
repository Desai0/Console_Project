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




    const char* sql = "SELECT username FROM Users;";
    sqlite3_stmt* stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            std::cout << "Имя: " << sqlite3_column_text(stmt, 0) << std::endl;
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return 0;
}
