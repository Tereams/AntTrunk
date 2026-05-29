// Finance.cpp

#include "finance.h"
#include "transaction.h"

namespace finance {

double calculateBalance(
    const Account& account,
    const std::vector<Transaction>& transactions
) {
    double balance = account.initial_balance;

    for (const auto& transaction : transactions) {

        if (transaction.account_id != account.id) {
            continue;
        }

        switch (transaction.type) {

            case TransactionType::Income:
                balance += transaction.amount_before_tax;
                break;

            case TransactionType::Expense:
                balance -= transaction.amount_before_tax;
                break;

            case TransactionType::Transfer:
                break;
        }
    }

    return balance;
}

std::unordered_map<std::string, double> calculateAllBalances(
    const std::vector<Account>& accounts,
    const std::vector<Transaction>& transactions
) {
    std::unordered_map<std::string, double> balances;

    for (const auto& account : accounts) {
        balances[account.name] =
            calculateBalance(account, transactions);
    }

    return balances;
}

}
