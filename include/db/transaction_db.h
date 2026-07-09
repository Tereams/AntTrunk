#pragma once

#include "model/transaction.h"

#include <sqlite3.h>
#include <optional>
#include <string>
#include <vector>

namespace transaction_db {

bool create_table(sqlite3* db);

bool insert(sqlite3* db, const Transaction& transaction);

std::optional<Transaction> find_by_id(sqlite3* db, const std::string& id);

std::vector<Transaction> get_all(sqlite3* db);

bool update(sqlite3* db, const Transaction& transaction);

bool remove(sqlite3* db, const std::string& id);

}
