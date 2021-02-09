
#include "Header.h"
/*funkcja pobiera z pliku dane o planszy do gry we wlasciwym formacie i na ich podstawie
okresla wartosci poszczegolnych pol w tablicy struktury reprezentuj¹cej pole gry*/
int utworz_Plansze(FILE *plik, int* countx, int*county, struct plansza* nowa)
{
	char scan = 0; //zmienna pomocnicza do obslugi funkcji fgetc - do niej zapisany zostaje ostatni pobrany z pliku znak
	int checkx = 1,checky = 1,checkw=1; //zmienne okreslaj¹ce poprawnosc liczby wierszy, kolumn oraz domkow
	struct cyfra* pHeadx = NULL; //wskaznik na glowe listy elementow pierwszego wiersza 
	struct cyfra* pHeady = NULL; //wskaznik na glowe listy elementow pierwszej kolumny
	struct cyfra* pHeadw = NULL; //wskaznik na glowe listy wspolrzednych pol zawieraj¹cych domki
	if (plik)
	{
		if ((*countx) == 1 && (*county) == 1)
		{
			// utworzenie listy kolumn
			while (((scan = fgetc(plik)) != EOF) && (scan != '\n')) //pêtla sczytuj¹ca i dodaj¹ca do listy elementy pierwszej linijki tekstu
			{
				if (scan == ',')
					(*county)++;
				dodaj_do_listy(&pHeady, scan);
			}
			if (scan == EOF)
			{
				return 0;
			}
			// utworzenie listy wierszy
			while (((scan = fgetc(plik)) != EOF) && (scan != '\n')) //pêtla sczytuj¹ca i dodaj¹ca do listy elementy drugiej linijki tekstu
			{
				if (scan == ',')
					(*countx)++;
				dodaj_do_listy(&pHeadx, scan);
			}
			if (scan == EOF)
			{
				return 0;
			}
			//alokuj_tablice_2d;
			nowa->pole_gry = (struct kratka**)malloc(((*countx) + 1) * sizeof(struct kratka*));// tu nastêpuje alokacja tablicy 2d struktur kratka
			for (int i = 0; i <= (*countx); i++)
			{
				nowa->pole_gry[i] = (struct kratka*)malloc(((*county) + 1) * sizeof(struct kratka));
			}
			for (int n = 0; n <= (*countx); n++) // tu zaalokowana tablica jest inicjalizowana wartosciami 0
			{
				for (int m = 0; m <= (*county); m++)
				{
					(nowa->pole_gry[n][m]).rodzaj = 0;
					(nowa->pole_gry[n][m]).wx = 0;
					(nowa->pole_gry[n][m]).wy = 0;
				}
			}
			/*w tym miejscu zawartosci list cyfr s¹ interpretowane jako liczby calkowite
			i ustawiane w odpowiednich miejscach tablicy 2d - oznaczaj¹ one ilosc zbiornikow w danym wierszu lub kolumnie*/
			checkx = okresl_ilosc_zbiornikow(pHeadx, nowa, 'x', (*countx), (*county));
			checky = okresl_ilosc_zbiornikow(pHeady, nowa, 'y', (*countx), (*county));
			if ((!checkx) || (!checky)||(checkx!=checky))
			{
				return 0;
			}
			// utworzenie listy domkow
			while (((scan = fgetc(plik)) != EOF) && (scan != '\n')) //pêtla sczytuj¹ca i dodaj¹ca do listy elementy trzeciej linijki tekstu
			{
				dodaj_do_listy(&pHeadw, scan);
			}
			// interpretacja listy domkow
			checkw = okresl_wspolrzedne_domkow(pHeadw, nowa, (*countx), (*county), checkx);// tutaj funkcja interpretuje trzeci¹ z list jako zbiór wspo³rzêdnych pól tablicy 2d na których maja byc domki
			/*listy zostaj¹ usuniete*/
			usun_liste(&pHeadx);
			usun_liste(&pHeady);
			usun_liste(&pHeadw);
			return checkw;
		}
	}
	return 0;
};
/*Funkcja dodaje do listy elementy typu cyfra*/
void dodaj_do_listy(struct cyfra**pHead, char sign)
{
	struct cyfra*temp=NULL; // pomocniczy wskaznik
	if ((*pHead) == NULL)
	{
		(*pHead) = malloc(sizeof(struct cyfra));
		(*pHead)->znak = sign;
		(*pHead)->next = NULL;
	}
	else
	{
		temp = *pHead;
		(*pHead)= malloc(sizeof(struct cyfra));
		(*pHead)->znak = sign;
		(*pHead)->next = temp;
	}
}
/*Funkcja odczytuje elementy listy i tworzy z nich wartosci wpisywane nastêpnie w zaleznosci od
zmiennej, do pierwszego wiersza lub do pierwszej kolumny tablicy jednoczesnie sprawdzaj¹c ich poprawnosc.*/
int okresl_ilosc_zbiornikow (struct cyfra*pHead, struct plansza* stwor, char type, int x, int y)
{
	int l_domkow = 0; //liczba sprawdzajaca ile lacznie domkow znajdzie sie we wszystkich wierszach i kolumnach (wykrywanie bledow)
	int koord = 0; //licznik okreslaj¹cy miejsce gdzie w tablicy ma byc umieszczony element nad ktorym aktualnie pracuje funkcja
	if (type == 'x')
		 koord = x;
	else if (type == 'y')
		koord = y;
	int mult = 1; //zmienna sluzaca mnozeniu przez kolejne potêgi 10
	int max = 0; //zmienna sluzaca do okreœlania wartosci aktualnego najwiekszego elementu (wykrywanie bledow)
	int idx = 0; //zmienna sluzaca zapisywaniu aktualnej wartosci tworzonego elementu - sumy elementow listy cyfr pomnozonych przez kolejne wielokrotnosci 10
	while (pHead != NULL)
	{
      idx = 0;
		while ((pHead!= NULL)&&(pHead->znak != ','))
		{
			idx += ((int)(pHead->znak-48))*mult;
			mult = mult * 10;
			pHead = pHead->next;
		}
		if (idx > max)
		{
			max = idx;
			if ((type == 'x'&&max > y) || (type == 'y'&&max > x))
			{
				return 0;
			}
		}
		if (type == 'x')
		{
			(stwor->pole_gry[koord][0]).rodzaj = idx;
			l_domkow+=idx;
		}
		else if (type == 'y')
		{
			(stwor->pole_gry[0][koord]).rodzaj = idx;
			l_domkow+=idx;
		}
		 mult = 1;
		 koord--;
	   if(pHead)
        pHead = pHead->next;
	}
		return l_domkow;
}
/*Funkcja interpretuje zawartosc listy wspolrzêdnych zbiornikow i na tej podstawie ustawia na
odpowiednich polach planszy do gry wartosc 1*/
int okresl_wspolrzedne_domkow(struct cyfra*pHead, struct plansza* stwor, int x, int y,int check)
{
	int idx = 0, idy = 0; // zmienne przechowujace wspolrzedne wierszowe i kolumnowe na etapie ich tworzenia
	int mult = 1; // zmienna sluzaca do przechowywania kolejnych poteg 10
	int zmiana = 0; // zmienna okreslajaca czy tworzymy wspolrzedna wierszowa czy kolumnowa
	while (pHead != NULL)
	{
		while ((pHead != NULL) && (pHead->znak != '('))
		{
			if (pHead->znak == ',')
			{
				zmiana = 1;
				mult = 1;
			} 
			else if (pHead->znak != ')')
			{
				if (zmiana)
				{
					idx += ((int)(pHead->znak - 48))*mult;
					mult = mult * 10;
				}
				else
				{
					idy += ((int)(pHead->znak - 48))*mult;
					mult = mult * 10;
				}
			}
			pHead = pHead->next;
		}
		zmiana = 0;
		mult = 1;
		if ((idx <= x) && (idy <= y))
		{
			(stwor->pole_gry[idx][idy]).rodzaj =1;
			idx = 0;
			idy = 0;
			check--;
		}
		else
		{
			niepoprawne_dane();
			return 0;
		}
		pHead = pHead->next;
	}
	if (check == 0)
		return 1;
	else return 0;
}
/*Funkcja wyswietla komunikat o niepoprawnosci danych*/
void niepoprawne_dane()
{
	printf("%s\n", "Niepoprawne dane!");
	getchar();
}

/*Funkcja usuwa liste*/
void usun_liste(struct cyfra**pHead)
{
	struct cyfra* temp = NULL; // pomocniczy wskaznik
	while ((*pHead) != NULL)
	{
		temp = (*pHead);
		(*pHead) = (*pHead)->next;
		free(temp);
	}
}
/*Funkcja otrzymuje wspolrzedne pola na ktorym uzytkownik chce postawic domek.
W zale¿nosci czy spelnione sa odpowiednie warunki funkcja moze na to zezwolic. */
void podaj_wspolrzedne_zbiornika(struct plansza* gra,int x, int y,int sx, int sy,int*win, int* coord)
{
	int alok = 0; // zmienna binarna sprawdzajaca czy mozna postawic domek
	int idk = 0; // indeks tablicy 
	int wspol[10] = { 0,0,0,0,0,0,0,0,x,y };// tablica przechowujaca wspolrzedne domkow otaczajacych pole - maks 4
	if ((gra->pole_gry[x][y].rodzaj== 0)&&(win[y-1]-1>=0)&& (win[x+sy-1] - 1 >= 0))
	{
		
			if (y - 1 == 0)// lewa skrajna kolumna
			{
				if (x - 1 == 0)// lewy gorny rog
				{
					int war[10] = { 1,1,0,1,1,0,0,0,0,0 };
				alok =	sprawdz_sasiedztwo(gra, x, y, wspol, war, 'r', coord,win, sy);
				}
				else if (x + 1 > sx)// lewy dolny rog
				{
					int war[10] = { -1,1,-1,0,0,1,0,0,0,0 };
					alok = sprawdz_sasiedztwo(gra, x, y, wspol, war, 'r', coord, win, sy);
				}
				else// reszta lewej skrajnej kolumny
				{
					int war[10] = { -1,1,1,1,0,1,-1,0,1,0 };
					alok = sprawdz_sasiedztwo(gra, x, y, wspol, war, 'k',coord, win, sy);
				}
			}
			else if (x - 1 == 0)// pierwszy wiersz
			{
				if (y + 1 >sy)// prawy gorny rog
				{
					int war[10] = { 1,-1,0,-1,1,0,0,0,0,0 };
					alok = sprawdz_sasiedztwo(gra, x, y, wspol, war, 'r', coord, win, sy);
				}
				else// reszta pierwszego wiersza
				{
					int war[10] = { 1,1,1,-1,0,1,1,0,0,-1 };
					alok = sprawdz_sasiedztwo(gra, x, y, wspol, war, 'k', coord, win, sy);
				}
			}
			else if (x + 1 > sx) // ostatni wiersz
			{
				if (y + 1 > sy)// prawy dolny rog
				{
					int war[10] = { -1,-1,0,-1,-1,0,0,0,0,0 };
					alok = sprawdz_sasiedztwo(gra, x, y, wspol, war, 'r', coord, win, sy);
				}
				else // reszta ostatniego wiersza
				{
					int war[10] = { -1,1,-1,-1,0,1,-1,0,0,-1 };
					alok = sprawdz_sasiedztwo(gra, x, y, wspol, war, 'k', coord, win, sy);
				}
			}
			else if (y + 1 >sy)//reszta prawej skrajnej kolumny
			{
				int war[10] = { -1,-1,1,-1,1,0,-1,0,0,-1 };
				alok = sprawdz_sasiedztwo(gra, x, y, wspol, war, 'k', coord, win, sy);
			}
			else// reszta planszy
			{
				alok = sprawdz_sasiedztwo(gra, x, y, wspol, NULL, 'n', coord, win,  sy);
			}
	}

	if (alok)
	{
		// dekrementacja odpowiednich wartosci w tablicy przechowujacej dopuszczalna liczbe domkow w danym wierszu i kolumnie
		win[y - 1]--;
		win[x + sy - 1]--;
	}
	else
	{
		printf("%s\n", "Nie mozna tak postawic zbiornika!");
		getchar();
		return;
	}
}
/*Funkcja przyjmuje informacje na temat domku do ktorego ma byæ podlaczony zbiornik
*/
int wybierz_domek(int tab[10], struct plansza* gra, int* coord,int* win, int sy)
{
	int i = 0; // licznik okreslaj¹cy indeks elementu tablicy ze wspolrzednymi domkow
	int ok = 0; // zmienna warunkuj¹ca dzialanie petli wprowadzania danych
	int data = 1; // zmienna sprawdzaj¹ca poprawnoœæ danych
	char decision = 'B'; // zmienna przechowujaca decyzjê uzytkwonika
	while (!ok)
	{
		i = 0;
		data = wprowadz_dane(coord);
		if (data)
		{
			while (!((tab[i] == coord[0]) && (tab[i + 1] == coord[1])) && (i <= 6))
			{
				i += 2;
			}
			if ((i <= 6))
			{
				ok = 1;
			}
			else
			{
				printf("%s", "Do tego pola nie mozna podlaczyc zbiornika! Czy chcesz wybrac inne pole?(t/n)");
				scanf("%c", &decision);
				while ((getchar()) != '\n');
				if (decision == 'n')
					return 0;
				else if (decision != 't')
					niepoprawne_dane();
			}
		}
	}
			while ((gra->pole_gry[tab[i]][tab[i+1]].wx != 0) && (gra->pole_gry[tab[i]][tab[i+1]].wy != 0))
			{
				printf("%s\n", "Ten domek ma juz zbiornik! Czy chcesz go zastapic?(t/n)");
				scanf("%c", &decision);
				while ((getchar()) != '\n');
				if (decision == 't')
				{
					ok = 1;
					int usunx = gra->pole_gry[tab[i]][tab[i+1]].wx;
					int usuny = gra->pole_gry[tab[i]][tab[i+1]].wy;
					usun_zbiornik(gra, win, usunx, usuny, sy);
					break;
				}
				else if (decision == 'n')
				{
					printf("%s\n", "Czy chcesz przylaczyc do innego domku?(t/n)");
					scanf("%c", &decision);
					while ((getchar()) != '\n');
					if (decision == 't')
					{
						ok = wybierz_domek(tab, gra, coord,win,sy);
						return ok;
					}
					else if (decision == 'n')
					{
						return 0;
					}
					else
					{
						niepoprawne_dane();
					}
					
				}
				else
				{
					niepoprawne_dane();
				}
			}
		gra->pole_gry[tab[i]][tab[i+1]].wx = tab[8];
		gra->pole_gry[tab[i]][tab[i+1]].wy = tab[9];
		gra->pole_gry[tab[8]][tab[9]].wx = tab[i];
		gra->pole_gry[tab[8]][tab[9]].wy = tab[i+1];
		gra->pole_gry[tab[8]][tab[9]].rodzaj = 2;
		return 1;
}
/*Funkcja umozliwia wprowadzenie danych do dwuelementowej tablicy na calkowite wspolrzedne*/
int wprowadz_dane(int* coord)
{
	int good = 0; // zmienna okreœlajaca poprawnosc danych
	printf("%s\n", "Podaj wspolrzedne domku do ktorego chcesz podlaczyc ten zbiornik (wiersz/kolumna)");
	good = scanf("%d %d", &coord[0], &coord[1]);
	while ((getchar()) != '\n');
	if (good!=2)
	{
		niepoprawne_dane();
		return 0;
	}
	return 1;
}
/*Funkcja przyjmuje wspolrzedne polozenia zbiornika i 
sprawdza czy spelnione sa warunki jego ustawienia.*/
int sprawdz_sasiedztwo(struct plansza* gra, int x, int y,int wspol[10],int war[10],char mode, int* coord,int*win,int sy)
{
	int alok = 0; // zmienna okreslajaca czy udalo sie ustawic domek
	int idk = 0; // zmienna indeksowa dla tablicy wspolrzednych sasiadujacych domkow
	if (mode == 'k') //jesli zbiornik ma zostac ustawiony w skrajnym wierszu lub skrajnej kolumnie (ale nie w narozniku)
	{
		if (!((gra->pole_gry[x + war[0]][y + war[1]].rodzaj == 2) || (gra->pole_gry[x + war[2]][y + war[3]].rodzaj == 2)))// czy nie s¹siaduje zbiornik
		{
			if (gra->pole_gry[x + war[4]][y  +war[5]].rodzaj == 1)// czy sasiaduje dom 
			{
				//odnaleziono sasiadujacy dom; jego wspolrzedne sa zapiywane na odpowiednich miejscach tablicy wspol
				alok = 1;
				wspol[idk] = x + war[4];
				wspol[idk + 1] = y + war[5];
				idk += 2;
			}
			else if (gra->pole_gry[x + war[4]][y + war[5]].rodzaj == 2) // blad - sasiaduje zbiornik
			{
				return 0;
			}
			if (gra->pole_gry[x + war[6]][y + war[7]].rodzaj == 1)
			{
				alok = 1;
				wspol[idk] = x + war[6];
				wspol[idk + 1] = y + war[7];
				idk += 2;
			}
			else if (gra->pole_gry[x + war[6]][y + war[7]].rodzaj == 2)
			{
				return 0;
			}
			if (gra->pole_gry[x + war[8]][y + war[9]].rodzaj == 1)
			{
				alok = 1;
				wspol[idk] = x  + war[8];
				wspol[idk + 1] = y + war[9];
			}
			else if (gra->pole_gry[x + war[8]][y + war[9]].rodzaj == 2)
			{
				return 0;
			}
			if (alok)
				alok= wybierz_domek(wspol, gra, coord, win,sy);
			else
				return 0;
		}
	}
		else if (mode == 'r') //jesli zbiornik ma zostac ustawiony w narozniku
	{
		if (!(gra->pole_gry[x + war[0]][y + war[1]].rodzaj == 2))
		{
			
			if (gra->pole_gry[x + war[2]][y + war[3]].rodzaj == 1) 
			{
				alok = 1;
				wspol[idk] = x + war[2];
				wspol[idk + 1] = y + war[3];
				idk += 2;
			}
			else if (gra->pole_gry[x + war[2]][y + war[3]].rodzaj == 2)
			{
				return 0;
			}
			if (gra->pole_gry[x + war[4]][y + war[5]].rodzaj == 1)
			{
				alok = 1;
				wspol[idk] = x + war[4];
				wspol[idk + 1] = y + war[5];
			}
			else if (gra->pole_gry[x + war[4]][y + war[5]].rodzaj == 2)
			{
				return 0;
			}
			if (alok)
				alok = wybierz_domek(wspol, gra, coord, win,  sy);
			else
				return 0;
		 }
	}
		else if(mode == 'n')//jesli zbiornik ma zostac ustawiony w jednym ze srodkowych pol planszy (nie bêdacych w skrajnym wierszu lub kolumnie)
	{
		if (!((gra->pole_gry[x + 1][y + 1].rodzaj == 2) || (gra->pole_gry[x - 1][y - 1].rodzaj == 2) || (gra->pole_gry[x + 1][y - 1].rodzaj == 2) || (gra->pole_gry[x - 1][y + 1].rodzaj == 2)))
		{
			if (gra->pole_gry[x][y + 1].rodzaj == 1)
			{
				alok = 1;
				wspol[idk] = x;
				wspol[idk + 1] = y + 1;
				idk += 2;
			}
			else if (gra->pole_gry[x][y + 1].rodzaj == 2)
			{
				return 0;
			}
			if (gra->pole_gry[x][y - 1].rodzaj == 1)
			{
				alok = 1;
				wspol[idk] = x;
				wspol[idk + 1] = y - 1;
				idk += 2;
			}
			else if (gra->pole_gry[x][y - 1].rodzaj == 2)
			{
				return 0;
			}
			if (gra->pole_gry[x + 1][y].rodzaj == 1)
			{
				alok = 1;
				wspol[idk] = x + 1;
				wspol[idk + 1] = y;
				idk += 2;
			}
			else if (gra->pole_gry[x + 1][y].rodzaj == 2)
			{
				return 0;
			}
			if (gra->pole_gry[x - 1][y].rodzaj == 1)
			{
				alok = 1;
				wspol[idk] = x - 1;
				wspol[idk + 1] = y;
				idk += 2;
			}
			else if (gra->pole_gry[x - 1][y].rodzaj == 2)
			{
				return 0;
			}
			if (alok)

				alok = wybierz_domek(wspol, gra, coord, win, sy);
			else
				return 0;

		}

	}
	if (alok)
	{
		return 1;
	}
	else return 0;
}
/*Funkcja usuwa zbiornik z odpowiedniego pola planszy*/
void usun_zbiornik(struct plansza*stwor, int* win, int x, int y,int ily)
{
	int dx = 0, dy = 0; // wspolrzedne pola wskazywanego przez zbiornik
	if (stwor->pole_gry[x][y].rodzaj == 2)
	{
		dx = stwor->pole_gry[x][y].wx;
		dy = stwor->pole_gry[x][y].wy;
		stwor->pole_gry[x][y].wx = 0;
		stwor->pole_gry[x][y].wy = 0;
		stwor->pole_gry[x][y].rodzaj = 0;
		stwor->pole_gry[dx][dy].wx = 0;
		stwor->pole_gry[dx][dy].wy = 0;
		win[y - 1]++;
		win[x + ily - 1]++;
	}
	else
	{
		printf("%s\n", "Pomyliles sie, tu nie ma zadnego zbiornika");
		getchar();
	}
}
/*Funkcja wyœwietla standardowa instrukcje*/
void instrukcja()
{
	printf("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n", "1) Przy kazdym domku umiesc zbiornik z gazem (sluzacym do ogrzewania domkow)", "2) Kazdy domek musi byc polaczony ze swoim zbiornikiem jednym z bokow (nie rogiem!)",
		"2) Pola ze zbiornikami nie moga stykac sie ze soba ani bokiem, ani rogiem", "3) Liczby w pierwszym wierszu i pierwszej kolumnie okreslaja ile powinno byc zbiornikow w danym wierszu lub kolumnie",
		"4) Liczby w ostatnim wierszu i ostatniej kolummnie to indeksy pol - przydatne przy podawaniu wspolrzednych",
		"5) Kazda lamiglowka ma dokladnie jedno rozwiazanie!", "6) Na planszy D oznacza domek, Z zbiornik a X puste pole",
		"7) Po kazdym wprowadzeniu danych oraz komunikacie ze strony programu klikinij ENTER",
		"8) By zobaczyc ten informacje jeszcze raz, nacisnij h gdy gra zada ci pytanie \"Co robisz?\"");
	
	getchar();
}
