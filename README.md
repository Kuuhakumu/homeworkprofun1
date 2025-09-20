# Transaction Manager

This C program manages bank transactions stored in a CSV file.


## How It Works

- Data is stored in arrays in memory.
- The program reads and writes to `transactions.csv`.
- Helper functions handle string processing and file setup.


## Functions

- `ensure_csv_has_header` — Ensures the CSV file exists and has a header
- `load_csv` — Loads transactions from the CSV file
- `list_all` — Lists all transactions
- `add` — Adds a new transaction
- `search` — Searches for transactions by account
- `update` — Updates a transaction
- `delete_by_account` — Deletes transactions by account
