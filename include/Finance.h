#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Transaction.h"
#include "Account.h"

namespace finance {

double calculateBalance(
    const Account& account,
    const std::vector<Transaction>& transactions
);

std::unordered_map<std::string, double> calculateAllBalances(
    const std::vector<Account>& accounts,
    const std::vector<Transaction>& transactions
);

}
