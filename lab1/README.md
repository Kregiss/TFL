
# Лабораторная работа №1
Моя SRS T:

|aaaa → ab<br>abbb → bba<br> babb → bb<br> aabb → aaba<br> bbbaa → bb| aaabab → baabb<br> baabb → aabab<br> baabab → bab<br> bbabab → bb<br> baabaab → a<br> bab → baaaab|
|:-|:-|

## Завершимость

Данная система не завершима, т.к. существует циклическая возрастающая последовательность переписываний:

$$
bab ⟶ baaaab ⟶ babb ⟶ baaaabb ⟶ babbb ⟶ ... ⟶ bab^n
$$

## Конечность классов эквивалентности по НФ
Введём фундированный порядок shortlex: сравнение длин (бОльшая строка больше), сравнение лексикографически: b>a.

Переориентация правил в соответствии с этим порядком:
1. aaaa → ab
2. abbb → bba
3. babb → bb
4. aabb → aaba
5. bbbaa → bb
6. aaabab → baabb
7. baabb → aabab
8. baabab → bab
9. bbabab → bb
10. baabaab → a
11. baaaab → bab &emsp; -- переориентировали это правило

В данной системе нормальной формой может быть строка $b^n$, рассмотрим её:

$$
\underbrace{bb}_{\text{3}}b ↔ b \underbrace{ab}_{\text{1}}bb ↔ \underbrace{baaaab}_{\text{11}}b ↔  \underbrace{babb}_{\text{3}} ↔ bb
$$

Значит все НФ вида $b^n$ эквивалентны строке $bb$ и состоят в одном с ней классе эквивалентности по НФ. 

Рассмотрим

   $$
   \underbrace{bb}_{\text{3}} ↔ \underbrace{bab}_{\text{11}}b ↔ baaa \underbrace{ab}_{\text{1}}b ↔ ba \underbrace{aaaa}_{\text{1}}aab ↔ \underbrace{baabaab}_{\text{10}} ↔ a
   $$
  
   $$
   \underbrace{a}_{\text{10}}a ↔ baab \underbrace{aaba}_{\text{4}} ↔ baa \underbrace{baabb}_{\text{7}} ↔ ba \underbrace{aaaa}_{\text{1}}bab ↔ b\underbrace{aabb}_{\text{4}}ab ↔ \underbrace{baabaab}_{\text{10}} ↔ a
   $$
  
   $$
   b\underbrace{a}_{\text{10}} ↔ \underbrace{bb}_{\text{4}}aabaab ↔ \underbrace{bab}_{\text{11}}baabaab ↔ baa\underbrace{bab}_{\text{10}}baabaab ↔ baabaaaa \underbrace{baabaab}_{\text{10}} ↔ baaba\underbrace{aaaa}_{\text{1}} ↔ \underbrace{baabaab}_{\text{10}} ↔ a
   $$
  
   $$
   \underbrace{a}_{\text{10}}b ↔ baa \underbrace{baabb}_{\text{7}} ↔ ba \underbrace{aaaa}_{\text{1}}bab ↔ b \underbrace{aabb}_{\text{4}}ab ↔ \underbrace{baabaab}_{\text{10}} ↔ a
   $$

Значит все нормальные формы (кроме b) лежат в одном классе эквивалентности по a. Значит у этой системы конечность классов эквивалентности по НФ.

## Локальная конфлюентность и пополняемость по Кнуту-Бендиксу

Проверка на локальную конфлюентность: возьмём критическую пару из правила 1 (aaaa → ab) с самим собой с перекрытием длиной 3.

$$ \underbrace{aaaa}_{\text{1}}a → aba $$  

$$ a\underbrace{aaaa}_{\text{1}} → aab $$

aba и aab - НФ, которые не сводятся к друг другу, значит наша система локально не конфлюентна.

Пополняемость по Кнуту-Бендиксу:

---

1. Крит. пара:  ( (aaaa, aaaa) ), ( k = 1 ), ( W = aaaaaaa )

$$
aaaaaaa \to abaaa
$$

$$
aaaaaaa \to abb
$$

* новое правило 1: `abaaa → abb`

---

2. Крит. пара:  ( (aaaa, aaaa) ), ( k = 2 ), ( W = aaaaaa )

$$
aaaaaa \to abaa
$$

$$
aaaaaa \to aaab
$$

* новое правило 2: `abaa → aaab`

---

3. Крит. пара:  ( (aaaa, aaaa) ), ( k = 3 ), ( W = aaaaa )

$$
aaaaa \to aba
$$

$$
aaaaa \to aab
$$

* новое правило 3: `aba → aab`

---

4. Крит. пара:  ( (aaaa, abbb) ), ( k = 1 ), ( W = aaaabbb )

$$
aaaabbb \to abbbb \to bbab
$$

$$
aaaabbb \to aaabba \to aaabaa \to aaaaab \to abab \to aabb \to aaba \to aaab
$$

* новое правило 4: `bbab → aaab`

---

5. Крит. пара:  ( (aaaa, aabb) ), ( k = 2 ), ( W = aaaabb )

$$
aaaabb \to abbb \to bba
$$

$$
aaaabb \to aaaaba \to abba
$$

* новое правило 5: `abba → bba`

---

6. Крит. пара:  ( (aaaa, aaabab) ), ( k = 2 ), ( W = aaaaabab )

$$
aaaaabab \to ababab \to  aabbab \to  aabaab \to  aaaabb \to abbb \to bba
$$

$$
aaaaabab \to aabaabb \to aaaabbb \to abbbb \to bbab \to aaab
$$

* новое правило 6: `aaab → bba`

---

7. Крит. пара:  ( (abbb, babb) ), ( k = 1 ), ( W = abbbabb )

$$
abbbabb \to bbaabb \to bababb \to baabbb \to aababb \to aaabbb \to bbabb \to aaabb \to bbab \to aaab \to bba \to bab
$$

$$
abbbabb \to abbbb \to bbab \to aaab \to bba
$$

* новое правило 7: `bba → bab`

---

8. Крит. пара:  ( (abbb, bbbaa) ), ( k = 3 ), ( W = abbbaa )

$$
abbbaa \to bbaaa \to babaa \to abbaa \to bbaa \to baba \to abba \to bba \to bab
$$

$$
abbbaa \to abb
$$

* новое правило 8: `bab → abb`

---

9. Крит. пара:  ( (abbb, baabb) ), ( k = 1 ), ( W = abbbaabb )

$$
abbbaabb \to bbaaabb \to babaabb \to abbaabb \to bbaabb \to bababb \to abbabb \to bbabb \to aaabb \to bbab \to aaab \to bba \to bab \to abb
$$

$$
abbbaabb \to abbaabab \to bbaabab \to bababab \to abbabab \to bbabab \to bb
$$

* новое правило 9: `abb → bb`

---

10. Крит. пара:  ( (babb, abbb) ), ( k = 3 ), ( W = babbb )

$$
babbb \to bbb \to bbb
$$

$$
babbb \to bbba \to bba \to bab \to abb \to bb
$$

* новое правило 10: `bbb → bb`

---

11. Крит. пара:  ( (baabb, baabb) ), ( k = 1 ), ( W = baabbaabb )

$$
baabbaabb \to aababaabb bbabaabb \to aaabaabb \to bbaaabb \to babaabb \to abbaabb \to bbaabb \to bababb \to abbabb \to bbabb \to aaabb \to bbab \to aaab \to bba \to bab \to abb \to bb
$$

$$
baabbaabb \to baabaabab \to aab
$$

* новое правило 11: `aab → bb`

---

12. Крит. пара:  ( (baabaab, babb) ), ( k = 1 ), ( W = baabaababb )

$$
baabaababb \to aabb \to aaba \to bba \to bab \to abb \to bb
$$

$$
baabaababb \to baabaabb \to ab
$$

* новое правило 12: `bb → ab`

---

13. Крит. пара:  ( (baabaab, aabb) ), ( k = 3 ), ( W = baabaabb )

$$
baabaabb \to ab
$$

$$
baabaabb \to baabaaba \to aa
$$

* новое правило 13: `ab → aa`

---

14. Крит. пара:  ( (baabaab, baabaab) ), ( k = 4 ), ( W = baabaabaab )

$$
baabaabaab \to aaab \to bba \to bab \to abb \to bb \to ab \to aa
$$

$$
baabaabaab \to baaa
$$

* новое правило 14: `baaa → aa`

---

15. Крит. пара:  ( (baabab, aba) ), ( k = 2 ), ( W = baababa )

$$
baababa \to baba \to abba \to bba \to bab \to abb \to bb \to ab \to aa
$$

$$
baababa \to baabaab \to a
$$

* новое правило 15: `aa → a`

---

16. Крит. пара:  ( (bba, baabaab) ), ( k = 2 ), ( W = bbaabaab )

$$
bbaabaab \to bababaab \to abbabaab \to bbabaab \to aaabaab \to bbaaab \to babaab \to abbaab \to bbaab \to babab \to abbab \to bbab \to aaab \to bba \to bab \to abb \to bb \to ab \to aa \to a
$$

$$
bbaabaab \to ba
$$

* новое правило 16: `ba → a`

Итого система после пополнения получилась:
|aaaa → ab<br>abbb → bba<br>babb → bb<br>aabb → aaba<br>bbbaa → bb<br>aaabab → baabb<br>baabb → aabab<br>baabab → bab<br>bbabab → bb<br>baabaab → a<br>baaaab → bab|abaaa → abb<br>aba → aab<br>bbab → aaab<br>abba → bba<br>aaab → bba<br>bba → bab<br>bab → abb<br>abb → bb<br>bbb → bb<br>aab → bb<br>bb → ab<br>ab → aa<br>baaa → aa<br>aa → a<br> ba → a|
|:-|:-|

Можно заметить, что правила bb → ab → aa → a и ba → a приводят любую строку (кроме `b`) к a, и все оставшиеся правила приводятся засчёт данных 4 правил в итоге к общей нормальной форме `a`. Значит итоговая минимальная система T' это:
|bb → a<br>ab → a<br> aa → a<br> ba → a|
|:-|
* Правило вида XY → a (где X,Y ∈ {a,b}) необходимо: если убрать, например, bb → a, то слово bb останется нерегулируемым по оставшимся правилам (они затрагивают только пары, содержащие a во втором месте или первое место), а в исходной srs bb выводилось в a (через правило bb -> ab и дальше). Следовательно без bb→a мы потеряем способность выводить некоторые исходные правила. Аналогично для любой другой пары: удаление любого из четырёх правил лишит систему возможности свести некоторые исходные LHS к a.
* Никакое правило в T' не выводимо из трех остальных: все четыре шаблона двухсимвольных сочетаний покрывают полный набор пар алфавита {a,b}, и ни одна пара не может быть получена редукцией из других пар (они неперекрываемы по виду), поэтому каждое необходимо.

Таким образом получена srs T'. Она является минимальной, завершимой, локально конфлюентной и сохраняет классы эквивалентности по НФ, определённые srs T.

---

Инварианты srs T:
1. Невозможность получить пустое слово
2. Длина слова не увеличивается
3. Ψ(w) = |w| + 2 * |w|_b
   
