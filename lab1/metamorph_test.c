#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 1024
#define MAX_OCCUR 10000

typedef struct {
    const char *lhs;
    const char *rhs;
} Rule;

/* Правила T' */
Rule Tnew_rules[] = {
    {"bb","a"},
    {"aa","a"},
    {"ba","a"},
    {"ab","a"}
};
int Tnew_rules_count = 4;

const char ALPHABET[] = "ab";
int ALPHABET_SIZE = 2;

int count_char(const char *s, char c) {
    int count = 0;
    for (int i = 0; s[i]; i++)
        if (s[i] == c)
            count++;
    return count;
}

/* Ψ(w) = |w| + 2*count('b') */
int psi_of(const char *s) {
    int len = (int)strlen(s);
    int count_b = count_char(s, 'b');
    return len + 2 * count_b;
}

/* Найти все вхождения LHS правил T' в строке s*/
int apply_one_rewrite_Tnew(const char *s, char *result) {
    /* positions: [idx, rule_index] */
    int positions[MAX_OCCUR][2];
    int pos_count = 0;
    int len_s = (int)strlen(s);

    for (int r = 0; r < Tnew_rules_count; ++r) {
        const char *lhs = Tnew_rules[r].lhs;
        int lhs_len = (int)strlen(lhs);
        if (lhs_len == 0 || lhs_len > len_s) continue;
        for (int i = 0; i <= len_s - lhs_len; ++i) {
            if (strncmp(s + i, lhs, lhs_len) == 0) {
                if (pos_count < MAX_OCCUR) {
                    positions[pos_count][0] = i;
                    positions[pos_count][1] = r;
                    pos_count++;
                } else {
                    break;
                }
            }
        }
    }

    if (pos_count == 0) {
        strcpy(result, s);
        return 0;
    }

    int choice = rand() % pos_count;
    int idx = positions[choice][0];
    int r = positions[choice][1];
    const char *lhs = Tnew_rules[r].lhs;
    const char *rhs = Tnew_rules[r].rhs;
    int lhs_len = (int)strlen(lhs);

    /* [до idx] + rhs + [после idx+lhs_len] */
    if (idx > 0) {
        strncpy(result, s, idx);
    }
    result[idx] = '\0';
    strcat(result, rhs);
    strcat(result, s + idx + lhs_len);
    return 1;
}

/* Генерация случайного слова длиной L*/
void random_word(char *out, int L) {
    for (int i = 0; i < L; ++i) {
        out[i] = ALPHABET[rand() % ALPHABET_SIZE];
    }
    out[L] = '\0';
}

/* Выполнение одного случайного теста:
   - сгенерировать слово длиной в [1..max_word_len]
   - выполнить до max_steps применений правил T' (выбирая случайное вхождение на каждом шаге)
   - после каждого шага проверять инварианты
   Возвращает 0 если все инварианты сохранялись, 1 если найдено нарушение (и печатает подробности).
*/
int run_single_test(int max_word_len, int max_steps) {
    char s[MAX_LEN], s_new[MAX_LEN];
    int L = (rand() % max_word_len) + 1;
    random_word(s, L);

    /* начальные значения */
    int len_prev = (int)strlen(s);
    int psi_prev = psi_of(s);

    /* (начальное слово не пустое) */
    if (len_prev == 0) {
        printf("[FAIL] Initial word empty (shouldn't happen): \"%s\"\n", s);
        return 1;
    }

    /* пошаговое применение T' */
    for (int step = 1; step <= max_steps; ++step) {
        int applied = apply_one_rewrite_Tnew(s, s_new);
        if (!applied) {
            return 0;
        }

        int len_new = (int)strlen(s_new);
        int psi_new = psi_of(s_new);


        /* Инвариант 1: невозможность получить пустое слово */
        if (len_new == 0) {
            printf("[FAIL] Step %d: empty word produced.\n", step);
            printf("  before (len=%d, psi=%d): \"%s\"\n", len_prev, psi_prev, s);
            printf("  after  (len=%d, psi=%d): \"\" (empty)\n", len_new, psi_new);
            return 1;
        }

        /* Инвариант 2: длина не увеличивается */
        if (len_new > len_prev) {
            printf("[FAIL] Step %d: length increased (was %d, became %d).\n", step, len_prev, len_new);
            printf("  before: \"%s\"\n  after : \"%s\"\n", s, s_new);
            return 1;
        }

        /* Инвариант 3: Psi монотонно строго убывает */
        if (psi_new > psi_prev) {
            printf("[FAIL] Step %d: Psi not decreased (was %d, became %d).\n", step, psi_prev, psi_new);
            printf("  before: \"%s\"\n  after : \"%s\"\n", s, s_new);
            return 1;
        }

        strcpy(s, s_new);
        len_prev = len_new;
        psi_prev = psi_new;
    }

    return 0;
}

/* Серия тестов и сводка */
void run_many_tests(int num_tests, int max_word_len, int max_steps) {
    int failures = 0;
    for (int t = 0; t < num_tests; ++t) {
        if (run_single_test(max_word_len, max_steps) != 0) {
            ++failures;
        }
    }
    if (failures == 0) {
        printf("All %d tests passed. No invariant violations detected.\n", num_tests);
    } else {
        printf("Total failures: %d out of %d tests.\n", failures, num_tests);
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    run_many_tests(10000, 100, 200);
    return 0;
}
