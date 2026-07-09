#pragma once

#include <vector>
#include <sqlite3.h>

#include "model/account.h"
#include "model/transaction.h"
#include "model/recurring_transaction.h"

class FinanceApp {
public:
    FinanceApp();
    ~FinanceApp();
    bool initialize();
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
