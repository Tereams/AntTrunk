#include "include/Transaction.h"

Transaction::Transaction(
    const std::string& date,
    TransactionType type,
    const std::string& account,
    const std::string& category,
    double amount,
    const std::string& note
)
    : date(date),
      type(type),
      account(account),
      category(category),
      amount(amount),
      note(note) {}
