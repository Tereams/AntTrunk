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

---

## System Architecture (MVP Design)

### Command-Line Interface (CLI)

The system uses a command-line interface as the sole interaction layer in the initial version. All inputs and outputs are handled through CLI commands.

Example commands:

```bash
finance add expense --account checking --category groceries --amount 32.5 --date 2026-04-28
finance balance
finance budget --month 2026-04
finance accounts
```

This design allows rapid iteration on core logic without being constrained by UI development. A graphical interface can be added later as a separate layer that reuses the same core logic.

---

### Data Storage (Text-Based)

All data is stored in structured text files rather than a database. This ensures simplicity, transparency, and ease of debugging.

Proposed directory structure:

```text
data/
  accounts.yaml
  transactions.csv
  plans.yaml
  wishlist.yaml
```

* **transactions.csv**
  Stores all financial records (expenses and income):

  ```csv
  date,type,account,category,amount,note
  2026-04-28,expense,checking,groceries,32.50,Walmart
  2026-04-29,income,checking,salary,3000.00,Paycheck
  ```

* **accounts.yaml**
  Stores account definitions and initial balances:

  ```yaml
  accounts:
    checking:
      initial_balance: 5000
    savings:
      initial_balance: 10000
  ```

* **plans.yaml**
  Stores planned future income or expenses:

  ```yaml
  plans:
    - date: 2026-05-01
      type: income
      account: checking
      category: salary
      amount: 3000
  ```

* **wishlist.yaml**
  Stores desired items and related metadata (for future use).

---

### Stateless Design

The system is designed to be stateless. Each command execution follows the same pattern:

```text
Read files → Compute results → Output results and/or update files → Exit
```

There is no hidden runtime state, background service, or persistent in-memory storage.

**Design principle:**

> The file system is the single source of truth.
> The program is stateless: every command reads from files, computes results, writes changes if needed, and exits.

This approach improves reliability, debuggability, and portability, and enables easy integration with version control systems.

---

### Layered Architecture

The system follows a simple layered architecture:

```text
CLI Layer
   ↓
Command Handler
   ↓
Core Logic
   ↓
File Repository
   ↓
Text Files
```

* **CLI Layer**
  Parses user input from the command line.

* **Command Handler**
  Maps CLI commands to specific operations.

* **Core Logic**
  Implements business logic (e.g., balance calculation, budget tracking).

* **File Repository**
  Handles reading from and writing to structured files.

* **Text Files**
  Serve as the persistent data storage and the single source of truth.
