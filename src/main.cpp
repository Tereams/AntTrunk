
#include "ui/mui_app.h"
#include "ui/tui_app.h"

#include "db/account_db.h"
#include "db/transaction_db.h"
#include "db/recurring_transaction_db.h"

#include <sqlite3.h>
#include <iostream>

int main(int argc, char* argv[]) {
    sqlite3* db = nullptr;

    if (sqlite3_open("finance.db", &db) != SQLITE_OK) {
        std::cerr << "Failed to open database.\n";
        return 1;
    }

    account_db::create_table(db);
    transaction_db::create_table(db);
    recurring_transaction_db::create_table(db);

    if (argc > 1 && std::string(argv[1]) == "--cli") {
        MUIApp app(db);
        app.run();
    } else {
        TuiApp app(db);
        app.run();
    }

    sqlite3_close(db);
    return 0;
}
