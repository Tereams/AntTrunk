#pragma once

#include "model/recurring_transaction.h"

#include <sqlite3.h>
#include <optional>
#include <string>
#include <vector>

namespace recurring_transaction_db {

bool create_table(sqlite3* db);

bool insert(sqlite3* db, const RecurringTransaction& recurring);

std::optional<RecurringTransaction> find_by_id(sqlite3* db, const std::string& id);

std::vector<RecurringTransaction> get_all(sqlite3* db);

bool update(sqlite3* db, const RecurringTransaction& recurring);

bool remove(sqlite3* db, const std::string& id);

}
