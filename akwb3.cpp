// akwb3.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include "header.h"
#include <Windows.h>
#include <string>
#include <iomanip>
#include <vector>
using namespace std;


int main()
{
	int window;
	cout << "ZNAJDZMY MOTYWY W TWOICH SEKWENCJACH!" << endl << endl;
	cout << "podaj szerokosc okna (4-7):";
	cin >> window;

	while (window > 7 || window < 4) {
		cout << "szerokosc okna musi sie miescic miedzy 4 a 7 \npodaj jeszcze raz:";
		cin >> window;
	}
	setWindow(window);
	cout << endl;

	window = 0;
	cout << "ponizej jakiej wiarygodnosci nie wierzysz nukleotydowi?:";
	cin >> window;

	while (window > 40 || window < 0) {
		cout << "wartosc musi sie miescic miedzy 0 a 40 \npodaj jeszcze raz:";
		cin >> window;
	}
	setDeletion(window);
	cout << endl;

	read();
	buildGraph();
	buildGraph2();
	makeItBiggerBetter();
	outputResults();

	

	system("Pause");
	return 0;
}
// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
