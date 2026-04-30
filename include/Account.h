#pragma once

#include <string>

struct Account {
    std::string name;
    double initial_balance;
    Account(
        const std::string& name,
        double initial_balance
    );
};
