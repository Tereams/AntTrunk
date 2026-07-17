#include "ui/tui_app.h"

#include "db/account_db.h"
#include "db/transaction_db.h"
#include "db/recurring_transaction_db.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include <vector>

using namespace ftxui;

namespace {

std::string money(double value) {
    return "$" + std::to_string(value);
}

std::vector<std::string> load_account_items(sqlite3* db) {
    std::vector<std::string> items;

    for (const auto& a : account_db::get_all(db)) {
        items.push_back(a.name + "  " + a.currency + "  " + money(a.initial_balance));
    }

    if (items.empty()) {
        items.push_back("(no accounts)");
    }

    return items;
}

std::vector<std::string> load_transaction_items(sqlite3* db) {
    std::vector<std::string> items;

    for (const auto& t : transaction_db::get_all(db)) {
        items.push_back(t.date + "  " + t.category + "  " + money(t.total_amount));
    }

    if (items.empty()) {
        items.push_back("(no transactions)");
    }

    return items;
}

std::vector<std::string> load_recurring_items(sqlite3* db) {
    std::vector<std::string> items;

    for (const auto& r : recurring_transaction_db::get_all(db)) {
        items.push_back(r.name + "  " + r.category + "  " + money(r.amount));
    }

    if (items.empty()) {
        items.push_back("(no recurring transactions)");
    }

    return items;
}

}

TuiApp::TuiApp(sqlite3* db)
    : db_(db) {}

void TuiApp::run() {
    auto screen = ScreenInteractive::TerminalOutput();

    enum class Page {
        Main,
        Accounts,
        Transactions,
        Recurring
    };

    Page page = Page::Main;

    std::vector<std::string> main_items = {
        "Accounts",
        "Transactions",
        "Recurring Transactions",
        "Quit"
    };

    int main_selected = 0;
    int list_selected = 0;

    std::vector<std::string> list_items;

    auto main_menu = Menu(&main_items, &main_selected);
    auto list_menu = Menu(&list_items, &list_selected);

    auto open_button = Button("Open", [&] {
        if (main_selected == 0) {
            page = Page::Accounts;
            list_items = load_account_items(db_);
        } else if (main_selected == 1) {
            page = Page::Transactions;
            list_items = load_transaction_items(db_);
        } else if (main_selected == 2) {
            page = Page::Recurring;
            list_items = load_recurring_items(db_);
        } else {
            screen.Exit();
        }

        list_selected = 0;
    });

    auto back_button = Button("Back", [&] {
        page = Page::Main;
    });

    auto quit_button = Button("Quit", [&] {
        screen.Exit();
    });

    auto main_container = Container::Vertical({
        main_menu,
        open_button,
        quit_button,
    });

    auto list_container = Container::Vertical({
        list_menu,
        back_button,
        quit_button,
    });

    auto root = Container::Tab(
        {
            main_container,
            list_container,
        },
        [&] {
            return page == Page::Main ? 0 : 1;
        }
    );

    auto renderer = Renderer(root, [&] {
        if (page == Page::Main) {
            return vbox({
                text("AntTrunk") | bold | center,
                separator(),
                main_menu->Render(),
                separator(),
                hbox({
                    open_button->Render(),
                    text(" "),
                    quit_button->Render(),
                }) | center,
            }) | border;
        }

        std::string title;

        if (page == Page::Accounts) {
            title = "Accounts";
        } else if (page == Page::Transactions) {
            title = "Transactions";
        } else {
            title = "Recurring Transactions";
        }

        return vbox({
            text(title) | bold | center,
            separator(),
            list_menu->Render() | flex,
            separator(),
            hbox({
                back_button->Render(),
                text(" "),
                quit_button->Render(),
            }) | center,
        }) | border;
    });

    screen.Loop(renderer);
}