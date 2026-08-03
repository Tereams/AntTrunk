#include "ui/tui_app.h"

#include "db/account_db.h"
#include "db/recurring_transaction_db.h"
#include "db/transaction_db.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include <vector>

using namespace ftxui;

namespace {

std::string money(double value) { return "$" + std::to_string(value); }

std::vector<std::string> load_account_items(sqlite3 *db) {
  std::vector<std::string> items;

  for (const auto &a : account_db::get_all(db)) {
    items.push_back(a.name + "  " + a.currency + "  " +
                    money(a.initial_balance));
  }

  if (items.empty()) {
    items.push_back("(no accounts)");
  }

  return items;
}

std::vector<std::string> load_transaction_items(sqlite3 *db) {
  std::vector<std::string> items;

  for (const auto &t : transaction_db::get_all(db)) {
    items.push_back(t.date + "  " + t.category + "  " + money(t.total_amount));
  }

  if (items.empty()) {
    items.push_back("(no transactions)");
  }

  return items;
}

std::vector<std::string> load_recurring_items(sqlite3 *db) {
  std::vector<std::string> items;

  for (const auto &r : recurring_transaction_db::get_all(db)) {
    items.push_back(r.name + "  " + r.category + "  " + money(r.amount));
  }

  if (items.empty()) {
    items.push_back("(no recurring transactions)");
  }

  return items;
}

enum class Page : int {
    Main = 0,
    Accounts = 1,
    Transactions = 2,
    RecurringTransactions = 3,
    AddAccount = 4,
};

int page_index(Page page) {
    return static_cast<int>(page);
}

} // namespace

TuiApp::TuiApp(sqlite3 *db): db_(db) {}

void TuiApp::run() {
  using namespace ftxui;

  auto screen = ScreenInteractive::TerminalOutput();
  int current_page = page_index(Page::Main);
  int main_selected = 0;
  int account_selected = 0;
  int transaction_selected = 0;
  int recurring_selected = 0;
  std::string new_account_name;
  std::string new_account_currency = "USD";
  std::string new_account_balance;

  std::vector<std::string> main_items = {
      "Accounts",
      "Transactions",
      "Recurring Transactions",
  };
  std::vector<std::string> account_items;
  std::vector<std::string> transaction_items;
  std::vector<std::string> recurring_items;

  auto refresh_accounts = [&] {
    account_items = load_account_items(db_);
    account_selected = 0;
  };

  auto refresh_transactions = [&] {
    transaction_items = load_transaction_items(db_);
    transaction_selected = 0;
  };

  auto refresh_recurring = [&] {
    recurring_items = load_recurring_items(db_);
    recurring_selected = 0;
  };

  auto go_to = [&](Page page) {
    current_page = page_index(page);
  };

  auto go_back = [&] {
    go_to(Page::Main);
  };

  auto main_menu = Menu(&main_items, &main_selected);
  auto accounts_menu =
      Menu(&account_items, &account_selected);
  auto transactions_menu =
      Menu(&transaction_items, &transaction_selected);
  auto recurring_menu =
      Menu(&recurring_items, &recurring_selected);
  auto add_account_name_input =
      Input(&new_account_name, "Account name");
  auto add_account_currency_input =
      Input(&new_account_currency, "Currency");
  auto add_account_balance_input =
      Input(&new_account_balance, "Initial balance");

  auto open_button = Button("Open", [&] {
    switch (main_selected) {
    case 0:
      refresh_accounts();
      go_to(Page::Accounts);
      break;
    case 1:
      refresh_transactions();
      go_to(Page::Transactions);
      break;
    case 2:
      refresh_recurring();
      go_to(Page::RecurringTransactions);
      break;
    default:
      break;
    }
  });

  auto quit_button = Button("Quit", screen.ExitLoopClosure());
  auto accounts_back_button = Button("Back", go_back);
  auto accounts_add_button = Button("Add", [&] {
      go_to(Page::AddAccount);
  });



  auto accounts_edit_button = Button("Edit", [] {
    // TODO: edit selected account
  });
  auto accounts_delete_button = Button("Delete", [] {
    // TODO: delete selected account
  });
  auto add_account_save_button = Button("Save", [&] {
    // TODO: validate and save account
  });
  auto add_account_cancel_button = Button("Cancel", [&] {
    new_account_name.clear();
    new_account_currency = "USD";
    new_account_balance.clear();
    go_to(Page::Accounts);
  });
  auto transactions_back_button = Button("Back", go_back);
  auto recurring_back_button = Button("Back", go_back);
  auto main_page = Container::Vertical({
      main_menu,
      open_button,
      quit_button,
  });
  auto accounts_actions = Container::Horizontal({
      accounts_add_button,
      accounts_edit_button,
      accounts_delete_button,
      accounts_back_button,

  });
  auto accounts_page = Container::Vertical({
      accounts_menu,
      accounts_actions,
  });
  auto add_account_buttons = Container::Horizontal({
      add_account_save_button,
      add_account_cancel_button,
  });

  auto add_account_page = Container::Vertical({
      add_account_name_input,
      add_account_currency_input,
      add_account_balance_input,
      add_account_buttons,
  });
  auto transactions_page = Container::Vertical({
      transactions_menu,
      transactions_back_button,
  });

  auto recurring_page = Container::Vertical({
      recurring_menu,
      recurring_back_button,
  });

  auto root = Container::Tab(
      {
          main_page,
          accounts_page,
          transactions_page,
          recurring_page,
          add_account_page,
      },
      &current_page
  );

  auto renderer = Renderer(root, [&]() -> Element {
    switch (static_cast<Page>(current_page)) {
    case Page::Main:
      return vbox({
                 text("AntTrunk") | bold | center,
                 separator(),
                 main_menu->Render() | flex,
                 separator(),
                 hbox({
                     open_button->Render(),
                     text("  "),
                     quit_button->Render(),
                 }) | center,
             }) |
             border |
             size(WIDTH, GREATER_THAN, 45);
    case Page::Accounts:
      return vbox({
                text("Accounts") | bold | center,
                separator(),
                accounts_menu->Render() | flex,
                separator(),
                hbox({
                    accounts_add_button->Render(),
                    text("  "),
                    accounts_edit_button->Render(),
                    text("  "),
                    accounts_delete_button->Render(),
                    text("  "),
                    accounts_back_button->Render(),
                }) | center,
             }) |
             border |
             size(WIDTH, GREATER_THAN, 45);
    case Page::Transactions:
      return vbox({
                 text("Transactions") | bold | center,
                 separator(),
                 transactions_menu->Render() | flex,
                 separator(),
                 transactions_back_button->Render() | center,
             }) |
             border |
             size(WIDTH, GREATER_THAN, 45);
    case Page::RecurringTransactions:
      return vbox({
                 text("Recurring Transactions") | bold | center,
                 separator(),
                 recurring_menu->Render() | flex,
                 separator(),
                 recurring_back_button->Render() | center,
             }) |
             border |
             size(WIDTH, GREATER_THAN, 45);
    case Page::AddAccount:
        return vbox({
                text("Add Account") | bold | center,
                separator(),
                text("Account form will be added here.") | center,
                separator(),
                add_account_cancel_button->Render() | center,
            }) |
            border |
            size(WIDTH, GREATER_THAN, 45);
    }
    return text("Unknown page");
  });

  auto app = CatchEvent(renderer, [&](Event event) {
    if (event == Event::Character('q')) {
      screen.Exit();
      return true;
    }
    if (event == Event::Escape &&
        current_page != page_index(Page::Main)) {
      go_back();
      return true;
    }
    if (event == Event::Return &&
        current_page == page_index(Page::Main)) {
      switch (main_selected) {
      case 0:
        refresh_accounts();
        go_to(Page::Accounts);
        break;
      case 1:
        refresh_transactions();
        go_to(Page::Transactions);
        break;
      case 2:
        refresh_recurring();
        go_to(Page::RecurringTransactions);
        break;
      default:
        break;
      }
      return true;
    }
    return false;
  });
  screen.Loop(app);
}
