#include "cli.h"
#include "finance_app.h"
#include "account_db.h"
#include "transaction_db.h"
#include "recurring_transaction_db.h"

#include <iostream>
#include <limits>
//#include <string>

FinanceApp::FinanceApp(){
    sqlite3_open("finance.db", &db_);
}

FinanceApp::~FinanceApp(){
    if(db_){
            sqlite3_close(db_);
        }
}

bool FinanceApp::initialize() {
    return
        account_db::create_table(db_) &&
        transaction_db::create_table(db_) &&
        recurring_transaction_db::create_table(db_);

}

void FinanceApp::run() {
    while (running) {
        showMainMenu();

        int option = readOption();

        handleOption(option);
    }
}

void FinanceApp::showMainMenu() const {
    std::cout << "\n=== AntTrunk Main Menu ===\n";
    std::cout << "1. Add account\n";
    std::cout << "2. List accounts\n";
    std::cout << "3. Add transaction\n";
    std::cout << "4. List transactions\n";
    std::cout << "5. Add recurring transaction\n";
    std::cout << "6. List recurring transactions\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose an option: ";
}

int FinanceApp::readOption() const {
    int option;

    while (!(std::cin >> option)) {
        std::cout << "Invalid input. Please enter a number: ";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    return option;
}

void FinanceApp::exitApp() {
    running = false;
    std::cout << "Goodbye.\n";
}

void FinanceApp::handleOption(int option) {
    switch (option) {
        case 1:
            addAccount();
            break;
        case 2:
            listAccounts();
            break;
        case 3:
            addTransaction();
            break;
        case 4:
            listTransactions();
            break;
        case 5:
            addRecurringTransaction();
            break;
        case 6:
            listRecurringTransactions();
            break;
        case 0:
            exitApp();
            break;
        default:
            std::cout << "Invalid option. Please try again.\n";
            break;
    }
}


void FinanceApp::addAccount() {
        Account account = readAccountFromCLI();
        if (account_db::insert(db_, account)) {
            std::cout << "Account added successfully.\n";
        } else {
            std::cout << "Failed to add account.\n";
        }
}

void FinanceApp::addTransaction(){
    Transaction t = readTransactionFromCLI();
    transaction_db::insert(db_, t);
}

void FinanceApp::addRecurringTransaction(){
    recurring_transactions.push_back(readRecurringTransactionFromCLI());
    std::cout << "Recurring transaction added successfully.\n";
}

void FinanceApp::listAccounts() const {
    auto accounts = account_db::get_all(db_);
        for (const auto& account : accounts) {
            printAccount(account);
        }
}

void FinanceApp::listTransactions() const{
    for (const auto& t : transactions){
        printTransaction(t);
    }
}

void FinanceApp::listRecurringTransactions() const
{
    for (const auto& rt : recurring_transactions){
        printRecurringTransaction(rt);
    }
}
