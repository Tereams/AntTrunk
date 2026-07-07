#pragma once

#include "account.h"

#include <sqlite3.h>
#include <optional>
#include <string>
#include <vector>

namespace account_db {

bool create_table(sqlite3* db);

bool insert(sqlite3* db, const Account& account);

std::optional<Account> find_by_id(sqlite3* db, const std::string& id);

std::vector<Account> get_all(sqlite3* db);

bool update(sqlite3* db, const Account& account);

bool remove(sqlite3* db, const std::string& id);

}
