#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "Transaction.h"
#include "Account.h"

class FinanceLogic {
public:

    // Calculate balance for a single account
    static double calculateBalance(
        const Account& account,
        const std::vector<Transaction>& transactions
    );

    // Calculate balances for all accounts
    static std::unordered_map<std::string, double> calculateAllBalances(
        const std::vector<Account>& accounts,
        const std::vector<Transaction>& transactions
    );

};
