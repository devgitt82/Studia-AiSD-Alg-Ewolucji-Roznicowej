#include <iostream>
#include <random>
#include<chrono>

#define EPSILON 0.000000000001          // to do funkcji pierwiastek, exponent, logn

using namespace std;
using namespace chrono;

default_random_engine silnik;           //potrzeba random by dzialalo stworzenie silnika losujacego


/**************************************************************************************************/
//DEFINICJE TYPOW - WSKAZNIKOW DO FUNKCJI
typedef double (*Funkcja_dwuargumentowa) (double , double);
typedef double (*Funkcja_jednoargumentowa) (double);

/**************************************************************************************************************************************/
// POJEDYNCZY EGZEMPLARZ W POPULACJI
struct osobnik
{
    int n;
    double dopasowanie;
    double argument[10];
};
/********************************************************************************************/
// ELEMENTY DO STOSU
struct lista_dbl{
    double     zmienna;
    lista_dbl  *next = nullptr;
};
/****************************************************************************************/
// ELEMENTY DO KOLEJKI - WCZYTANIA POSTACI FUNKCJI

struct lista_do_kolejki{
    double ** wsk_arg = nullptr;                            //wsk na wsk do double (do wgranych wartosci z zakresu)
    double ** wsk_stal = nullptr;                           //wsk na wsk do double( do wgranych stalych doslownych)
    Funkcja_jednoargumentowa ptr_na_f1 = nullptr;           //wsk na funkcje jednoargumentowa
    Funkcja_dwuargumentowa ptr_na_f2 = nullptr;             //wsk na funkcje dwuargumentowa
    lista_do_kolejki * next = nullptr;
};
/*****************************************************************************************/
// ZWYKLY STOS LICZB DOUBLE

struct Tstos{                               //WSZYSTKO PUBLICZNE WIEC STRUCT NIE CLASS

    lista_dbl* szczyt_stosu = nullptr;

    void dodaj(double var)
    {
        lista_dbl *temp = new lista_dbl;
        if (szczyt_stosu == nullptr){
            temp->next = nullptr;
            szczyt_stosu = temp;
        }
        else {
            temp->next = szczyt_stosu;
            szczyt_stosu = temp;
        }
        szczyt_stosu->zmienna = var;
    }
    double zwroc_szczyt(){
        return szczyt_stosu->zmienna;
    }
    void usun_szczyt()
    {
        if (szczyt_stosu != nullptr){
            lista_dbl * temp = szczyt_stosu;
            szczyt_stosu = szczyt_stosu->next;
            delete temp;
        }
    }
};
/*****************************************************************************************************/
//KOLEJKA - ELEMENTAMI SA SPECYFICZNE STRUKTURY                    //WSZYSTKO PUBLICZNE - STRUCT

struct Tkolejka{

    lista_do_kolejki* head = nullptr ;
    lista_do_kolejki* tail = nullptr ;

    void dodaj_do_kolejki(lista_do_kolejki* nowy_element){
        if (tail == nullptr){
            tail = nowy_element;
            head = tail;
        }
        else{
            tail->next = nowy_element;
            tail=tail->next;
        }
    }
    //lista_do_kolejki* zwroc_przod_kolejki(){
    //    return head;
    //}

    //TU WAZNE - NIC NIE USOWA - TYLKO SKACZE DO NASTEPNEGO ELEMENTU - PAMIETAJ BY NA KONIEC ZWOLNIC PAMIEC
    void usun_z_kolejki(){
       if (head != tail->next)
           head = head->next;
    }

    //ZWALNIA PAMIEC ZAJETA PRZEZ KOLEJKE
    void zwolnij_pamiec(){
       while (head != tail){
           lista_do_kolejki * temp = head;
           head = head->next;
           delete temp;
       }
       delete head;
       head = nullptr;
       tail = nullptr;
    }
};
/********************************************************************************************/
//DEKLARACJE FUNKCJI GLOBALNYCH
int char2int(char *tab);
double atoff(char* skladnik);  //przy zwyklym atof jest 100%
void wczytaj_kolejke(Tkolejka* kolejka );
double licz_dopasowanie(Tstos stos, Tkolejka kolejka,osobnik* osobnik_biezacy, double& a, double& b, double *tablica_argumentow);
void krzyzowanie(osobnik& mutant, osobnik* biezacy);
void mutacja(osobnik& mutant, osobnik* os1, osobnik* os2, osobnik* os3, double* zakres_dolny, double* zakres_gorny);

/**********************************************************************************************************/
//DEKLARACJE i DEFINICJE FUNKCJI MATEMATYCZNYCH

double dodawanie(double var1, double var2){var2 = var2 + var1; return var2;}
double odejmowanie(double var1, double var2){var2 = var2 - var1; return var2;}
double mnozenie(double var1, double var2){var2 = var2 * var1; return var2;}
double dzielenie(double var1, double var2){var2=var2/var1; return var2;}
double pierwiastek_kwadratowy (double var1);
double kwadrat_liczby (double var1){var1 = var1*var1; return var1;}
double sinus_liczby (double var1);                                              //{var1 = sin(var1); return var1;}
double cosinus_liczby (double var1);                                            // {var1 = cos(var1); return var1;}
double negative_liczby (double var1) {var1 = var1 * (-1); return var1;}
double abs_liczby (double var1) {if (var1<0) var1 = var1*(-1); return var1;}
double exp_liczby (double var1);                                                // {var1 = exp(var1); return var1;}
double log_liczby (double var1);                                                 // {var1 = log(var1) ;return var1;}
double potega_liczby(double var1, double var2);

/*****************************************************************************************************/
//DEKLARACJE TABLIC

double*  tablica_wsk_do_argumentow[10];
double*  tablica_wsk_do_stalych_arg[100];
double tablica_stalych[100];
double tablica_argumentow[100];

/*****************************************************************************************************/
//DEFINICJE WSKAZNIKOW FUNKCJI / OPERATOROW

Funkcja_dwuargumentowa  wsk_na_funkcje_nr_1 = dodawanie;
Funkcja_dwuargumentowa  wsk_na_funkcje_nr_2 = odejmowanie;
Funkcja_dwuargumentowa  wsk_na_funkcje_nr_3 = mnozenie;
Funkcja_dwuargumentowa  wsk_na_funkcje_nr_4 = dzielenie;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_5 = pierwiastek_kwadratowy;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_6 = kwadrat_liczby;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_7 = sinus_liczby;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_8 = cosinus_liczby;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_9 = negative_liczby;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_10 = abs_liczby;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_11 = exp_liczby;
Funkcja_jednoargumentowa wsk_na_funkcje_nr_12 = log_liczby;
Funkcja_dwuargumentowa  wsk_na_funkcje_nr_13 = potega_liczby;
/*****************************************************************************************************/

/*******************************************************************************************************************************************************/
int main(){

//PUNKT CZASU DO ZLICZNIA ILOSCI POWTORZEN PETLI
system_clock::time_point     teraz = system_clock::now();

//PRZYPISANIE WARTOSCI TABLICOM WSKAZNIKOW
for(int i=0; i<10; i++){
    tablica_wsk_do_argumentow[i] = &tablica_argumentow[i];
}
for(int i=0; i<100; i++){
    tablica_wsk_do_stalych_arg[i] = &tablica_stalych[i];
}

//UTWORZENIE KOLEJKI, STOSU, ZMIENNYCH, JEDNOSTKI CZASU

Tkolejka kolejka;                                                                                                //tworze  kolejka
Tstos stos;                                                                                                              //tworze  stos
duration<int> jednasekunda(1);

double a,b;
double minimum = 2000970000; //gorny zakres doubla wpisac
int n{0},t{0};

//TABLICA DO WYPISANIA JAKO ROIWAZANIE
double wart_minimum[10];

//USTAIWENIE LICZEBNOSCI POPULACJI
int liczebnosc = 100;

//STWORZENIE TABLIC ZAKRESOW
double zakres_dolny[10]{0};
double zakres_gorny[10]{0};

//TABLICA ZNAKOW DO POMINIECIA KOMENTARZA
char komentarz[150];

//STWORZENIE POPULACJI I MUTANTA
osobnik populacja[liczebnosc];
osobnik mutant;

//WPISANIE W MUTANTA WARTOSCI MAX DOUBLE
mutant.dopasowanie = minimum;

/*******************************************************************************************************************************/
//WCZYTANIE WEJSCIA

//WCZYTANIE LINI KOMENTARZA I LINI Z ILOSCIA ARGUMENTOW ORAZ CZASEM
while (cin.getline(komentarz,150,'\n')&& *komentarz=='#' ){}

//Z OSTATNIEJ WCZYTANEJ LINI WYLUSKAJ n ORAZ t      (n maxymalnie = 10)
if (komentarz[1] == ' ')
{
    n = komentarz[0] - '0';
    if (komentarz[3] == '\0')
        t = komentarz[2] - '0';
    else
        t = (komentarz[2] - '0') * 10 + (komentarz[3] - '0');       //t tez nie wiecej niz 99s
}
if (komentarz[1] != ' ')
{
    n = (komentarz[0] - '0') * 10 + (komentarz[1] - '0');
    if (komentarz[4] == '\0')
        t = komentarz[3] - '0';
    else
        t = (komentarz[3] - '0') * 10 + (komentarz[4] - '0');
}

mutant.n = n;             //od razu zeby nie zapomniec, wpisanie ilosci argumentow do mutanta

//WCZYTANIE ZAKRESOW ARGUMENTOW
for (int i=0; i<n; i++){
        cin>>zakres_dolny[i];
        cin>>zakres_gorny[i];
}
//WCZYTANIE FUNKCJI DO POLICZENIA MINIMUM
    wczytaj_kolejke(&kolejka);

/****************************************************************************************************************************/

//TWORZENIE POPULACJI LOSOWYCH OSOBNIKOW
for (int i=0; i<liczebnosc; i++){
        populacja[i].n = n;
        for (int j=0; j<n; j++){
            uniform_real_distribution<double> rozklad_od_dol_do_gora (zakres_dolny[j],zakres_gorny[j]);     //rozklad z jednak prawdop liczb double od zakresu do zakresu
            populacja[i].argument[j] =  rozklad_od_dol_do_gora(silnik);                                     //wpisanie losowych licz z ustawionego zakresu
        }

        populacja[i].dopasowanie = licz_dopasowanie(stos,kolejka,&populacja[i],a,b,tablica_argumentow);     //obliczenie poczakowego dopasowania

        if (minimum > populacja[i].dopasowanie){                                                         //sprawdzenie poczatkowego minimum funkcji
            minimum = populacja[i].dopasowanie;
             for (int k=0; k<n; k++)
                        wart_minimum[k]=populacja[i].argument[k];
        }
}
/**************************************************************************************************************/
    //DOPOKI TIMER SIE NIE SKONCZY

while (system_clock::now() < teraz + ((t-0.1)*jednasekunda)){

            //DLA KAZDEGO OSOBNIKA W POPULACJI
      for (int i=0; i<liczebnosc; i++){
                uniform_int_distribution<int> rozklad_intigera(0,liczebnosc-1);     //wylosuj 3 roznych osobnikow z populacji
                int nr1 = rozklad_intigera(silnik);
                while (nr1 == i)
                    nr1 = rozklad_intigera(silnik);
                int nr2 = rozklad_intigera(silnik);
                while ((nr2 == i) || (nr2 == nr1))
                    nr2 = rozklad_intigera(silnik);
                int nr3 = rozklad_intigera(silnik);
                while ((nr3 == i) || (nr3 == nr1) || (nr3 == nr2))
                    nr3 = rozklad_intigera(silnik);

        //ZROB MUTACJE I KRZYZOWANIE
                mutacja(mutant, &populacja[nr1], &populacja[nr2], &populacja[nr3],zakres_dolny,zakres_gorny);
                krzyzowanie(mutant, &populacja[i]);
        //SPRAWDZ DOPASOWANIE SKRZYZOWANEGO MUTANTA
                mutant.dopasowanie = licz_dopasowanie(stos,kolejka,&mutant,a,b,tablica_argumentow);
        //ZASTAP OSOBNIKA MUTANTEM JESLI MUTANT MA LEPSZE DOPASOWANIE
                if (populacja[i].dopasowanie > mutant.dopasowanie){
                        populacja[i].dopasowanie = mutant.dopasowanie;
                        for (int j=0; j<n; j++) populacja[i].argument[j] = mutant.argument[j];
                }
        //SPRAWDZ MINIMUM FUNKCJI
                if(populacja[i].dopasowanie < minimum){
                    minimum = populacja[i].dopasowanie;
                    for (int k=0; k<n; k++)
                        wart_minimum[k]=populacja[i].argument[k];
                 }
        }
    }                                       //koniec while

for (int k=0; k<n; k++)
       cout<<wart_minimum[k]<<" ";

//ZWALNIANIE PAMIECI
while (stos.szczyt_stosu != nullptr)
    stos.usun_szczyt();

kolejka.zwolnij_pamiec();

return 0;  //KONIEC MAINA
}

/**************************************************************************************************************/
/**************************************************************************************************************/
/**************************************************************************************************************/

void wczytaj_kolejke(Tkolejka* kolejka){
     int counter = 0;
     char skladnik_funkcji[10];
     while (cin>>skladnik_funkcji){

    //TE LINIJKI PONIZEJ ZOSTAW W KOMENTARZU - DO TESTOWANIA ZAMIEN Z WHILE POWYZEJ
    //for (int i=0; i<115; i++){
    //cin>>skladnik_funkcji;

        //  + DODAWANIE
                        if (skladnik_funkcji[0] == '+' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f2 = wsk_na_funkcje_nr_1;
                             kolejka->dodaj_do_kolejki(temp);
                        }
       //  - ODEJMOWANIE
                        if ((skladnik_funkcji[0] == '-' ) && (skladnik_funkcji[1] == '\0' ))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f2 = wsk_na_funkcje_nr_2;
                             kolejka->dodaj_do_kolejki(temp);
                        }
       //  *   MNOZENIE
                        if (skladnik_funkcji[0] == '*' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f2 = wsk_na_funkcje_nr_3;
                             kolejka->dodaj_do_kolejki(temp);
                        }
       // *   DZIELENIE
                        if (skladnik_funkcji[0] == '/' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f2 = wsk_na_funkcje_nr_4;
                             kolejka->dodaj_do_kolejki(temp);
                        }
       //      SQRT
                        if ((skladnik_funkcji[0] == 's' ) && (skladnik_funkcji[1] == 'q') && (skladnik_funkcji[3] == 't'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_5;
                             kolejka->dodaj_do_kolejki(temp);       //      SQR
                        }
                        if ((skladnik_funkcji[0] == 's' ) && (skladnik_funkcji[1] == 'q') && (skladnik_funkcji[3] != 't'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_6;
                             kolejka->dodaj_do_kolejki(temp);
                        }
        //    SINUS
                        if ((skladnik_funkcji[0] == 's' ) && (skladnik_funkcji[1] == 'i'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_7;
                             kolejka->dodaj_do_kolejki(temp);
                        }
        //    COSINUS
                        if ((skladnik_funkcji[0] == 'c' ) && (skladnik_funkcji[1] == 'o'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_8;
                             kolejka->dodaj_do_kolejki(temp);
                        }
        //    NEGATIVE
                        if (skladnik_funkcji[0] == 'n' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_9;
                             kolejka->dodaj_do_kolejki(temp);
                        }
        //    ABS
                        if ((skladnik_funkcji[0] == 'a' ) && (skladnik_funkcji[1] == 'b'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_10;
                             kolejka->dodaj_do_kolejki(temp);
                        }
        //    EXP
                        if ((skladnik_funkcji[0] == 'e' ) && (skladnik_funkcji[1] != '\0'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_11;
                             kolejka->dodaj_do_kolejki(temp);
                        }
        //    LOG
                        if ((skladnik_funkcji[0] == 'l' ) && (skladnik_funkcji[1] == 'o'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f1 = wsk_na_funkcje_nr_12;
                             kolejka->dodaj_do_kolejki(temp);
                        }
        // *   POTEGA
                        if ((skladnik_funkcji[0] == 'p' ) && (skladnik_funkcji[1] == 'o'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->ptr_na_f2 = wsk_na_funkcje_nr_13;
                             kolejka->dodaj_do_kolejki(temp);
                        }


                        if (skladnik_funkcji[0] == 'x' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = tablica_wsk_do_argumentow;
                             kolejka->dodaj_do_kolejki(temp);

                        }
                        if (skladnik_funkcji[0] == 'y' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (&tablica_wsk_do_argumentow[1]);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                        if (skladnik_funkcji[0] == 'z' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (&tablica_wsk_do_argumentow[2]);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                       if ((skladnik_funkcji[0] == 'a' ) && (skladnik_funkcji[1] == '\0'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (tablica_wsk_do_argumentow+3);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                          if (skladnik_funkcji[0] == 'b' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (tablica_wsk_do_argumentow+4);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                          if ((skladnik_funkcji[0] == 'c' ) && (skladnik_funkcji[1] == '\0'))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (tablica_wsk_do_argumentow+5);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                          if (skladnik_funkcji[0] == 'd' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (tablica_wsk_do_argumentow+6);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                          if ((skladnik_funkcji[0] == 'e' ) && (skladnik_funkcji[1] == '\0' ))
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (tablica_wsk_do_argumentow+7);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                          if (skladnik_funkcji[0] == 'f' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (tablica_wsk_do_argumentow+8);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                          if (skladnik_funkcji[0] == 'g' )
                        {
                             lista_do_kolejki * temp = new lista_do_kolejki;
                             temp->wsk_arg = (tablica_wsk_do_argumentow+9);
                             kolejka->dodaj_do_kolejki(temp);

                        }
                          if ((skladnik_funkcji[0] == 'p') && (skladnik_funkcji[1] == 'i'))
                        {

                             lista_do_kolejki * temp = new lista_do_kolejki;
                             tablica_stalych[counter] = 3.1415926535897932384626;
                             temp->wsk_stal = (tablica_wsk_do_stalych_arg + counter);
                             kolejka->dodaj_do_kolejki(temp);
                             counter++;
                        }

                        if (((skladnik_funkcji[0] >= '0') && (skladnik_funkcji[0] <= '9')) || ((skladnik_funkcji[0] == '-') && (skladnik_funkcji[1] != '\0')))
                        {

                             lista_do_kolejki * temp = new lista_do_kolejki;
                             tablica_stalych[counter] = atoff(skladnik_funkcji);
                             temp->wsk_stal = (tablica_wsk_do_stalych_arg + counter);
                             kolejka->dodaj_do_kolejki(temp);
                             counter++;
                        }
            for (int j=0; j<10; j++){skladnik_funkcji[j]='\0';}
       }//koniec while
}           //koniec wczytaj_kolejke()

/*******************************************************************************************************************/
//DEFINICJE FUNKCJI GLOBALNYCH

double licz_dopasowanie(Tstos stos, Tkolejka kolejka,osobnik* osobnik_biezacy, double& a, double& b, double *tablica_argumentow){

    for (int i=0; i < (osobnik_biezacy->n); i++){                   //SKOPIUJ ARGUMENTY BIEZACEGO OSOBNIKA DO TABLICY ARGUMENTOW
        tablica_argumentow[i] = osobnik_biezacy->argument[i];
        }
    while (kolejka.head != kolejka.tail->next)                      //PRZEJEDZ PO KOLEJCE I DODAJ WARTOSCI NA STOS - ODCZYTAJ FUNKCJE W ONP
    {
       if (kolejka.head->wsk_arg != nullptr){                   //gdy z przodu kolejki argument: x,y,z...
               stos.dodaj(**kolejka.head->wsk_arg);             //to dodaj to, na co wskazuje wskaznik pod x,y,z, na stos
               kolejka.usun_z_kolejki();                        //(tak naprawde to przejedzie do nastepnego, nie usowa nic - tylko udaje)

       }
       else if (kolejka.head->wsk_stal != nullptr){             //gdy z przodu kolejki jest stala wartosc: 1,2,3,3.12 itp
               stos.dodaj(**kolejka.head->wsk_stal);            //to dodaj to na stos
               kolejka.usun_z_kolejki();

        }
       else if (kolejka.head->ptr_na_f2 != nullptr){            //gdy z przodu kolejki jest operator - wskaznik na funkcje przyjmujaca 2 argumenty,
               a = stos.zwroc_szczyt();                         //to scianij dwa argumenty ze stosu i wykonaj funkcje, po czym wynik wloz na stos
               stos.usun_szczyt();
               b = stos.zwroc_szczyt();
               stos.usun_szczyt();
               a = kolejka.head->ptr_na_f2(a,b);
               stos.dodaj(a);
               kolejka.usun_z_kolejki();
       }
       else if (kolejka.head->ptr_na_f1 != nullptr){            //gdy z przodu jest operator - wskaznik na funkcje przyjmujaca jeden argument
               a = stos.zwroc_szczyt();                         //to sciagnij ze stosu jeden argument, wykonaj funkcje, wynij wloz na stos
               stos.usun_szczyt();
               a = kolejka.head->ptr_na_f1(a);
               stos.dodaj(a);
               kolejka.usun_z_kolejki();
       }
    }
    a = stos.zwroc_szczyt();
    stos.usun_szczyt();                                         //oproznij stos
return a;                                                       //zwroc wynik
}
/******************************************************************************************************************************************************/
void mutacja(osobnik& mutant, osobnik* os1, osobnik* os2, osobnik* os3, double* zakres_dolny, double* zakres_gorny){

    normal_distribution<double> rozklad_od_0_do_1 (0 , 0.999999);                      //rozklad normalny liczb double, od 0 do 1, bez 1
    double F = rozklad_od_0_do_1(silnik);
    for (int i = 0; i<(os1->n);  i++){


        double roznica = F* (os2->argument[i] - os3->argument[i]);                                                     //MUTACJA wspolczynnik F= losowy
        if (((os1->argument[i] + roznica) <= zakres_gorny[i]) && ((os1->argument[i] + roznica) >= zakres_dolny[i]))       //SPRAWDZANIE CZY MUTANT JEST W ZAKRESIE
                mutant.argument[i] = os1->argument[i] + roznica;
        else if (((os1->argument[i] - roznica) <= zakres_gorny[i]) && ((os1->argument[i] - roznica) >= zakres_dolny[i]))  //SPRAWDZANIE CZY MUTANT JEST W ZAKRESIE
                mutant.argument[i] = os1->argument[i] - roznica;
        else
                mutant.argument[i] = os1->argument[i];
        }
    }
/**************************************************************************************************************************************/
void krzyzowanie(osobnik& mutant, osobnik* biezacy){
        normal_distribution<double> rozklad_od_0_do_1 (0 , 0.999999);;                      //rozklad normalny liczb double, od 0 do 1, bez 1
        uniform_int_distribution<int> rozklad_od_0_do_n_minus_1 (0 , mutant.n - 1);         //rozklad z identycznym pradopodob - tyle ile argumentow
        for (int i = 0; i<(mutant.n); i++){
            double rnd_wart_argumentu = rozklad_od_0_do_1(silnik);             //losowa zmiana double zgodna z rozkladem normalnym
            int rnd_nr_argument = rozklad_od_0_do_n_minus_1(silnik);           //losowa zmiana int z ident prawdopod.
            if ((rnd_wart_argumentu < 0.9) || (rnd_nr_argument == i))          // wspolczynnik CR - musi byc mniejszy rowny , teraz 0.9 co oznacza:
                {}                                                             // zazwyczaj zostaw zmutowany agument
            else
                {mutant.argument[i] = biezacy->argument[i];}                   //czasami zostaw argument biezacego osobnika
        }
} //koniec krzyzowanie


/******************************************************************************************************************************************************************************************************************/
/******************************************************************************************************************************************************************************************************************/
double pierwiastek_kwadratowy(double var1) {
    double low = 0;
    double high = var1;
    double mid = 0;

    while (high - low > EPSILON) {
            mid = low + (high - low) / 2;                //szuka wartosci sredniej
            if (mid * mid > var1) {
                high = mid;
            } else {
                low = mid;
            }
    }
return mid;                                             //tu mid jest juz pierwiastkiem z dokladnoscia do EPSILON
}
/***************************************************************************************************************************************/
double potega_liczby(double var1, double var2){         //algorytm szybkiego potegowania
         double wynik = 0;
         if (var1 == 0)
         return 1;
         int war1 = int(var1);
         if (war1%2 != 0)
        return var2 * potega_liczby(var1 - 1, var2);
         else
        wynik = potega_liczby(var1 / 2, var2);
         return wynik * wynik;
}
/***************************************************************************************************************************************/
double silnia(double a){
    if (a > 0)
        return a * silnia(a - 1);
    else
        return 1;
}
/***************************************************************************************************************************************/
double sinus_liczby(double var1){
    double wynik = 0;
    for (int i=0; i<33; i++){
            wynik = ((potega_liczby(i, -1)) / silnia(2*i+1)) * (potega_liczby(2*i+1, var1)) + wynik;
    }
    return wynik;
}
/***************************************************************************************************************************************/
double cosinus_liczby(double var1){

    double wynik=0;
    for (int i=0; i<20; i++){
            wynik = ((potega_liczby(i,-1)) * (potega_liczby(2*i, var1)) / silnia(2*i)) + wynik;}
    return wynik;
}
/***************************************************************************************************************************************/
double exp_liczby(double var1){
    double kolejny = 1;
    double wynik = 0;
    bool ujemny = false;
    int i = 1;
    wynik = 0;
    if (var1 < 0) {
        ujemny = true;
        var1 = -var1;
    }
    do
    {
    wynik = kolejny + wynik;
    kolejny = kolejny * (var1 / i);
    i++;
      if (wynik >2000970000 ) break;
    }
    while (kolejny >= EPSILON);
    if (ujemny == true)
       return 1 / wynik;
    else
    return wynik;
}
/***************************************************************************************************************************************/
double log_liczby(double var1){
    double dol = 0;
    double srodek = 0;
    double gora = var1;
    while ((gora-dol)>EPSILON){
        srodek = dol + (gora - dol)/2;
        if (exp_liczby(srodek) > var1)
            gora = srodek;
        else
            dol = srodek;
    }
    return srodek;
}
/*******************************************************************/
double atoff(char* skladnik){
    int i        = 0;
    int licznik1 = 0;
    int licznik2 = 0;
    while (skladnik[i] != '\0')
    {
        licznik1++;
        i++;
    }
    i = 0;
    while (skladnik[i] != '.') {
        if   (skladnik[i] != '\0')
        {
        licznik2++;;
        i++;
        }
        else
          break;
    }
    if (licznik1==licznik2)
        return char2int(skladnik);       //od 0 do licznik1
    else
        {
        char temp[50];
        int chwilowy = licznik2+1;
        int do_i = chwilowy;
        for(int i = chwilowy; i <= licznik1; i++)
            temp[i-(do_i)] = skladnik[chwilowy++];         //tu czesc ulamkowa
        skladnik[licznik2] = '\0';                         //tu czesc calkowita
        double wynik1, wynik2;
        int ile_cyfr = licznik1 - licznik2;
        wynik1 = abs(char2int(skladnik));
        wynik2 = char2int(temp);
        wynik1 = wynik1+ wynik2/potega_liczby(ile_cyfr-1,10);
        
                if (skladnik[0] == '-')
                    return -wynik1;
                else
                    return wynik1;
    }
}
/**********************************************************************/
int char2int(char *tab)
{
    int wynik = 0;
    int symbol = 1;                                           //potrzebne do liczb ujemnych
    int i = 0;
    if (tab[0] == '-'){                                        //jak minus to zamien symbol
        symbol = -1;
        i++;
    }
    for ( ; tab[i] != '\0'; i++){
        wynik = (wynik * 10) + (tab[i] - '0');               // '0' ma kod 48
        tab[i] = '\0';                                       //zeruje tablice przed kolejna liczba
    }
    return wynik * symbol ;
}
