# AntTrunk Data Model List

## 1. Account

An `Account` represents a container of money, such as a checking account, savings account, credit card, cash account, or investment account.

Attributes:

- `id`: unique identifier of the account
- `name`: user-facing account name
- `bank_name`: name of the bank or institution
- `type`: account type, such as checking, savings, credit card, cash, investment, or other
- `currency`: currency code, such as USD
- `initial_balance`: starting balance recorded by the user
- `description`: optional natural language description

## 2. Transaction

A `Transaction` represents a real financial event that has already happened. It can be an expense, income, or transfer.

Attributes:

- `id`: unique identifier of the transaction
- `type`: transaction type, such as expense, income, or transfer
- `amount_before_tax`: amount before tax, mainly useful for expenses
- `tax_rate`: tax rate applied to the transaction, if any
- `total_amount`: final amount after tax
- `account_id`: source account related to the transaction
- `target_account_id`: target account, used for transfers
- `category`: transaction category, such as rent, grocery, salary, or subscription
- `date`: transaction date in YYYY-MM-DD format
- `description`: optional natural language description

## 3. RecurrenceRule

A `RecurrenceRule` describes how often a recurring transaction happens.

Attributes:

- `frequency`: recurrence frequency, such as daily, weekly, biweekly, monthly, or yearly
- `interval`: recurrence interval, such as every 1 month or every 2 weeks
- `day_of_month`: optional day of month, such as the 15th of every month
- `day_of_week`: optional weekday, such as every Friday

## 4. RecurringTransaction

A `RecurringTransaction` represents a scheduled or expected financial event that happens repeatedly. It can be reused for both scheduled bills and salary schedules.

Attributes:

- `id`: unique identifier of the recurring transaction
- `name`: user-facing name, such as rent, salary, car insurance, or subscription
- `type`: transaction type, usually expense or income
- `amount`: amount for each occurrence
- `account_id`: source or receiving account
- `target_account_id`: target account, used for recurring transfers
- `category`: category of the recurring transaction
- `recurrence_rule`: rule that describes when the transaction repeats
- `start_date`: date when the recurring transaction starts
- `end_date`: optional date when the recurring transaction ends
- `description`: optional natural language description
