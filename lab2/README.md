# Лабораторная работа №2
Моё регулярное выражение:

$$
((baa)* |(abb)* )(bb|cc)* (aa(ba|bab))* (a|b|c) a* b ((a|bcc*)abc)*
$$

## Минимальный ДКА

<img width="2296" height="1473" alt="минДКА(well)" src="https://github.com/user-attachments/assets/79784b55-e92e-4b4b-8f32-aa4bf9515834" />

---

Обоснование минимальности:
Таблица классов эквивалентности ДКА. Какая-то часть строки выделена зелёным - эта строка уже уникальна (незакрашеная её часть) и нет смысла проверять её дальнейшие (закрашенные зелёным) ячейки

<img width="2398" height="847" alt="dfa_table" src="https://github.com/user-attachments/assets/4ad2f8ef-fef5-4b86-ad74-f6954b003706" />


---
## НКА

ε-НКА:

<img width="2120" height="303" alt="красивыйНКА" src="https://github.com/user-attachments/assets/d8bc07f5-198d-4259-a124-321143b21535" />


НКА, построенный по ε-НКА:

<img width="2079" height="600" alt="graphviz (22)" src="https://github.com/user-attachments/assets/031b638b-27a7-450e-9cd9-590f5dd94a3e" />


---
Часть таблицы множеств классов эквивалентности НКА:

<img width="1024" height="364" alt="NFA_2" src="https://github.com/user-attachments/assets/be9b7640-da4f-439b-b09e-1ddb388d471f" />




---
## ПКА

<img width="2045" height="893" alt="graphviz (21)" src="https://github.com/user-attachments/assets/8b0621b9-2b1d-4262-a866-70c504c7e0be" />

Часть таблицы множеств классов эквивалентности ПКА:

<img width="1024" height="225" alt="_tableAFA" src="https://github.com/user-attachments/assets/6d77424a-b1fd-46f9-b637-db4bc8604b23" />


## ExtReg

$$
ˆ((baa)* |(abb)* )(bb|cc)* (aabab?)* . a* b ((a|bc^+)abc)*$
$$

Расширенное выражение распознаёт тот же язык, потому что:

  1. $c^+=cc*=c*c$
  2. wildcard-операция $.$ - замена произвольного символа алфавита
  3. $b? = (b|ε)$
  4. ˆ и $ - символы начала и конца строки соответственно
