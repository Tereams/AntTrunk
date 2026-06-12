#include "cli.h"
#include "cli_utils.h"

#include <iostream>
#include <string>

Account readAccountFromCLI(){
    Account account;
    account.id = promptString("Account ID: ");
    account.name = promptString("Name: ");
    account.bank_name = promptString("Bank Name: ");
    account.initial_balance = promptDouble("Initial Balance: ");
    account.currency = promptString("Currency: ");
    account.description = promptString("Description: ");

    return account;
}

Transaction readTransactionFromCLI(){
    Transaction t;

    t.id = promptString("Transaction ID: ");

    int type_choice = promptInt("Type (0=Income, 1=Expense, 2=Transfer): ");

    t.type = static_cast<TransactionType>(type_choice);
    t.amount_before_tax = promptDouble("Amount before tax: ");
    t.tax_rate = promptDouble("Tax rate (e.g. 0.1 for 10%): ");
    t.total_amount = t.amount_before_tax * (1.0 + t.tax_rate);
    t.account_id = promptString("Account ID: ");

    if (t.type == TransactionType::Transfer){
        t.target_account_id = promptString("Target Account ID: ");
    }

    t.category = promptString("Category: ");
    t.date = promptString("Date (YYYY-MM-DD): ");
    t.description = promptString("Description: ");

    return t;
}

RecurringTransaction readRecurringTransactionFromCLI()
{
    RecurringTransaction rt;

    rt.id = promptString("ID: ");
    rt.name = promptString("Name: ");

    int type_choice = promptInt("Type (0=Income, 1=Expense, 2=Transfer): ");
    rt.type = static_cast<TransactionType>(type_choice);

    rt.amount = promptDouble("Amount: ");
    rt.account_id = promptString("Account ID: ");

    if (rt.type == TransactionType::Transfer){
        rt.target_account_id = promptString("Target Account ID: ");
    }
    rt.category = promptString("Category: ");

    int frequency_choice = promptInt(
            "Frequency (0=Daily, 1=Weekly, 2=Biweekly, 3=Monthly, 4=Yearly): "
        );
    rt.recurrence_rule.frequency = static_cast<Frequency>(frequency_choice);
    rt.recurrence_rule.interval = promptInt("Interval: ");

    if (rt.recurrence_rule.frequency == Frequency::Monthly){
        int day = promptInt("Day of month (1-31): ");
        rt.recurrence_rule.day_of_month = day;
    }

    if (rt.recurrence_rule.frequency == Frequency::Weekly ||
        rt.recurrence_rule.frequency == Frequency::Biweekly){
        int weekday = promptInt("Weekday (0=Mon ... 6=Sun): ");
        rt.recurrence_rule.day_of_week = static_cast<Weekday>(weekday);
    }

    rt.start_date = promptString("Start date (YYYY-MM-DD): ");
    std::string end = promptString("End date (empty if none): ");

    if (!end.empty()){
        rt.end_date = end;
    }
    rt.description = promptString("Description: ");
    return rt;
}


std::string transactionTypeToString(TransactionType type){
    switch (type){
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

std::string accountTypeToString(AccountType type){
    switch (type){
        case AccountType::Checking:   return "Checking";
        case AccountType::Savings:    return "Savings";
        case AccountType::CreditCard: return "CreditCard";
        case AccountType::Cash:       return "Cash";
        case AccountType::Investment: return "Investment";
        default:                      return "Other";
    }
}

std::string frequencyToString(Frequency f){
    switch (f){
        case Frequency::Daily:    return "Daily";
        case Frequency::Weekly:   return "Weekly";
        case Frequency::Biweekly: return "Biweekly";
        case Frequency::Monthly:  return "Monthly";
        case Frequency::Yearly:   return "Yearly";
        default:                  return "Other";
    }
}

std::string weekdayToString(Weekday d){
    switch (d){
        case Weekday::Monday:    return "Monday";
        case Weekday::Tuesday:   return "Tuesday";
        case Weekday::Wednesday: return "Wednesday";
        case Weekday::Thursday:  return "Thursday";
        case Weekday::Friday:    return "Friday";
        case Weekday::Saturday:  return "Saturday";
        case Weekday::Sunday:    return "Sunday";
        default:                 return "Other";
    }
}

void printAccount(const Account& account){
    std::cout
        << "ID: " << account.id << '\n'
        << "Name: " << account.name << '\n'
        << "Bank: " << account.bank_name << '\n'
        << "Type: " << accountTypeToString(account.type) << '\n'
        << "Currency: " << account.currency << '\n'
        << "Initial Balance: " << account.initial_balance << '\n'
        << "Description: " << account.description;
}

void printTransaction(const Transaction& t){
    std::cout
        << "ID: " << t.id << '\n'
        << "Type: " << transactionTypeToString(t.type) << '\n'
        << "Amount Before Tax: "
        << t.amount_before_tax << '\n'
        << "Tax Rate: " << t.tax_rate << '\n'
        << "Total Amount: " << t.total_amount << '\n'
        << "Account: " << t.account_id << '\n';

    if (t.type == TransactionType::Transfer){
        std::cout
            << "Target Account: "
            << t.target_account_id << '\n';
    }

    std::cout
        << "Category: " << t.category << '\n'
        << "Date: " << t.date << '\n'
        << "Description: " << t.description;
}

void printRecurringTransaction(const RecurringTransaction& rt){
    std::cout
        << "ID: " << rt.id << '\n'
        << "Name: " << rt.name << '\n'
        << "Amount: " << rt.amount << '\n'
        << "Account: " << rt.account_id << '\n';

    if (rt.type == TransactionType::Transfer){
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

    if (rt.recurrence_rule.day_of_month){
        std::cout
            << "Day of Month: "
            << *rt.recurrence_rule.day_of_month
            << '\n';
    }

    if (rt.recurrence_rule.day_of_week){
        std::cout
            << "Day of Week: "
            << weekdayToString(
                   *rt.recurrence_rule.day_of_week)
            << '\n';
    }

    std::cout
        << "Start Date: "
        << rt.start_date << '\n';

    if (rt.end_date){
        std::cout
            << "End Date: "
            << *rt.end_date << '\n';
    }

    std::cout
        << "Description: "
        << rt.description << '\n';
}
