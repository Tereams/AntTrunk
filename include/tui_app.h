#pragma once

#include <sqlite3.h>

class TuiApp {
public:
    explicit TuiApp(sqlite3* db);

    void run();

private:
    sqlite3* db_;
};
