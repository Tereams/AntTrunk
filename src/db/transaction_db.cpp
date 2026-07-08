#include "transaction_db.h"

#include <iostream>

namespace {

int to_int(TransactionType type) {
    return static_cast<int>(type);
}

TransactionType to_transaction_type(int value) {
    switch (value) {
        case 0: return TransactionType::Income;
        case 1: return TransactionType::Expense;
        case 2: return TransactionType::Transfer;
        default: return TransactionType::Expense;
    }
}

double calculate_total(double amount_before_tax, double tax_rate) {
    return amount_before_tax * (1.0 + tax_rate);
}

std::string column_text(sqlite3_stmt* stmt, int index) {
    const unsigned char* text = sqlite3_column_text(stmt, index);
    return text ? reinterpret_cast<const char*>(text) : "";
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

Transaction read_transaction(sqlite3_stmt* stmt) {
    Transaction t;

    t.id = column_text(stmt, 0);
    t.type = to_transaction_type(sqlite3_column_int(stmt, 1));
    t.amount_before_tax = sqlite3_column_double(stmt, 2);
    t.tax_rate = sqlite3_column_double(stmt, 3);

    t.total_amount = calculate_total(t.amount_before_tax, t.tax_rate);

    t.account_id = column_text(stmt, 4);
    t.target_account_id = column_text(stmt, 5);
    t.category = column_text(stmt, 6);
    t.date = column_text(stmt, 7);
    t.description = column_text(stmt, 8);

    return t;
}

}

namespace transaction_db {

bool create_table(sqlite3* db) {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id TEXT PRIMARY KEY,

            type INTEGER NOT NULL,

            amount_before_tax REAL NOT NULL,
            tax_rate REAL NOT NULL,

            account_id TEXT NOT NULL,
            target_account_id TEXT,

            category TEXT,
            date TEXT NOT NULL,

            description TEXT
        );
    )";

    return exec(db, sql);
}

bool insert(sqlite3* db, const Transaction& t) {
    const char* sql = R"(
        INSERT INTO transactions
        (
            id,
            type,
            amount_before_tax,
            tax_rate,
            account_id,
            target_account_id,
            category,
            date,
            description
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, t.id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, to_int(t.type));
    sqlite3_bind_double(stmt, 3, t.amount_before_tax);
    sqlite3_bind_double(stmt, 4, t.tax_rate);
    sqlite3_bind_text(stmt, 5, t.account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, t.target_account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, t.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, t.date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, t.description.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

std::optional<Transaction> find_by_id(sqlite3* db, const std::string& id) {
    const char* sql = R"(
        SELECT
            id,
            type,
            amount_before_tax,
            tax_rate,
            account_id,
            target_account_id,
            category,
            date,
            description
        FROM transactions
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        Transaction t = read_transaction(stmt);
        sqlite3_finalize(stmt);
        return t;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<Transaction> get_all(sqlite3* db) {
    const char* sql = R"(
        SELECT
            id,
            type,
            amount_before_tax,
            tax_rate,
            account_id,
            target_account_id,
            category,
            date,
            description
        FROM transactions
        ORDER BY date ASC;
    )";

    sqlite3_stmt* stmt = nullptr;
    std::vector<Transaction> transactions;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return transactions;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        transactions.push_back(read_transaction(stmt));
    }

    sqlite3_finalize(stmt);
    return transactions;
}

bool update(sqlite3* db, const Transaction& t) {
    const char* sql = R"(
        UPDATE transactions
        SET
            type = ?,
            amount_before_tax = ?,
            tax_rate = ?,
            account_id = ?,
            target_account_id = ?,
            category = ?,
            date = ?,
            description = ?
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_int(stmt, 1, to_int(t.type));
    sqlite3_bind_double(stmt, 2, t.amount_before_tax);
    sqlite3_bind_double(stmt, 3, t.tax_rate);
    sqlite3_bind_text(stmt, 4, t.account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, t.target_account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, t.category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, t.date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, t.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, t.id.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

bool remove(sqlite3* db, const std::string& id) {
    const char* sql = R"(
        DELETE FROM transactions
        WHERE id = ?;
    )";

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
