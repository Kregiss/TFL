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

<img width="1559" height="988" alt="final_НКА(well)" src="https://github.com/user-attachments/assets/71681250-fc38-43a6-bd16-4429e00e410b" />

---
Часть таблицы множеств классов эквивалентности НКА:

<img width="1024" height="444" alt="_tableNFA" src="https://github.com/user-attachments/assets/863a67b7-50eb-4abe-82a6-86eec898b191" />



---
## ПКА

<img width="1685" height="1257" alt="ПКА" src="https://github.com/user-attachments/assets/a9f17b89-053c-42d8-a6eb-1080fa6c4530" />

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
