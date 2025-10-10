#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 256

typedef struct {
    char *lhs;
    char *rhs;
} Rule;

// Правила T
Rule T_rules[] = {
    {"aaaa", "ab"},
    {"abbb", "bba"},
    {"babb", "bb"},
    {"aabb", "aaba"},
    {"bbbaa", "bb"},
    {"aaabab", "baabb"},
    {"baabb", "aabab"},
    {"baabab", "bab"},
    {"bbabab", "bb"},
    {"baabaab", "a"},
    {"bab", "baaaab"}
};
int T_rules_count = 11;

// Правила T'
Rule Tnew_rules[] = {
    {"bb", "a"},
    {"aa", "a"},
    {"ba", "a"},
    {"ab", "a"}
};
int Tnew_rules_count = 4;

char ALPHABET[] = {'a', 'b'};
int ALPHABET_SIZE = 2;

// Найти все вхождения lhs в s и выбрать одно случайное для замены
int apply_one_rewrite(char *s, char *result) {
    int positions[1000][2]; // [i, rule_index]
    int count = 0;

    int len_s = strlen(s);
    for (int r = 0; r < T_rules_count; r++) {
        char *lhs = T_rules[r].lhs;
        int lhs_len = strlen(lhs);
        for (int i = 0; i <= len_s - lhs_len; i++) {
            if (strncmp(&s[i], lhs, lhs_len) == 0) {
                positions[count][0] = i; // позиция в слове
                positions[count][1] = r; // номер правила
                count++;
            }
        }
    }

    if (count == 0) {
        strcpy(result, s);
        return 0;
    }

    int choice = rand() % count;
    int i = positions[choice][0];
    int r = positions[choice][1];
    int lhs_len = strlen(T_rules[r].lhs);
    char *rhs = T_rules[r].rhs;
    
    // новая строка
    strncpy(result, s, i);
    result[i] = '\0';
    strcat(result, rhs);
    strcat(result, s + i + lhs_len);
    return 1;
}

// Случайные шаги по T
void random_T_steps(char *s, char *result, int max_steps) {
    char cur[MAX_LEN];
    char tmp[MAX_LEN];
    strcpy(cur, s);

    for (int step = 0; step < max_steps; step++) {
        if (!apply_one_rewrite(cur, tmp)) {
            break;
        }
        strcpy(cur, tmp);
    }
    strcpy(result, cur);
}

// Полная редукция по T'
void normalize_Tnew(char *s, char *result) {
    char cur[MAX_LEN];
    char tmp[MAX_LEN];
    strcpy(cur, s);

    int applied;
    do {
        applied = 0;
        for (int r = 0; r < Tnew_rules_count; r++) {
            char *lhs = Tnew_rules[r].lhs;
            char *rhs = Tnew_rules[r].rhs;
            char *pos = strstr(cur, lhs); // первое вхождение
            if (pos != NULL) {
                int idx = pos - cur;
                strncpy(tmp, cur, idx);
                tmp[idx] = '\0';
                strcat(tmp, rhs);
                strcat(tmp, cur + idx + strlen(lhs));
                strcpy(cur, tmp);
                applied = 1;
                break;
            }
        }
    } while (applied);
    strcpy(result, cur);
}

// Генерация случайного слова длиной L
void random_word(char *s, int L) {
    for (int i = 0; i < L; i++) {
        s[i] = ALPHABET[rand() % ALPHABET_SIZE];
    }
    s[L] = '\0';
}

void fuzz_test(int num_tests, int max_word_len, int max_steps) {
    char s[MAX_LEN], s_prime[MAX_LEN];
    char nf_s[MAX_LEN], nf_sprime[MAX_LEN];
    int failures = 0;

    for (int t = 0; t < num_tests; t++) {
        int L = rand() % max_word_len + 1;
        random_word(s, L);
        random_T_steps(s, s_prime, max_steps);
        normalize_Tnew(s, nf_s);
        normalize_Tnew(s_prime, nf_sprime);
        if (strcmp(nf_s, nf_sprime) != 0) {
            failures++;
            if (failures <= 20) {
                printf(" ! Failure:\n%s -> %s\n%s != %s\n\n", s, s_prime, nf_s, nf_sprime);
            }
        }
    }

    if (failures == 0) {
        printf("За %d тестов контрпримеров не найдено.\n", num_tests);
    } else {
        printf("Всего найдено %d контрпримеров.\n", failures);
    }
}

int main() {
    srand(time(NULL));
    fuzz_test(10000, 100, 30);
    return 0;
}
