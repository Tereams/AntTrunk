# Personal Finance Tracking System — Design Overview

## User Stories

### Inputs

1. **Expenses**

   * Record various types of expenses with categories:

     * *Fixed expenses*: rent, car loan, etc.
     * *Variable expenses*: groceries, daily spending, etc.

2. **Financial Plan**

   * Define a plan for future income.

3. **Account Management**

   * Maintain balances across multiple accounts and track them separately.

---

### Outputs

1. **Balance**

   * Show the current remaining balance per account.
   * Enable reconciliation with actual account balances.

2. **Budget**

   * Calculate how much can still be spent within a given period:

     * Monthly
     * Weekly
     * Daily

---

## Level 1 Extensions

1. **Wishlist**

   * Maintain a list of desired items.
   * After incorporating income plans, automatically estimate when each item can be afforded.

---

## Level 2 Extensions

1. **Stock Valuation**

   * Fetch stock prices and compute total portfolio value.

2. **Investment Assistant**

   * Provide insights or suggestions related to investments.

3. **Tax Assistant**

   * Estimate tax obligations based on current income.
   * Assist with validation during tax filing.

---

## System Design Considerations

1. **Data Security**

   * Sensitive financial data must be securely stored with encryption.

2. **Initial Version (MVP)**

   * Command-line interface only (no GUI).
   * Use files as the primary output format.
