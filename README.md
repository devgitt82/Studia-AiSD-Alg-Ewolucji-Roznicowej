## Minimalizacja funkcji (alg. populacyjny)

Program szukaj�cy minimum funkcji podanej na wej�ciu. Agorytm metaheurystyczny - algorytm ewolucji r�nicowej.

Algorytm nie musi znajdowa� dok�adnego rozwi�zania, ale nie mo�e tak�e za bardzo si� myli�.

- Wej�cie
        - Na pocz�tku wej�cia zero lub wi�cej linii b�dzie zaczyna�o si� od znaku # (hasz). Linie te s� komentarzem i nale�y je pomin��. 
        - Po komentarzach, w osobnej linii, pojawi� si�: liczba argument�w funkcji� n� oraz limit czasu� t. Argumenty nazywane s� pojedynczymi literami w nast�puj�cej kolejno�ci: x, y, z, a, b, c, d, e, f, g. - - 
        - Nast�pnie w� n� kolejnych liniach pojawi� si� zakresy dla kolejnych argument�w. Ka�dy zakres to para dw�ch liczb: dolna granica zakresu i g�rna granica zakresu (granice nale�� do zakresu). 
        - Na ko�cu podana b�dzie funkcja do minimalizacji. Funkcja b�dzie podana w postaci ONP. Operacje, kt�re mog� si� pojawi� to:
            - +, -, *, /� � podstawowe operatory,
            - pow� � pot�gowanie np.� 2 3 pow� pozostawia na stosie 8,
            - sin, cos� � funkcje trygonometryczne (argument w radianach) np.� 0 cos� pozostawia na stosie 1,
            - x, y, z, a, b, c, d, e, f, g� � dodanie na stos warto�ci odpowiadaj�cej odpowiedniemu argumentowi,
            - pi� � dodanie na stos liczby ?,
            - neg� � zmiana znaku warto�ci na szczycie stosu,
            - abs� � warto�� bezwzgl�dna warto�ci na szczycie stosu,
            - sqr� � podniesienie warto�ci na szczycie stosu do kwadratu,
            - sqrt� � wyci�gni�cie pierwiastka kwadratowego z warto�ci na szczycie stosu ,
            - exp� � zast�pienie warto�ci� v� na szczycie stosu przez e podniesione do pot�gi� v,
            - log� � zast�pienie warto�ci� v� na szczycie stosu przez logarytm natrualny z� v,

Z limitu czasu podanego na wej�ciu mo�na było skorzysta� do wyznaczenia liczby iteracji algorytmu.
- Wyj�cie
        - Na wyj�ciu nale�y wypisa� n� liczb - warto�ci argument�w, dla kt�rych funkcja osi�ga minimum.
    
 - Przyk�ad
        - Wej�cie
1 1
-10 10
x 5 - abs

        - Wyj�cie
4.997559

        - Wej�cie
# distance from point (5, 5)
2 1
0 10
0 10
x 5 - sqr y 5 - sqr +

        - Wyj�cie
5.000000 4.999695

- W pierwszym przyk�adzie szukamy minimum funkcji |x-5|, w drugim minimalizujemy funkcj� (x-5)2+(y-5)2.
