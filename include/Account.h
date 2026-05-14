#ifndef ANTTRUNK_ACCOUNT_HPP
#define ANTTRUNK_ACCOUNT_HPP

#include <string>

namespace anttrunk {

enum class AccountType {
    Checking,
    Savings,
    CreditCard,
    Cash,
    Investment,
    Other
};

struct Account {
    std::string id;

    std::string name;
    std::string bank_name;
    AccountType type = AccountType::Other;

    std::string currency = "USD";

    // The starting balance recorded by the user.
    // Current balance should be calculated from this value plus transactions.
    double initial_balance = 0.0;

    std::string description;
};

} // namespace anttrunk

#endif // ANTTRUNK_ACCOUNT_HPP
