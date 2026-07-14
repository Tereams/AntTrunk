#pragma once

#include <vector>
#include <sqlite3.h>

#include "model/account.h"
#include "model/transaction.h"
#include "model/recurring_transaction.h"

class MUIApp {
public:
    explicit MUIApp(sqlite3* db);
    void run();

private:
    sqlite3* db_ = nullptr;

private:
    void showMainMenu() const;
    int readOption() const;
    void handleOption(int option);

    void addAccount();
    void listAccounts() const;

    void addTransaction();
    void listTransactions() const;

    void addRecurringTransaction();
    void listRecurringTransactions() const;

    void exitApp();
};
