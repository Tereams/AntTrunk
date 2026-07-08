#include "recurring_transaction_db.h"

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

int to_int(Frequency frequency) {
    return static_cast<int>(frequency);
}

Frequency to_frequency(int value) {
    switch (value) {
        case 0: return Frequency::Daily;
        case 1: return Frequency::Weekly;
        case 2: return Frequency::Biweekly;
        case 3: return Frequency::Monthly;
        case 4: return Frequency::Yearly;
        default: return Frequency::Monthly;
    }
}

int to_int(Weekday weekday) {
    return static_cast<int>(weekday);
}

Weekday to_weekday(int value) {
    switch (value) {
        case 0: return Weekday::Monday;
        case 1: return Weekday::Tuesday;
        case 2: return Weekday::Wednesday;
        case 3: return Weekday::Thursday;
        case 4: return Weekday::Friday;
        case 5: return Weekday::Saturday;
        case 6: return Weekday::Sunday;
        default: return Weekday::Monday;
    }
}

std::string column_text(sqlite3_stmt* stmt, int index) {
    const unsigned char* text = sqlite3_column_text(stmt, index);
    return text ? reinterpret_cast<const char*>(text) : "";
}

std::optional<int> column_optional_int(sqlite3_stmt* stmt, int index) {
    if (sqlite3_column_type(stmt, index) == SQLITE_NULL) {
        return std::nullopt;
    }

    return sqlite3_column_int(stmt, index);
}

std::optional<std::string> column_optional_text(sqlite3_stmt* stmt, int index) {
    if (sqlite3_column_type(stmt, index) == SQLITE_NULL) {
        return std::nullopt;
    }

    return column_text(stmt, index);
}

void bind_optional_int(sqlite3_stmt* stmt, int index, const std::optional<int>& value) {
    if (value.has_value()) {
        sqlite3_bind_int(stmt, index, *value);
    } else {
        sqlite3_bind_null(stmt, index);
    }
}

void bind_optional_weekday(sqlite3_stmt* stmt, int index, const std::optional<Weekday>& value) {
    if (value.has_value()) {
        sqlite3_bind_int(stmt, index, to_int(*value));
    } else {
        sqlite3_bind_null(stmt, index);
    }
}

void bind_optional_text(sqlite3_stmt* stmt, int index, const std::optional<std::string>& value) {
    if (value.has_value()) {
        sqlite3_bind_text(stmt, index, value->c_str(), -1, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_null(stmt, index);
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

RecurringTransaction read_recurring_transaction(sqlite3_stmt* stmt) {
    RecurringTransaction r;

    r.id = column_text(stmt, 0);
    r.name = column_text(stmt, 1);
    r.type = to_transaction_type(sqlite3_column_int(stmt, 2));
    r.amount = sqlite3_column_double(stmt, 3);

    r.account_id = column_text(stmt, 4);
    r.target_account_id = column_text(stmt, 5);
    r.category = column_text(stmt, 6);

    r.recurrence_rule.frequency = to_frequency(sqlite3_column_int(stmt, 7));
    r.recurrence_rule.interval = sqlite3_column_int(stmt, 8);

    r.recurrence_rule.day_of_month = column_optional_int(stmt, 9);

    auto weekday_value = column_optional_int(stmt, 10);
    if (weekday_value.has_value()) {
        r.recurrence_rule.day_of_week = to_weekday(*weekday_value);
    } else {
        r.recurrence_rule.day_of_week = std::nullopt;
    }

    r.start_date = column_text(stmt, 11);
    r.end_date = column_optional_text(stmt, 12);
    r.description = column_text(stmt, 13);

    return r;
}

}

namespace recurring_transaction_db {

bool create_table(sqlite3* db) {
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS recurring_transactions (
            id TEXT PRIMARY KEY,

            name TEXT NOT NULL,
            type INTEGER NOT NULL,

            amount REAL NOT NULL,

            account_id TEXT NOT NULL,
            target_account_id TEXT,

            category TEXT,

            frequency INTEGER NOT NULL,
            interval INTEGER NOT NULL,

            day_of_month INTEGER,
            day_of_week INTEGER,

            start_date TEXT NOT NULL,
            end_date TEXT,

            description TEXT
        );
    )";

    return exec(db, sql);
}

bool insert(sqlite3* db, const RecurringTransaction& r) {
    const char* sql = R"(
        INSERT INTO recurring_transactions
        (
            id,
            name,
            type,
            amount,
            account_id,
            target_account_id,
            category,
            frequency,
            interval,
            day_of_month,
            day_of_week,
            start_date,
            end_date,
            description
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, r.id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, r.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, to_int(r.type));
    sqlite3_bind_double(stmt, 4, r.amount);

    sqlite3_bind_text(stmt, 5, r.account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, r.target_account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7, r.category.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_int(stmt, 8, to_int(r.recurrence_rule.frequency));
    sqlite3_bind_int(stmt, 9, r.recurrence_rule.interval);

    bind_optional_int(stmt, 10, r.recurrence_rule.day_of_month);
    bind_optional_weekday(stmt, 11, r.recurrence_rule.day_of_week);

    sqlite3_bind_text(stmt, 12, r.start_date.c_str(), -1, SQLITE_TRANSIENT);
    bind_optional_text(stmt, 13, r.end_date);

    sqlite3_bind_text(stmt, 14, r.description.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

std::optional<RecurringTransaction> find_by_id(sqlite3* db, const std::string& id) {
    const char* sql = R"(
        SELECT
            id,
            name,
            type,
            amount,
            account_id,
            target_account_id,
            category,
            frequency,
            interval,
            day_of_month,
            day_of_week,
            start_date,
            end_date,
            description
        FROM recurring_transactions
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return std::nullopt;
    }

    sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        RecurringTransaction r = read_recurring_transaction(stmt);
        sqlite3_finalize(stmt);
        return r;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<RecurringTransaction> get_all(sqlite3* db) {
    const char* sql = R"(
        SELECT
            id,
            name,
            type,
            amount,
            account_id,
            target_account_id,
            category,
            frequency,
            interval,
            day_of_month,
            day_of_week,
            start_date,
            end_date,
            description
        FROM recurring_transactions
        ORDER BY start_date ASC;
    )";

    sqlite3_stmt* stmt = nullptr;
    std::vector<RecurringTransaction> result;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        result.push_back(read_recurring_transaction(stmt));
    }

    sqlite3_finalize(stmt);
    return result;
}

bool update(sqlite3* db, const RecurringTransaction& r) {
    const char* sql = R"(
        UPDATE recurring_transactions
        SET
            name = ?,
            type = ?,
            amount = ?,
            account_id = ?,
            target_account_id = ?,
            category = ?,
            frequency = ?,
            interval = ?,
            day_of_month = ?,
            day_of_week = ?,
            start_date = ?,
            end_date = ?,
            description = ?
        WHERE id = ?;
    )";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    sqlite3_bind_text(stmt, 1, r.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, to_int(r.type));
    sqlite3_bind_double(stmt, 3, r.amount);

    sqlite3_bind_text(stmt, 4, r.account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, r.target_account_id.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, r.category.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_int(stmt, 7, to_int(r.recurrence_rule.frequency));
    sqlite3_bind_int(stmt, 8, r.recurrence_rule.interval);

    bind_optional_int(stmt, 9, r.recurrence_rule.day_of_month);
    bind_optional_weekday(stmt, 10, r.recurrence_rule.day_of_week);

    sqlite3_bind_text(stmt, 11, r.start_date.c_str(), -1, SQLITE_TRANSIENT);
    bind_optional_text(stmt, 12, r.end_date);

    sqlite3_bind_text(stmt, 13, r.description.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 14, r.id.c_str(), -1, SQLITE_TRANSIENT);

    bool ok = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    return ok;
}

bool remove(sqlite3* db, const std::string& id) {
    const char* sql = R"(
        DELETE FROM recurring_transactions
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
