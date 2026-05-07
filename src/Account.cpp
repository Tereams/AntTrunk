#include "include/Account.h"

Account::Account(
    const std::string& name,
    double initial_balance
)
    : name(name),
      initial_balance(initial_balance) {}
