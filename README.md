# Minimalizacja funkcji (alg. populacyjny).

**Program szukający minimum funkcji podanej na wejściu. Agorytm metaheurystyczny - algorytm ewolucji różnicowej. Zadanie  przedmiotu AiSD na drugim roku studiów, programowanie proceduralne bez STL.**

**Wymagania**
---

* Algorytm nie musi znajdować dokładnego rozwiązania, ale także za bardzo nie może się mylić.
* W zadaniu można korzystać z obiektów i funkcji dostępnych w plikach nagłówkowych random i chrono.
* STL niedostępny.
* Wszystkie funkcje: log, silnia, potęgowanie, pierwiastkowanie itp. muszą byc napisane samodzielnie.

**Wejście**
---

Na początku wejścia zero lub więcej linii będzie zaczynało się od znaku # (hasz). Linie te są komentarzem i należy je pominąć. 
Po komentarzach, w osobnej linii, pojawią się: liczba argumentów funkcji n oraz limit czasu t. Argumenty nazywane są pojedynczymi literami w następującej kolejności: x, y, z, a, b, c, d, e, f, g.
Następnie w n kolejnych liniach pojawią się zakresy dla kolejnych argumentów. Każdy zakres to para dwóch liczb: dolna granica zakresu i górna granica zakresu (granice należą do zakresu). 
Na końcu podana będzie funkcja do minimalizacji. Funkcja będzie podana w postaci ONP. Operacje, które mogą się pojawić to:
* **+, -, \*, /** podstawowe operatory,
* **pow** potęgowanie np. 2 3 pow pozostawia na stosie 8,
* **sin, cos** funkcje trygonometryczne (argument w radianach) np. 0 cos pozostawia na stosie 1,
* **x, y, z, a, b, c, d, e, f, g** dodanie na stos wartości odpowiadającej odpowiedniemu argumentowi,
* **p**i dodanie na stos liczby Pi,
* **neg**  zmiana znaku wartości na szczycie stosu,
* **abs** wartość bezwzględna wartości na szczycie stosu,
* **sqr** podniesienie wartości na szczycie stosu do kwadratu,
* **sqrt** wyciągnięcie pierwiastka kwadratowego z wartości na szczycie stosu ,
* **exp** zastąpienie wartości v na szczycie stosu przez e podniesione do potęgi v,
* **log** zastąpienie wartości v na szczycie stosu przez logarytm natrualny z v,

    Z limitu czasu podanego na wejściu można skorzystać do wyznaczenia liczby iteracji algorytmu.

**Wyjście**
---
    
Na wyjściu należy wypisać n liczb - wartości argumentów, dla których funkcja osiąga minimum.

**Przykłady**
---
W pierwszym przykładzie szukamy minimum funkcji |x-5|.

**Wejście**
<code>
1 1
-10 10
x 5 - abs 
</code>

**Wyjście**
<code>
4.997559
</code>

---

W drugim przykładzie minimalizujemy funkcję (x-5)2+(y-5)2. Odleglosc od punktu (5, 5).

**Wejście**
<code>
2 1
0 10
0 10
x 5 - sqr y 5 - sqr +
</code>

**Wyjście**
<code>
5.000000 4.999695
</code>
