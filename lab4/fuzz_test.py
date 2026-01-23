import time
import random
import sys
import matplotlib.pyplot as plt
import numpy as np

# ---------------------
# Cлова, которые 100% принадлежат языку
# ---------------------
def build_wk(k):
    '''
    Возвращает строку w_k, соответствующую конструкциям:
        w_0 = ab
        w_k = ab + b + w_{k-1} + w_{k-1} + a  для k>=1

    S.attr = 2^{2^k}
    '''
    if k == 0:
        return "ab"
    prev = build_wk(k-1)
    return "ab" + "b" + prev + prev + "a"

def length_wk(k):
    return 6 * (2**k) - 4

# ---------------------
# Парсеры
# ---------------------
sys.setrecursionlimit(10000)

def parse_naive(word):
    n = len(word)

    def parse_S(i):
        res = []
        # S -> abP
        if i+1 < n and word[i:i+2] == "ab":
            for (j, a) in parse_P(i+2):
                res.append((j, a))
        # S -> baP
        if i+1 < n and word[i:i+2] == "ba":
            for (j, a) in parse_P(i+2):
                res.append((j, a))
        # S -> ab
        if i+1 < n and word[i:i+2] == "ab":
            res.append((i+2, 2))
        # S -> ba
        if i+1 < n and word[i:i+2] == "ba":
            res.append((i+2, 3))
        return res

    def parse_P(i):
        res = []
        if i < n and word[i] == 'b':
            # P -> bTaP
            for (jT, aT) in parse_T(i+1):
                if jT < n and word[jT] == 'a':
                    for (jP, aP) in parse_P(jT+1):
                        # P0.a := P1.a
                        res.append((jP, aP))
            # P -> bTa
            for (jT, aT) in parse_T(i+1):
                if jT < n and word[jT] == 'a':
                    res.append((jT+1, aT))
        return res

    def parse_T(i):
        res = []
        # T -> SS
        for (j1, a1) in parse_S(i):
            for (j2, a2) in parse_S(j1):
                # S1.a == S2.a
                if a1 == a2:
                    res.append((j2, a1 * a2))
        # T -> bT
        if i < n and word[i] == 'b':
            for (j, a) in parse_T(i+1):
                res.append((j, a + 1))
        return res

    results = parse_S(0)
    accepts = any((pos == n) for (pos, _) in results)
    attrs = [a for (pos, a) in results if pos == n]
    return accepts, attrs

def parse_optimal(word):
    n = len(word)
    memo = {}  # key = (sym, pos) -> list of (next_pos, attr)

    def parse_S(i):
        key = ('S', i)
        if key in memo:
            return memo[key]
        res = []
        # S -> abP
        if i+1 < n and word[i:i+2] == "ab":
            for (j, a) in parse_P(i+2):
                res.append((j, a))
        # S -> baP
        if i+1 < n and word[i:i+2] == "ba":
            for (j, a) in parse_P(i+2):
                res.append((j, a))
        # S -> ab
        if i+1 < n and word[i:i+2] == "ab":
            res.append((i+2, 2))
        # S -> ba
        if i+1 < n and word[i:i+2] == "ba":
            res.append((i+2, 3))
        memo[key] = res
        return res

    def parse_P(i):
        key = ('P', i)
        if key in memo:
            return memo[key]
        res = []
        if i < n and word[i] == 'b':
            # P -> bTaP
            for (jT, aT) in parse_T(i+1):
                if jT < n and word[jT] == 'a':
                    for (jP, aP) in parse_P(jT+1):
                        res.append((jP, aP))
            # P -> bTa
            for (jT, aT) in parse_T(i+1):
                if jT < n and word[jT] == 'a':
                    res.append((jT+1, aT))
        memo[key] = res
        return res

    def parse_T(i):
        key = ('T', i)
        if key in memo:
            return memo[key]
        res = []
        # T -> SS
        for (j1, a1) in parse_S(i):
            for (j2, a2) in parse_S(j1):
                if a1 == a2:
                    res.append((j2, a1 * a2))
        # T -> bT
        if i < n and word[i] == 'b':
            for (j, a) in parse_T(i+1):
                res.append((j, a + 1))
        memo[key] = res
        return res

    results = parse_S(0)
    accepts = any((pos == n) for (pos, _) in results)
    attrs = [a for (pos, a) in results if pos == n]
    return accepts, attrs

# --------------------------
# Генераторы пулов
# --------------------------
def generate_in_language(k_values, per_k=10):
    samples = []
    for k in k_values:
        wk = build_wk(k)
        for _ in range(per_k):
            samples.append(wk)
    return samples

def generate_out_of_language(k_values, per_k=10, mutation_prob=0.15):
    samples = []
    for k in k_values:
        wk = build_wk(k)
        for _ in range(per_k):
            s = list(wk)
            for i in range(len(s)):
                if random.random() < mutation_prob:
                    op = random.choice(['flip', 'del', 'ins'])
                    if op == 'flip':
                        s[i] = 'a' if s[i] == 'b' else 'b'
                    elif op == 'del':
                        s[i] = ''  # удалим
                    elif op == 'ins':
                        s[i] = s[i] + random.choice(['a','b'])
            s = ''.join(s)
            if len(s) == 0:
                s = random.choice(['a','b'])*5
            samples.append(s)
    return samples

# ----------------------
# Фазз-тест
# ----------------------
def time_parsing(func, words):
    times = []
    accepts = []
    for w in words:
        t0 = time.perf_counter()
        ok, _ = func(w)
        t1 = time.perf_counter()
        times.append(t1-t0)
        accepts.append(ok)
    return float(np.mean(times)), times, accepts

def fuzz_test(k_list, per_k=12):
    results = {
        'k': [],
        'len': [],
        'naive_in_mean': [], 'optimal_in_mean': [],
        'naive_out_mean': [], 'optimal_out_mean': []
    }

    for k in k_list:
        wk = build_wk(k)
        L = len(wk)
        in_pool = generate_in_language([k], per_k)
        out_pool = generate_out_of_language([k], per_k, 0.2)

        naive_in_mean, _, _ = time_parsing(parse_naive, in_pool)
        optimal_in_mean, _, _ = time_parsing(parse_optimal, in_pool)

        naive_out_mean, _, _ = time_parsing(parse_naive, out_pool)
        optimal_out_mean, _, _ = time_parsing(parse_optimal, out_pool)

        for i, w in enumerate(in_pool):
            n_ok, _ = parse_naive(w)
            o_ok, _ = parse_optimal(w)
            if n_ok != o_ok:
                print("ОШИБКА: слово из языка ", w, ", k=", k, " idx", i, "наивный парсер: ", n_ok, "оптим парсер: ", o_ok)
        for i, w in enumerate(out_pool):
            n_ok, _ = parse_naive(w)
            o_ok, _ = parse_optimal(w)
            if n_ok != o_ok:
                print("ОШИБКА: слово НЕ из языка ", w, ", k=", k, "out idx", i, "наивный парсер: ", n_ok, "оптим парсер: ", o_ok)

        results['k'].append(k)
        results['len'].append(L)
        results['naive_in_mean'].append(naive_in_mean)
        results['optimal_in_mean'].append(optimal_in_mean)
        results['naive_out_mean'].append(naive_out_mean)
        results['optimal_out_mean'].append(optimal_out_mean)

        print(f"k={k}, len={L}: naive_in={naive_in_mean:.6f}, optimal_in={optimal_in_mean:.6f}, naive_out={naive_out_mean:.6f}, optimal_out={optimal_out_mean:.6f}")

    return results

def plot_results(res):
    xs = res['len']
    plt.figure(figsize=(10, 5))

    # Из языка
    plt.subplot(1,2,1)
    plt.plot(xs, res['naive_in_mean'], 'o-', label='Наивный (in-lang)')
    plt.plot(xs, res['optimal_in_mean'], 's-', label='Оптимизированный (in-lang)')
    plt.xlabel('Длина слова')
    plt.ylabel('Среднее время парсинга (с)')
    plt.title('Слова из языка')
    plt.legend()
    plt.grid(True)

    # Не из языка
    plt.subplot(1,2,2)
    plt.plot(xs, res['naive_out_mean'], 'o-', label='Наивный (out-lang)')
    plt.plot(xs, res['optimal_out_mean'], 's-', label='Оптимизированный (out-lang)')
    plt.xlabel('Длина слова')
    plt.ylabel('Среднее время парсинга (с)')
    plt.title('Слова не из языка')
    plt.legend()
    plt.grid(True)

    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    random.seed(12345)
    # длины: 2,8,20,44,92,188,...
    k_values = [0,1,2,3,4,5,6,7,8,9]
    results = fuzz_test(k_values, 12)
    plot_results(results)
    
    print(parse_naive("abbbabbbbbbbbababaabbbbabaaa"), 
          parse_optimal("abbbabbbbbbbbababaabbbbabaaa")) #101
    
    print(parse_naive("abbabbbbbbbbbbbbbbbbbbbbbbbababaabbabbbababaabbbababaaa"), 
          parse_optimal("bababbbbbbbbbbbbbbbbbbbbbbbababaabbabbbababaabbbababaaa")) #25*25