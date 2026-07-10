#pragma once

#include "model/account.h"
#include "model/transaction.h"
#include "model/recurring_transaction.h"

Account readAccountFromCLI();
Transaction readTransactionFromCLI();
RecurringTransaction readRecurringTransactionFromCLI();

void printAccount(const Account& account);
void printTransaction(const Transaction& transaction);
void printRecurringTransaction(const RecurringTransaction& rt);
