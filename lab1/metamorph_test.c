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

int count_substring(const char *s, const char *sub) {
    int count = 0;
    size_t len_s = strlen(s);
    size_t len_sub = strlen(sub);
    if (len_sub == 0 || len_sub > len_s)
        return 0;
    for (size_t i = 0; i <= len_s - len_sub; i++) {
        if (strncmp(s + i, sub, len_sub) == 0)
            count++;
    }
    return count;
}

/* Ψ(w) = |w| + 2*count('b') */
int psi_of(const char *s) {
    int len = (int)strlen(s);
    int count_b = count_substring(s, "b");
    return len + 2 * count_b;
}

/* 
   I(w) = -23*a -11*b +23*aa +11*ab +11*ba -1*bb
           +12*aba +12*abb +12*bba +12*bbb
*/
int invariant_I(const char *s) {
    int a   = count_substring(s, "a");
    int b   = count_substring(s, "b");
    int aa  = count_substring(s, "aa");
    int ab  = count_substring(s, "ab");
    int ba  = count_substring(s, "ba");
    int bb  = count_substring(s, "bb");
    int aba = count_substring(s, "aba");
    int abb = count_substring(s, "abb");
    int bba = count_substring(s, "bba");
    int bbb = count_substring(s, "bbb");

    int val = -23*a -11*b +23*aa +11*ab +11*ba -1*bb
              +12*aba +12*abb +12*bba +12*bbb;
    return val;
}

/* 
   phi(w) = композиция функций от каждой буквы в строке phi(w0) o phi(w1) o ...
*/
#define PHI_N 4
static const int PHI_A[PHI_N] = {1, 1, 1, 3};
static const int PHI_B[PHI_N] = {1, 1, 0, 3};

/*fog = [f[g[0]], f[g[1]], f[g[2]], f[g[3]]]*/
static void compose_transform(const int f[], const int g[], int out[], int n) {
    for (int i = 0; i < n; ++i) out[i] = f[g[i]];
}

static void phi_of_word(const char *s, int out[]) {
    int cur[PHI_N], tmp[PHI_N];
    for (int i = 0; i < PHI_N; ++i) cur[i] = i;
    for (int i = 0; s[i]; ++i) {
        if (s[i] == 'a') compose_transform(PHI_A, cur, tmp, PHI_N);
        else compose_transform(PHI_B, cur, tmp, PHI_N);
        for (int j = 0; j < PHI_N; ++j) cur[j] = tmp[j];
    }
    for (int j = 0; j < PHI_N; ++j) out[j] = cur[j];
}

static int phi_equal(const int x[], const int y[]) {
    for (int i = 0; i < PHI_N; ++i)
        if (x[i] != y[i]) return 0;
    return 1;
}

static void print_phi(const int x[]) {
    printf("[");
    for (int i = 0; i < PHI_N; ++i) {
        printf("%d", x[i]);
        if (i < PHI_N-1) printf(", ");
    }
    printf("]");
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

int run_single_test(int max_word_len, int max_steps) {
    char s[MAX_LEN], s_new[MAX_LEN];
    int L = (rand() % max_word_len) + 1;
    random_word(s, L);

    int len_prev = (int)strlen(s);
    int psi_prev = psi_of(s);
    int I_prev  = invariant_I(s);

    /* пошаговое применение T' */
    for (int step = 1; step <= max_steps; ++step) {
        int applied = apply_one_rewrite_Tnew(s, s_new);
        if (!applied) {
            return 0;
        }

        int len_new = (int)strlen(s_new);
        int psi_new = psi_of(s_new);
        int I_new  = invariant_I(s_new);

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
        
        /* Инвариант 4: I - лин комбинация подстрок длины <= 3*/
        if (I_new != I_prev) {
            printf("[FAIL] Step %d: Invariant I violated.\n", step);
            printf("  before (I=%d): \"%s\"\n", I_prev, s);
            printf("  after  (I=%d): \"%s\"\n", I_new, s_new);
            return 1;
        }
        
        /* Инвариант 5:  композиция функций от каждой буквы в строке phi(w0) o phi(w1) o ...*/
        int phi_before[PHI_N], phi_after[PHI_N];
        phi_of_word(s, phi_before);
        phi_of_word(s_new, phi_after);
        if (!phi_equal(phi_before, phi_after)) {
            printf("[FAIL] phi-invariant violated.\n");
            printf("  before phi = "); print_phi(phi_before); printf(", word: \"%s\"\n", s);
            printf("  after  phi = "); print_phi(phi_after);  printf(", word: \"%s\"\n", s_new);
            return 1;
        }
        
        strcpy(s, s_new);
        len_prev = len_new;
        psi_prev = psi_new;
        I_prev  = I_new;
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
