#include "Header.h"
int main(int argc, char* argv[])
{
	char buff[300];// bufor na nazwê pliku
	// sprawdzenie czy wywo³ano z odpowiednim argumentem - gdy nie - wprowadzenie argumentu
	if (argv[1] == '\0')
	{
			printf("%s", "Podaj sciezke do pliku: ");
			scanf("%s", &buff);
			while ((getchar()) != '\n');
	}
	else strcpy(buff, argv[1]);
	int dzialaj = 1; //zmienna uzyta w petli determinuj¹ca jej przebieg - dzialanie programu
	char comunic; // zmienna sluzaca do przechowywania danych przekazanych przez uzytkownika - podejmowanie decyzji
	int xaxis = 1; // zmienna okreslaj¹ca liczbe wierszy w aktualnej planszy
	int yaxis = 1; // zmienna okreslaj¹ca liczbe kolumn w aktualnej planszy
	int* wygrana = NULL;  // tablica z liczbami domków - okreslanie postêpu w rozgrywce, spe³anianie warunków zwyciêstwa
	int licz_k = 0; // licznik sluzacy do inicjalizacji tablicy z liczbami domkow
	int graj = 1; //zmienna okreslajaca czy gra siê zakonczyla
	int x = 0; // zmienna przechowuj¹ca wspolrzedna wierszowa przekazana przez uzytkownika - zbiornik
	int y = 0; // zmienna przechowujaca wspolrzedna kolumnowa przekazan¹ przez uzytkownika - zbiornik
	int wspol = 1; // zmienna umozliwiajaca czytelne wypisanie wspolrzednych przy planszy
	int coord[2]; // zmienna przechowujaca wspolrzedne kolumnowe przekazane przez uzytkownika - domek
	int proper = 0;// zmienna sprawdzajaca poprawnosc dzialania funckji scanf
	printf("\n%s\n\n%s\n", "Witaj w grze Lamiglowka Architekta!", "Oto zasady gry:");
	instrukcja();
	FILE *plik;//zmienna plikowa
	plik = fopen(buff, "r");
	//glowna petla programu
	while (dzialaj)
	{
		
		struct plansza* nowa = malloc(sizeof(struct plansza));
		// sczytanie informacji z pliku
		if (!utworz_Plansze(plik, &xaxis, &yaxis, nowa))
		{
			printf("%s", "W podanym pliku nie ma poprawnych danych o planszy / plansze skonczyly sie!");
			free(nowa);
			getchar();
			return 0;
		}
		// alokacja tablicy warunkow zwyciêstwa
		wygrana = (int*)malloc((xaxis + yaxis) * sizeof(int));
		while (licz_k < yaxis)
		{
			wygrana[licz_k] = (nowa->pole_gry[0][licz_k+1]).rodzaj;
			licz_k++;
		}
		for (int k = 0; k < xaxis; k++)
		{
			wygrana[k + licz_k] = (nowa->pole_gry[k + 1][0]).rodzaj;
		}
		// g³owna petla rozgrywki
		while (graj)
		{
			wspol = 1;
			system("cls");
			printf("\n");
			//wypisanie tablicy
			for (int m = 0; m <= xaxis; m++)
			{
				for (int n = 0; n <= yaxis; n++)
				{
					if ((n != 0) && (m != 0))
					{
						if ((nowa->pole_gry[m][n]).rodzaj == 1)
							printf(" %c", 'D');
						else if ((nowa->pole_gry[m][n]).rodzaj == 2)
							printf(" %c", 'Z');
						else
							printf(" %c", 'X');
						printf(" ");
					}
					else if ((n == 0) && (m == 0))
					{
						printf("   ");
					}
					else if ((n != 0) && (m == 0))
					{
						printf(" %d", (nowa->pole_gry[m][n]).rodzaj);
						printf(" ");
					}
					else if ((n == 0) && (m != 0))
					{
						printf(" %d", (nowa->pole_gry[m][n]).rodzaj);
						printf(" ");

					}
					else
					{
						printf(" %d", (nowa->pole_gry[m][n]).rodzaj);
					}
				}
				if (m != 0)
					printf(" %d ", m);
				printf("\n\n");
			}
			printf("   ");
			//wypisanie pomocniczych informacji - wspolrzednych
			for (int l = 0; l < yaxis; l++)
			{
				if(wspol<10)
				printf(" %d ", wspol);
				else if (wspol >= 10)
					printf("%d ", wspol);
				wspol++;
			}
			printf("\n\n");
			printf("%s", "Co robisz? (by dodac zbiornik nacisnij a, by usunac zbiornik nacisnij d, by uzyskac dostep do instrukcji nacisnij h, by wyjsc z gry nacisnij e)");
			proper = scanf("%c", &comunic);
			while ((getchar()) != '\n');
			// opcje wywolania instrukcji i niepoprawne
			if (proper == 1)
			{
				if (!((comunic == 'a') || (comunic == 'd')))
				{
					if (comunic == 'h')
					{
						instrukcja();
					}
					else if (comunic == 'e')
					{
						for (int i = 0; i <= xaxis; i++)
						{
							free(nowa->pole_gry[i]);
						}
						free(nowa->pole_gry);
						free(nowa);
						free(wygrana);
						return 0;
					}
					else
					{
						niepoprawne_dane();
					}
					continue;
				}
				// opcja dodania lub usuniecia zbiornika
				printf("%s\n", "Podaj wspolrzedne zbiornika (wiersz/kolumna)");
				proper = scanf("%d %d", &x, &y);
				while ((getchar()) != '\n');
				// jesli podane wspolrzedne wykraczaja poza rozmiar tablicy
				if (!(((x >= 1) && (x <= xaxis)) && ((y >= 1) && (y <= yaxis)) && proper == 2))
				{
					niepoprawne_dane();
					continue;
				}
				// jesli dodajemy
				if (comunic == 'a')
					podaj_wspolrzedne_zbiornika(nowa, x, y, xaxis, yaxis, wygrana, coord);
				// jesli usuwamy
				else if (comunic == 'd')
					usun_zbiornik(nowa, wygrana, x, y, yaxis);
				else printf("%s", "Nie dokonales wyboru!");
				graj = 0;
				for (int l = 0; l < (xaxis + yaxis); l++)
				{
					if (wygrana[l] != 0)
						graj = 1;
				}
			}
			else
			{
				niepoprawne_dane();
			}
			
		}
		// komunikat zwyciêstwa
			printf("%s\n", "Koniec gry! Wygrales!");
			xaxis = 1;
			yaxis = 1;
			licz_k = 0;
			// dealokacja pamieci
			for (int i = 0; i <= xaxis; i++)
			{
				free(nowa->pole_gry[i]);
			}
			free(nowa->pole_gry);
			free(nowa);
			free(wygrana);
			// mo¿liwosc kontynuacji rozgrywki
			printf("%s\n", "Czy chcesz rozwiazac kolejna plansze z pliku?(t/n)");
			scanf("%c", &comunic);
			while ((getchar()) != '\n');
			if (comunic == 't')
			{
				dzialaj = 1;
				graj = 1;
			}
			else if (comunic == 'n')
			{
				dzialaj = 0;
			}
			else
			{
				niepoprawne_dane();
				dzialaj = 0;
			}
		
	}
	fclose(plik);
	getchar();
	return 0;
}