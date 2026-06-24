#pragma once

#include <vector>
#include <sqlite3.h>

#include "account.h"
#include "transaction.h"
#include "recurring_transaction.h"

class FinanceApp {
public:
    void run();

private:
    bool running = true;
    sqlite3* db_ = nullptr;

    std::vector<Account> accounts;
    std::vector<Transaction> transactions;
    std::vector<RecurringTransaction> recurring_transactions;

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
