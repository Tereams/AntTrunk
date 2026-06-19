#include "account_db.h"

#include <iostream>

namespace {

int to_int(AccountType type) {
    return static_cast<int>(type);
}

AccountType to_account_type(int value) {
    switch (value) {
        case 0: return AccountType::Checking;
        case 1: return AccountType::Savings;
        case 2: return AccountType::CreditCard;
        case 3: return AccountType::Cash;
        case 4: return AccountType::Investment;
        default: return AccountType::Other;
    }
}

bool exec(sqlite3* db, const char* sql) {
    char* err = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);

    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << err << "\n";
        sqlite3_free(err);
        return false;
    }

    return true;
}

}

namespace account_db {

bool create_table(sqlite3* db) {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS accounts (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            bank_name TEXT,
            type INTEGER NOT NULL,
            currency TEXT NOT NULL,
            initial_balance REAL NOT NULL,
            description TEXT
        );
    )";

    return exec(db, sql);
}

bool insert(sqlite3* db, const Account& account) {
    const char* sql = R"(
        INSERT INTO accounts
        (id, name, bank_name, type, currency, initial_balance, description)
        VALUES (?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, account.id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, account.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, account.bank_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, to_int(account.type));
    sqlite3_bind_text(stmt, 5, account.currency.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 6, account.initial_balance);
    sqlite3_bind_text(stmt, 7, account.description.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

std::optional<Account> find_by_id(sqlite3* db, const std::string& id) {
    const char* sql = R"(
        SELECT id, name, bank_name, type, currency, initial_balance, description
        FROM accounts
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    Account account;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        account.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        account.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        account.bank_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        account.type = to_account_type(sqlite3_column_int(stmt, 3));
        account.currency = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        account.initial_balance = sqlite3_column_double(stmt, 5);
        account.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        sqlite3_finalize(stmt);
        return account;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<Account> get_all(sqlite3* db) {
    const char* sql = R"(
        SELECT id, name, bank_name, type, currency, initial_balance, description
        FROM accounts;
    )";

    sqlite3_stmt* stmt = nullptr;
    std::vector<Account> accounts;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return accounts;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Account account;

        account.id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        account.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        account.bank_name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        account.type = to_account_type(sqlite3_column_int(stmt, 3));
        account.currency = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        account.initial_balance = sqlite3_column_double(stmt, 5);
        account.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        accounts.push_back(account);
    }

    sqlite3_finalize(stmt);
    return accounts;
}

bool update(sqlite3* db, const Account& account) {
    const char* sql = R"(
        UPDATE accounts
        SET name = ?,
            bank_name = ?,
            type = ?,
            currency = ?,
            initial_balance = ?,
            description = ?
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, account.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, account.bank_name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, to_int(account.type));
    sqlite3_bind_text(stmt, 4, account.currency.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 5, account.initial_balance);
    sqlite3_bind_text(stmt, 6, account.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, account.id.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

bool remove(sqlite3* db, const std::string& id) {
    const char* sql = "DELETE FROM accounts WHERE id = ?;";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

}
