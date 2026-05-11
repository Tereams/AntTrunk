#include <iostream>
#include <vector>
#include "../include/Account.h"
#include "../include/Transaction.h"
#include "../include/Finance.h"

int main() {
    std::vector<Account> accounts = {
        Account("checking", 1000.0),
        Account("savings", 5000.0)
    };

    std::vector<Transaction> transactions = {
        Transaction("2026-05-01", TransactionType::Income, "checking", "salary", 2000.0, "Paycheck"),
        Transaction("2026-05-02", TransactionType::Expense, "checking", "groceries", 50.0, "Walmart"),
        Transaction("2026-05-03", TransactionType::Expense, "checking", "rent", 300.0, "May rent"),
        Transaction("2026-05-04", TransactionType::Income, "savings", "interest", 10.0, "Bank interest")
    };

    auto balances = FinanceLogic::calculateAllBalances(accounts, transactions);

    for (std::unordered_map<std::string, double>::const_iterator it = balances.begin();
         it != balances.end();
         ++it) {
        const std::string& account_name = it->first;
        double balance = it->second;

        std::cout << account_name << ": " << balance << std::endl;
    }



    return 0;
}
