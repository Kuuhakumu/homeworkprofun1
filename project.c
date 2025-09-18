
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
    printf("\n==== เมนู ====\n");
    printf("1) แสดงทั้งหมด\n");
    printf("2) เพิ่ม\n");
    printf("3) ค้นหา\n");
    printf("4) อัปเดตจำนวนเงิน\n");
    printf("5) ลบตามบัญชี\n");
    printf("6) บันทึกลงไฟล์\n");
    printf("0) ออก\n");
    printf("เลือก: ");
    char buf[16]; if (!fgets(buf, sizeof(buf), stdin)) return -1;
    return atoi(buf);
}

int main() {

}

