import re
import random
from typing import Dict, Set, Tuple, List

ALPHABET = ['a', 'b', 'c']

# -------------------------
# Регулярное выражение
# -------------------------
reg = r'(((baa)*)|((abb)*))(bb|cc)*(aa(ba|bab))*(a|b|c)a*b((a|bcc*)abc)*'

regex_prog = re.compile(reg)

def regex_accepts(word: str):
    return bool(regex_prog.fullmatch(word))

# -------------------------
# ДКА
# -------------------------
dfa_transitions: Dict[str, Dict[str, str]]
dfa_start: str
dfa_finals: Set[str]

dfa_start = "0"
dfa_finals = {"15","10","9","6","11","13","5","12","7","8","14"}

dfa_transitions = {
    "24": {"c": "17", "a": "34", "b": "13"},
    "19": {"c": "36", "a": "32", "b": "6"},
    "16": {"c": "23", "a": "27", "b": "18"},
    "13": {"a": "35", "b": "37"},
    "32": {"a": "32", "b": "13"},
    "4":  {"a": "30", "b": "5"},
    "30": {"a": "32", "b": "10"},
    "11": {"c": "13", "a": "25", "b": "37"},
    "2":  {"a": "22", "b": "13"},
    "38": {"b": "39"},
    "31": {"a": "32", "b": "11"},
    "28": {"a": "31", "b": "13"},
    "21": {"c": "23", "a": "27", "b": "9"},
    "3":  {"c": "24", "a": "4", "b": "18"},
    "36": {"c": "36", "a": "38"},
    "18": {"a": "32", "b": "6"},
    "25": {"c": "32", "a": "29", "b": "26"},
    "8":  {"c": "13", "a": "2", "b": "7"},
    "27": {"a": "30", "b": "13"},
    "26": {"c": "32", "a": "27", "b": "12"},
    "33": {"a": "34", "b": "13"},
    "15": {"c": "13", "a": "35", "b": "37"},
    "35": {"a": "38"},
    "39": {"c": "13"},
    "10": {"a": "25", "b": "37"},
    "9":  {"a": "2", "b": "7"},
    "29": {"a": "30", "b": "15"},
    "34": {"a": "32", "b": "15"},
    "5":  {"a": "35", "b": "3"},
    "12": {"a": "33", "b": "14"},
    "17": {"c": "24", "a": "29", "b": "18"},
    "23": {"c": "16", "a": "32", "b": "13"},
    "14": {"c": "36", "a": "35", "b": "37"},
    "1":  {"a": "21", "b": "13"},
    "20": {"a": "1", "b": "6"},
    "6":  {"c": "23", "a": "28", "b": "19"},
    "37": {"c": "36"},
    "22": {"c": "23", "a": "27", "b": "8"},
    "0":  {"c": "23", "a": "4", "b": "20"},
    "7":  {"c": "24", "a": "28", "b": "19"}
}

def simulate_dfa(trans: Dict[str, Dict[str, str]], 
                 start: str, finals: Set[str], 
                 word: str):

    state = start
    if state is None:
        return False
    for ch in word:
        if state not in trans:
            return False
        if ch not in trans[state]:
            return False
        state = trans[state][ch]
    return state in finals

# -------------------------
# НКА
# -------------------------
nfa_transitions: Dict[str, Dict[str, Set[str]]]
nfa_start: str
nfa_finals: Set[str]

nfa_start = "0"
nfa_finals = {"17"}

nfa_transitions = {
    "0": {
        "a": {"4", "10", "16"},
        "b": {"1", "7", "16"},
        "c": {"9", "16"}
    },
    "1": {"a": {"2"}},
    "2": {"a": {"3"}},
    "3": {
        "a": {"10", "16"},
        "b": {"1", "7", "16"},
        "c": {"9", "16"}
    },
    "4": {"b": {"5"}},
    "5": {"b": {"6"}},
    "6": {
        "a": {"4", "10", "16"},
        "b": {"7", "16"},
        "c": {"9", "16"}
    },
    "7": {"b": {"8"}},
    "8": {
        "a": {"10", "16"},
        "b": {"7", "16"},
        "c": {"9", "16"}
    },
    "9": {"c": {"8"}},
    "10": {"a": {"11"}},
    "11": {"b": {"12", "14"}},
    "12": {"a": {"13"}},
    "13": {"a": {"10", "16"}, "b": {"16"}, "c": {"16"}},
    "14": {"a": {"15"}},
    "15": {"b": {"13"}},
    "16": {"a": {"16"}, "b": {"17"}},
    "17": {"a": {"18"}, "b": {"19"}},
    "18": {"a": {"21"}},
    "19": {"c": {"20"}},
    "20": {"a": {"21"}, "c": {"20"}},
    "21": {"b": {"22"}},
    "22": {"c": {"17"}}
}

def simulate_nfa(trans: Dict[str, Dict[str, Set[str]]], 
                 start: str, finals: Set[str], 
                 word: str):
    
    if start is None:
        return False
    cur: Set[str] = {start}
    for ch in word:
        nxt: Set[str] = set()
        for s in cur:
            if s in trans and ch in trans[s]:
                nxt.update(trans[s][ch])
        cur = nxt
        if not cur:
            return False
    return any(s in finals for s in cur)

# -------------------------
# ПКА
# -------------------------

nfa1_trans={
    "0": {"a": {"5"}, "b": {"4"}, "c": {"1"}},
    "1": {"a": {"2"}, "b": {"4"}, "c": {"1"}},
    "2": {"a": {"2"}, "b": {"3", "6"}},
    "3": {"a": {"5"}, "b": {"3"}, "c": {"1"}},
    "4": {"a": {"5"}, "b": {"3"}, "c": {"1"}},
    "5": {"a": {"5"}, "b": {"3", "6"}, "c": {"1"}},
    "6": {"c": {"7"}},
    "7": {}
}
nfa1_start="0"
nfa1_finals={"3", "4", "7"}

nfa2_trans=nfa_transitions
nfa2_start=nfa_start
nfa2_finals=nfa_finals

def simulate_afa(nfa1_trans, nfa1_start, nfa1_finals,
                 nfa2_trans, nfa2_start, nfa2_finals,
                 word: str):
    
    cur1: Set[str] = {nfa1_start}
    cur2: Set[str] = {nfa2_start}

    for ch in word:
        nxt1: Set[str] = set()
        nxt2: Set[str] = set()

        for s in cur1:
            if s in nfa1_trans and ch in nfa1_trans[s]:
                nxt1.update(nfa1_trans[s][ch])
        for s in cur2:
            if s in nfa2_trans and ch in nfa2_trans[s]:
                nxt2.update(nfa2_trans[s][ch])

        cur1, cur2 = nxt1, nxt2
        if not cur1 or not cur2:
            return False

    return any(s in nfa1_finals for s in cur1) and any(s in nfa2_finals for s in cur2)


# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------


def random_word(max_len: int):
    length = random.randint(0, max_len)
    return ''.join(random.choice(ALPHABET) for _ in range(length))

def run_fuzz_tests(num_tests: int, max_len: int, seed: int):
    if seed is not None:
        random.seed(seed)

    mismatches: List[Tuple[str, bool, bool, bool, bool]] = []
    all_equal = 0

    for _ in range(num_tests):
        w = random_word(max_len)
        r = regex_accepts(w)
        d = simulate_dfa(dfa_transitions, dfa_start, dfa_finals, w)
        n = simulate_nfa(nfa_transitions, nfa_start, nfa_finals, w)
        a = simulate_afa(nfa1_trans, nfa1_start, nfa1_finals,                
                         nfa2_trans, nfa2_start, nfa2_finals,
                                w)
        if r == d == n == a:
            all_equal += 1
        else:
            mismatches.append((w, r, d, n, a))
    return mismatches, all_equal

# -------------------------------------------

NUM_TESTS = 10000
MAX_LEN = 80
SEED = 12345

random.seed(SEED)

if __name__ == '__main__':
    print('Запуск ...')

    mismatches, all_equal = run_fuzz_tests(NUM_TESTS, MAX_LEN, SEED)
    print(f"Всего тестов: {NUM_TESTS} шт")
    print(f"Все согласны: {all_equal} раз")
    print(f"Несовпадений: {len(mismatches)} шт")
    if mismatches:
        print('\nПримеры несовпадений (слово, regex, DFA, NFA, AFA):')
        for w, r, d, n, a in mismatches[:10]:
            print(f"  '{w}' -> regex={r}, \t DFA={d}, \t NFA={n}, \t AFA={a}")
