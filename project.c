#include <stdio.h>
#include <string.h>

/* ===== CONFIG ===== */
#define FILE_NAME "transactions.csv"

#define MAX_TXNS  1000
#define MAX_ACC   64
#define MAX_TYPE  32
#define MAX_DATE  32

#define OK  1
#define ERR 0

/* ===== GLOBAL PARALLEL ARRAYS (no struct) ===== */
char   accounts[MAX_TXNS][MAX_ACC];
char   types   [MAX_TXNS][MAX_TYPE];
double amounts [MAX_TXNS];
char   dates   [MAX_TXNS][MAX_DATE];
int    txn_count = 0;

//prototypes
int ensure_csv_has_header(void);
int load_csv(void);
int save_csv(void);
int list_all(void);
int add(void);
int search(void);
int update(void);
int delete_by_account(void);
int displaymenu(void);
int pause_enter(void);

//helper
static void consume_line(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { /* flush */ }
}

//implementation
int ensure_csv_has_header(void) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        f = fopen(FILE_NAME, "w");
        if (!f) { printf("Cannot create CSV.\n"); return ERR; }
        fprintf(f, "AccountNumber,TransactionType,Amount,TransactionDate\n");
        fclose(f);
    } else {
        fclose(f);
    }
    return OK;
}

int load_csv(void) {
    txn_count = 0;
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) { printf("Cannot open CSV for reading.\n"); return ERR; }

    char line[256];
    if (!fgets(line, sizeof(line), fp)) { fclose(fp); return OK; }

    while (fgets(line, sizeof(line), fp)) {
        if (txn_count >= MAX_TXNS) { printf("Array full; some rows ignored.\n"); break; }
        if (sscanf(line, " %63[^,],%31[^,],%lf,%31s",
                   accounts[txn_count], types[txn_count],
                   &amounts[txn_count], dates[txn_count]) == 4) {
            txn_count++;
        }
    }
    fclose(fp);
    return OK;
}

int save_csv(void) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) { printf("Cannot open CSV for writing.\n"); return ERR; }

    fprintf(fp, "AccountNumber,TransactionType,Amount,TransactionDate\n");
    for (int i = 0; i < txn_count; ++i) {
        fprintf(fp, "%s,%s,%.2f,%s\n",
                accounts[i], types[i], amounts[i], dates[i]);
    }
    fclose(fp);
    printf("Saved.\n");
    return OK;
}

int list_all(void) {
    if (txn_count == 0) { printf("(no data)\n"); return OK; }
    for (int i = 0; i < txn_count; ++i) {
        printf("%3d) %s | %s | %.2f | %s\n",
               i+1, accounts[i], types[i], amounts[i], dates[i]);
    }
    return OK;
}

int add(void) {
    if (txn_count >= MAX_TXNS) { printf("Array full.\n"); return ERR; }

    printf("Account: "); if (scanf("%63s", accounts[txn_count]) != 1) { consume_line(); return ERR; }
    consume_line();
    printf("Type   : "); if (scanf("%31s",  types[txn_count])    != 1) { consume_line(); return ERR; }
    consume_line();
    printf("Amount : "); if (scanf("%lf",   &amounts[txn_count]) != 1) { consume_line(); printf("Invalid number.\n"); return ERR; }
    consume_line();
    if (amounts[txn_count] < 0) { printf("Amount must be non-negative.\n"); return ERR; }
    printf("Date(YYYY-MM-DD): ");
    if (scanf("%31s", dates[txn_count]) != 1) { consume_line(); return ERR; }
    consume_line();

    txn_count++;
    printf("Added in memory (remember to Save).\n");
    return OK;
}

/* Search by Account or Type (exact match) */
int search(void) {
    int choice;
    char key[64];

    printf("Search by (1) Account  (2) Type : ");
    if (scanf("%d", &choice) != 1) { consume_line(); return ERR; }
    consume_line();

    if (choice == 1) {
        printf("Account: "); if (scanf("%63s", key) != 1) { consume_line(); return ERR; }
        consume_line();
        int found = 0;
        for (int i = 0; i < txn_count; ++i) {
            if (strcmp(accounts[i], key) == 0) {
                printf("%3d) %s | %s | %.2f | %s\n", i+1, accounts[i], types[i], amounts[i], dates[i]);
                found++;
            }
        }
        if (!found) printf("Not found.\n");
    } else if (choice == 2) {
        printf("Type: "); if (scanf("%63s", key) != 1) { consume_line(); return ERR; }
        consume_line();
        int found = 0;
        for (int i = 0; i < txn_count; ++i) {
            if (strcmp(types[i], key) == 0) {
                printf("%3d) %s | %s | %.2f | %s\n", i+1, accounts[i], types[i], amounts[i], dates[i]);
                found++;
            }
        }
        if (!found) printf("Not found.\n");
    } else {
        printf("Invalid choice.\n");
        return ERR;
    }
    return OK;
}

int update(void) {
    char acc[MAX_ACC], dt[MAX_DATE];
    double new_amount;

    printf("Account: "); if (scanf("%63s", acc) != 1) { consume_line(); return ERR; }
    consume_line();
    printf("Date(YYYY-MM-DD): "); if (scanf("%31s", dt) != 1) { consume_line(); return ERR; }
    consume_line();
    printf("New Amount: "); if (scanf("%lf", &new_amount) != 1) { consume_line(); printf("Invalid number.\n"); return ERR; }
    consume_line();
    if (new_amount < 0) { printf("Amount must be non-negative.\n"); return ERR; }

    for (int i = 0; i < txn_count; ++i) {
        if (strcmp(accounts[i], acc) == 0 && strcmp(dates[i], dt) == 0) {
            amounts[i] = new_amount;
            printf("Updated at row %d\n", i+1);
            return OK;
        }
    }
    printf("Target row not found.\n");
    return ERR;
}

int delete_by_account(void) {
    char acc[MAX_ACC];
    printf("Account to delete: ");
    if (scanf("%63s", acc) != 1) { consume_line(); return ERR; }
    consume_line();

    int write = 0, deleted = 0;
    for (int read = 0; read < txn_count; ++read) {
        if (strcmp(accounts[read], acc) == 0) { deleted++; continue; }
        if (write != read) {
            strcpy(accounts[write], accounts[read]);
            strcpy(types[write],    types[read]);
            amounts[write] = amounts[read];
            strcpy(dates[write],    dates[read]);
        }
        write++;
    }
    txn_count = write;

    if (deleted == 0) { printf("No rows deleted (account not found).\n"); return ERR; }
    printf("Deleted %d row(s).\n", deleted);
    return OK;
}

int displaymenu(void) {
    int c;
    printf("\n==== MENU ====\n");
    printf("1) List All\n");
    printf("2) Add\n");
    printf("3) Search\n");
    printf("4) Update Amount\n");
    printf("5) Delete by Account\n");
    printf("6) Save CSV\n");
    printf("0) Exit\n");
    printf("Choose: ");
    if (scanf("%d", &c) != 1) { consume_line(); return -1; }
    consume_line();
    return c;
}

int pause_enter(void) {
    printf("\nPress Enter to continue...");
    getchar();
    return OK;
}

int main(void) {
    if (ensure_csv_has_header() == ERR) {
        printf("Cannot prepare CSV.\n");
        return 1;
    }
    if (load_csv() == ERR) {
        printf("Cannot load CSV (will start empty).\n");
    }

    for (;;) {
        int c = displaymenu();
        if (c == 0) break;
        switch (c) {
            case 1: list_all();            break;
            case 2: add();                 break;
            case 3: search();              break;
            case 4: update();              break;
            case 5: delete_by_account();   break;
            case 6: save_csv();            break;
            default: printf("Invalid menu.\n"); break;
        }
        pause_enter();
    }

    printf("Save before exit? (y/n): ");
    int ch = getchar(); consume_line();
    if (ch == 'y' || ch == 'Y') save_csv();

    printf("Bye.\n");
    return 0;
}