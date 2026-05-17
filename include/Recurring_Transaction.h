#include <optional>
#include <string>

#include "Recurrence_Rule.h"
#include "transaction.h"

struct RecurringTransaction {
    std::string id;

    std::string name;
    TransactionType type = TransactionType::Expense;

    double amount = 0.0;

    std::string account_id;

    // Used only when type == Transfer.
    // For Expense or Income, this field can be empty.
    std::string target_account_id;

    std::string category;
    RecurrenceRule recurrence_rule;

    std::string start_date; // Expected format: YYYY-MM-DD

    // If there is no fixed end date, this can be empty.
    std::optional<std::string> end_date;

    std::string description;
};
