
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "transactions.csv"
#define MAX_TXNS  1000
#define MAX_ACC   64
#define MAX_TYPE  32
#define MAX_DATE  32

#define OK  1
#define ERR 0

char   accounts[MAX_TXNS][MAX_ACC];
char   types   [MAX_TXNS][MAX_TYPE];
double amounts [MAX_TXNS];
char   dates   [MAX_TXNS][MAX_DATE];
int    txn_count = 0;

//helpers prototypes
void trim_newline(char *s);
void to_lower_str(char *s);

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

//impliment
void trim_newline(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[n-1] = '\0';
        n--;
    }
}

void to_lower_str(char *s) {
    for (; s && *s; ++s) *s = (char)tolower((unsigned char)*s);
}

int ensure_csv_has_header(void) {
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) {
        f = fopen(FILE_NAME, "w");
        if (!f) { perror("create csv"); return 0; }
        fprintf(f, "AccountNumber,TransactionType,Amount,TransactionDate\n");
        fclose(f);
    } else {
        fclose(f);
    }
    return 1;
}

int load_csv() {
    txn_count = 0;
    FILE *f = fopen(FILE_NAME, "r");
    if (!f) { perror("open csv"); return 0; }

    char line[512];

    if (!fgets(line, sizeof(line), f)) { fclose(f); return 1; }

    while (fgets(line, sizeof(line), f)) {
        if (txn_count >= MAX_TXNS) { printf("เต็มความจุ array\n"); break; }

        char *token;
        token = strtok(line, ",");               if (!token) continue;
        strncpy(accounts[txn_count], token, MAX_ACC-1);
        accounts[txn_count][MAX_ACC-1] = '\0';

        token = strtok(NULL, ",");               if (!token) continue;
        strncpy(types[txn_count], token, MAX_TYPE-1);
        types[txn_count][MAX_TYPE-1] = '\0';

        token = strtok(NULL, ",");               if (!token) continue;
        amounts[txn_count] = strtod(token, NULL);

        token = strtok(NULL, ",");               if (!token) continue;
        strncpy(dates[txn_count], token, MAX_DATE-1);
        dates[txn_count][MAX_DATE-1] = '\0';
        trim_newline(dates[txn_count]);

        txn_count++;
    }

    fclose(f);
    return 1;
}

int list_all() {

}

int add() {

}

int search() {

}

int update() {

}

int delete_by_account() {

}

int save_csv() {

}

int displaymenu() {

}

int main() {
    if (!ensure_csv_has_header()) {
        printf("Failed to ensure CSV header.\n");
        return 1;
    }

    if (!load_csv()) {
        printf("Failed to load CSV.\n");
        return 1;
    }

    printf("Loaded %d transactions:\n", txn_count);
    for (int i = 0; i < txn_count; ++i) {
        printf("%s | %s | %.2f | %s\n", accounts[i], types[i], amounts[i], dates[i]);
    }

    return 0;
}

