#include "include/FinanceLogic.h"

double FinanceLogic::calculateBalance(
    const Account& account,
    const std::vector<Transaction>& transactions
) {
    double balance = account.initial_balance;

    for (const auto& transaction : transactions) {
        if (transaction.account != account.name) {
            continue;
        }

        if (transaction.type == TransactionType::Income) {
            balance += transaction.amount;
        } else if (transaction.type == TransactionType::Expense) {
            balance -= transaction.amount;
        }
    }

    return balance;
}

std::unordered_map<std::string, double> FinanceLogic::calculateAllBalances(
    const std::vector<Account>& accounts,
    const std::vector<Transaction>& transactions
) {
    std::unordered_map<std::string, double> balances;

    for (const auto& account : accounts) {
        balances[account.name] = calculateBalance(account, transactions);
    }

    return balances;
}
