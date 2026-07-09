#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "model/transaction.h"
#include "model/account.h"

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
