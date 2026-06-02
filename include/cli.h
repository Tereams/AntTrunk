#pragma once

#include "account.h"
#include "transaction.h"
#include "recurring_transaction.h"

Account readAccountFromCLI();
Transaction readTransactionFromCLI();
RecurringTransaction readRecurringTransactionFromCLI();

void printAccount(const Account& account);
void printTransaction(const Transaction& transaction);
void printRecurringTransaction(const RecurringTransaction& rt);
