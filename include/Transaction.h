#pragma once

#include <string>

enum class TransactionType {
    Income,
    Expense
};

struct Transaction {
    std::string date;       // format: YYYY-MM-DD
    TransactionType type;
    std::string account;    // account name
    std::string category;   // e.g. groceries, rent, salary
    double amount;
    std::string note;

    Transaction(
        const std::string& date,
        TransactionType type,
        const std::string& account,
        const std::string& category,
        double amount,
        const std::string& note = ""
    );
};
