#include "finance_app.h"

#include <iostream>
#include <limits>
//#include <string>

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
    Account account;

    std::cout << "Enter account id: ";
    std::getline(std::cin, account.id);

    std::cout << "Enter account name: ";
    std::getline(std::cin, account.name);

    std::cout << "Enter bank name: ";
    std::getline(std::cin, account.bank_name);

    std::cout << "Select account type:\n";
    std::cout << "1. Checking\n";
    std::cout << "2. Savings\n";
    std::cout << "3. Credit Card\n";
    std::cout << "4. Cash\n";
    std::cout << "5. Investment\n";
    std::cout << "6. Other\n";
    std::cout << "Choice: ";

    int type_choice;
    std::cin >> type_choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (type_choice) {
        case 1:
            account.type = AccountType::Checking;
            break;
        case 2:
            account.type = AccountType::Savings;
            break;
        case 3:
            account.type = AccountType::CreditCard;
            break;
        case 4:
            account.type = AccountType::Cash;
            break;
        case 5:
            account.type = AccountType::Investment;
            break;
        case 6:
            account.type = AccountType::Other;
            break;
        default:
            std::cout << "Invalid choice. Set to Other by default.\n";
            account.type = AccountType::Other;
            break;
    }

    std::cout << "Enter currency [default: USD]: ";
    std::string currency;
    std::getline(std::cin, currency);

    if (!currency.empty()) {
        account.currency = currency;
    }

    std::cout << "Enter initial balance: ";
    std::cin >> account.initial_balance;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter description: ";
    std::getline(std::cin, account.description);

    accounts.push_back(account);

    std::cout << "Account added successfully.\n";
}
