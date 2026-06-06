#include "cli.h"
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


std::string accountTypeToString(AccountType type)
{
    switch (type)
    {
        case AccountType::Checking:   return "Checking";
        case AccountType::Savings:    return "Savings";
        case AccountType::CreditCard: return "CreditCard";
        case AccountType::Cash:       return "Cash";
        case AccountType::Investment: return "Investment";
        default:                      return "Other";
    }
}


std::ostream& operator<<(std::ostream& os, const Account& account)
{
    os << "ID: " << account.id << '\n'
       << "Name: " << account.name << '\n'
       << "Bank: " << account.bank_name << '\n'
       << "Type: " << accountTypeToString(account.type) << '\n'
       << "Currency: " << account.currency << '\n'
       << "Initial Balance: " << account.initial_balance << '\n'
       << "Description: " << account.description;

    return os;
}

void FinanceApp::listAccounts() const
{
    for (const auto& account : accounts)
        {
            std::cout << account << "\n\n";
        }
}

std::string transactionTypeToString(TransactionType type)
{
    switch (type)
    {
        case TransactionType::Income:
            return "Income";

        case TransactionType::Expense:
            return "Expense";

        case TransactionType::Transfer:
            return "Transfer";

        default:
            return "Unknown";
    }
}

std::ostream& operator<<(std::ostream& os,
                         const Transaction& t)
{
    os
        << "ID: " << t.id << '\n'
        << "Type: " << transactionTypeToString(t.type) << '\n'
        << "Amount Before Tax: "
        << t.amount_before_tax << '\n'
        << "Tax Rate: " << t.tax_rate << '\n'
        << "Total Amount: " << t.total_amount << '\n'
        << "Account: " << t.account_id << '\n';

    if (t.type == TransactionType::Transfer)
    {
        os << "Target Account: "
           << t.target_account_id << '\n';
    }

    os << "Category: " << t.category << '\n'
       << "Date: " << t.date << '\n'
       << "Description: " << t.description;

    return os;
}

void FinanceApp::listTransactions() const
{
    for (const auto& t : transactions)
    {
        std::cout << t << "\n\n";
    }
}


std::string frequencyToString(Frequency f)
{
    switch (f)
    {
        case Frequency::Daily:    return "Daily";
        case Frequency::Weekly:   return "Weekly";
        case Frequency::Biweekly: return "Biweekly";
        case Frequency::Monthly:  return "Monthly";
        case Frequency::Yearly:   return "Yearly";
    }

    return "Unknown";
}

std::string weekdayToString(Weekday d)
{
    switch (d)
    {
        case Weekday::Monday:    return "Monday";
        case Weekday::Tuesday:   return "Tuesday";
        case Weekday::Wednesday: return "Wednesday";
        case Weekday::Thursday:  return "Thursday";
        case Weekday::Friday:    return "Friday";
        case Weekday::Saturday:  return "Saturday";
        case Weekday::Sunday:    return "Sunday";
    }

    return "Unknown";
}

void FinanceApp::addTransaction(){
    transactions.push_back(readTransactionFromCLI());
    std::cout << "Transaction added successfully.\n";
}

void FinanceApp::addRecurringTransaction(){
    recurring_transactions.push_back(readRecurringTransactionFromCLI());

    std::cout << "Recurring transaction added successfully.\n";
}

void FinanceApp::listRecurringTransactions() const
{
    for (const auto& rt : recurring_transactions)
    {
        std::cout
            << "ID: " << rt.id << '\n'
            << "Name: " << rt.name << '\n'
            << "Amount: " << rt.amount << '\n'
            << "Account: " << rt.account_id << '\n';

        if (rt.type == TransactionType::Transfer)
        {
            std::cout
                << "Target Account: "
                << rt.target_account_id << '\n';
        }

        std::cout
            << "Category: " << rt.category << '\n'
            << "Frequency: "
            << frequencyToString(
                   rt.recurrence_rule.frequency)
            << '\n';

        std::cout
            << "Interval: "
            << rt.recurrence_rule.interval
            << '\n';

        if (rt.recurrence_rule.day_of_month)
        {
            std::cout
                << "Day of Month: "
                << *rt.recurrence_rule.day_of_month
                << '\n';
        }

        if (rt.recurrence_rule.day_of_week)
        {
            std::cout
                << "Day of Week: "
                << weekdayToString(
                       *rt.recurrence_rule.day_of_week)
                << '\n';
        }

        std::cout
            << "Start Date: "
            << rt.start_date << '\n';

        if (rt.end_date)
        {
            std::cout
                << "End Date: "
                << *rt.end_date << '\n';
        }

        std::cout
            << "Description: "
            << rt.description << '\n';

        std::cout
            << "----------------------------------\n";
    }
}
