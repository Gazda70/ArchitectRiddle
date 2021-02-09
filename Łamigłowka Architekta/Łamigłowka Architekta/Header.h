#pragma once
#include <stdio.h>
#include <stdlib.h>

/*Struktura opisuje pojedyncza cyfre - element listy*/
struct cyfra
{
	char znak;
	struct cyfra* next;
};
/*Struktura opisuje pojedyncza kratke - element tablicy dwuwymiarowej,pola gry*/
struct kratka
{
	int rodzaj;
	int wx;
	int wy;
};
/*Struktura opisuje pojedyncza plansze do gry*/
struct plansza
{
	struct kratka** pole_gry ;// reprezentacja wlasciwej planszy do gry
};
int utworz_Plansze(FILE* plik, int* countx, int*county, struct plansza* nowa);
void dodaj_do_listy(struct cyfra**pHead, char sign);
void niepoprawne_dane();
int okresl_ilosc_zbiornikow(struct cyfra*pHead, struct plansza* stwor, char type, int x, int y);
int okresl_wspolrzedne_domkow(struct cyfra*pHead, struct plansza* stwor, int x, int y,int check);
void usun_liste(struct cyfra**pHead);
int wybierz_domek(int tab[10], struct plansza* gra, int* coord, int*win,int sy);
int sprawdz_sasiedztwo(struct plansza* gra, int x, int y, int wspol[10], int war[10], char mode,int* coord,int*win, int sy);
void podaj_wspolrzedne_zbiornika(struct plansza* gra, int x, int y, int sx, int sy, int*win, int* coord);
int wprowadz_dane(int* coord);
void usun_zbiornik(struct plansza*stwor, int* win, int x, int y, int ily);
void instrukcja();
