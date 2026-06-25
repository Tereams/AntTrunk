
#include "finance_app.h"

int main() {
    FinanceApp app;
    if (!app.initialize()) {
            return 1;
        }

    app.run();
    return 0;
}
