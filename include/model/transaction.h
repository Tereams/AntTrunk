#pragma once

#include <string>

enum class TransactionType {
    Income,
    Expense,
    Transfer
};

struct Transaction {
    std::string id;

    TransactionType type = TransactionType::Expense;

    // For most expenses, this can be the pre-tax amount.
    // For income, transfer, or tax-free expenses, tax_rate can simply be 0.
    double amount_before_tax = 0.0;
    double tax_rate = 0.0;

    // This can be calculated from amount_before_tax and tax_rate,
    // but is stored here for convenience and explicitness.
    double total_amount = 0.0;

    std::string account_id;

    // Used only when type == Transfer.
    // For Expense or Income, this field can be empty.
    std::string target_account_id;

    std::string category;
    std::string date; // Expected format: YYYY-MM-DD
    std::string description;
};
