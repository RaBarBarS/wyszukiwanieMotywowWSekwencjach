#include "pch.h"
#include "header.h"
#include <string>
#include <fstream>
#include <iostream>

vector <vertex> vertexesList;
vector <vector <int>> nextList;
vector <string> seqList;
vector <int> maxMotyw;	//prawo,lewo,wierzcho�ki kt�re nale��
int window;	int deletion;
int vertexID = 0;
int seqNumber;
int minimumSekwencji = 4;
int dlsekw = 100;
//int skadnumerki = 0;


void setWindow(int a) {
	window = a;
}

void setDeletion(int a) {
	deletion = a;
}

void read() {
	ifstream test, test2;
	string seq, numbers;
	//string helpLine;
	test.open("plik5-1.txt", ios::in);
	if (test.good() == false)
		cout << "Blad otwierania pliku" << endl;
	else {
		test2.open("plik5-2.txt", ios::in);
		if (test2.good() == false)
			cout << "Blad otwierania pliku" << endl;
		else {
			while (getline(test, seq) && getline(test2, numbers)) {
				if (seq == "\n") {
					getline(test, seq);
					getline(test2, numbers);
				}
				if (seq[0] == '>') {
					//wczytanie kolejnej linii, bo kolejna to to co potrzebne
					getline(test, seq);
					getline(test2, numbers);
					seqList.push_back(seq);

					addingSequence(seq, numbers);
					seqNumber++;
				}
			}
		}
	}
}

void addingSequence(string sequence, string numbers) {
	int skadnumerki = 0;
	for (int i = 0; i < dlsekw - window + 1; i++) {	//domy�lnie <100!!!
		vertex*object = new vertex;
		object->sequence.insert(0, sequence, i, window);//sprawdz co robi kt�ry parametr, zeby nei wywali�o b��du
		object->id = vertexID++;
		object->sequenceNumer = seqNumber;
		object->sequencePosition = i;
		int pos = skadnumerki;
		string halp; 
		for (int j = i; j < i + window; j++) {
			int found = numbers.find(' ', pos);//b��d mo�e by� z '/"
			halp.insert(0, numbers, pos, found - pos);//(nie wiem, sk�d, pozycja startu, ile skopiowa�) 
			pos = found + 1;
			object->tab.push_back(atoi(halp.c_str()));//kolejne warto��i wiarygodno�ci w wektorze
			halp.clear();
			if (j == i) {
				skadnumerki = found + 1;
			}

			object->tab2.push_back(true);//�eby nei pisa� kolejnego fora
		}
		vertexesList.push_back(*object);
	}
}

void buildGraph() {
	for (int i = 0; i < vertexesList.size(); i++) {
		vector<int>container;
		for (int j = 0; j < vertexesList.size(); j++) {
			if (vertexesList[i].sequenceNumer == vertexesList[j].sequenceNumer) {
				continue;
			}
			else if (vertexesList[i].sequence == vertexesList[j].sequence) {
				container.push_back(j);
			}
		}
		nextList.push_back(container);
	}
}

void addDeletions() {
	int howMuch = 0;
	int max = 0;
	if (window == 4 || window == 5)max = 1;
	else max = 2;
	for (int i = 0; i < vertexesList.size(); i++) {
		for (int j = 0; j < vertexesList[i].sequence.length(); j++) {
			if (vertexesList[i].tab[j] <= deletion) {
				vertexesList[i].sequence.erase(vertexesList[i].sequence.begin() + j);
 				vertexesList[i].tab2[j + howMuch] = false;
				howMuch++;
				j--;
				if (howMuch == max)break;
			}
		}
		howMuch = 0;
	}
}

void buildGraph2() {//z uwzgl�dnieniem delecji (dopisuje kraw�dzie tylko miedzy podobnymi wierzcho�kami)
	addDeletions();
	for (int i = 0; i < vertexesList.size(); i++) {
		for (int j = i; j < vertexesList.size(); j++) {//bo po co sprawdza� te same wierzcho�ki dwa razy
			if (vertexesList[i].sequence.length() == window && vertexesList[j].sequence.length()== window) {
				continue;//bo je�li ma byc mi�dzy nimi kraw� to ju� zosta�a dodana
			}
			else if ((vertexesList[i].sequence.find(vertexesList[j].sequence) != string::npos || vertexesList[j].sequence.find(vertexesList[i].sequence) != string::npos)&&vertexesList[i].sequenceNumer!=vertexesList[j].sequenceNumer) {
				//je�li jeden wierzcho�ek zawiera sie w drugim i s� z r�nych sekwencji
				vector<int>::iterator it;
				it = find(nextList[i].begin(), nextList[i].end(), j);
				if (it == nextList[i].end()) {	//spr czy ten by�o dodane czy nie
					nextList[i].push_back(j);
					nextList[j].push_back(i);
				}
			}
		}
	}
}

/*
void makeItBigger() {
	for (int i = 0; i < nextList.size(); i++) {
		if (nextList[i].size() >= minimumSekwencji) {//ponad po�owa sekwencji
			int liczbaSekwencji = 0;
			int*sekwencje = new int[seqNumber];
			vector<int>klika;
			for (int j = 0; j < seqNumber; j++) {
				sekwencje[j] = 0;
			}

			for (int j = 0; j < nextList[i].size(); j++) {
				vector<int>::iterator it;
				it = find(klika.begin(), klika.end(), nextList[i][j]);
				if (it != klika.end()) {	//spr czy ten by�o dodane czy nie
					continue;	//jak tak to 
				}
				else {
					klika.push_back(nextList[i][j]);
					sekwencje[vertexesList[nextList[i][j]].sequenceNumer]++;	//zwi�ksza ilo�� wyst�pie� motywu w sekwencji o danym indeksie
					if (sekwencje[vertexesList[nextList[i][j]].sequenceNumer] == 1)	//je�li pierwszy raz wyst�pi ta sekwencja to zwi�ksz ilo�� sekwencji branych pod uwag�
						liczbaSekwencji++;
				}
			}
			for (int j = 0; j < nextList[nextList[i][0]].size(); j++) {
				vector<int>::iterator it;
				it = find(klika.begin(), klika.end(), nextList[nextList[i][0]][j]);
				if (it != klika.end()) {	//spr czy ten by�o dodane czy nie
					continue;	//jak tak to 
				}
				else {
					klika.push_back(nextList[nextList[i][0]][j]);
					sekwencje[vertexesList[nextList[nextList[i][0]][j]].sequenceNumer]++;	//zwi�ksza ilo�� wyst�pie� motywu w sekwencji o danym indeksie
					if (sekwencje[vertexesList[nextList[nextList[i][0]][j]].sequenceNumer] == 1)	//je�li pierwszy raz wyst�pi ta sekwencja to zwi�ksz ilo�� sekwencji branych pod uwag�
						liczbaSekwencji++;
				}
			}
			if (liczbaSekwencji < minimumSekwencji)//ponad polowa sekwecji
				continue;
			if (maxMotyw.size() == 0) {
				maxMotyw.push_back(0); maxMotyw.push_back(0);
				for (int c = 0; c < klika.size(); c++) {
					maxMotyw.push_back(klika[c]);
				}
			}
			/////////////////////struktury przygotowan, teraz rozszerzanko///////////////////
			bool mogeRozszerzac = true, rozpraw = true;
			int prawo = 0, lewo = 0;	//przechowuj� warto�ci o ile zosta�a rozszerzony motyw w kt�r� stron�

			//rozszerza� patrz�c nie czy nukleotydy s� takiem same tylko czy nale�� do jakiej� wsp�lnej kliki

			while (mogeRozszerzac) {
				if (i + prawo + 1 < vertexID && vertexesList[i + prawo + 1].sequenceNumer == vertexesList[i].sequenceNumer && vertexesList[i + prawo + 1].tab2[vertexesList[i + prawo + 1].tab2.size() - 1] == true && rozpraw) {
					//je�li kolejny wierzcho�ek nalezy wci�� do tej samej sekwencji i istnieje wgl taki wierzcho�ek oraz element kt�ry chcemy rozpatrywa� nie uleg� delecji
					char znak = vertexesList[i + prawo + 1].sequence[vertexesList[i + prawo + 1].sequence.size() - 1];	//zawsze bierzemy ostatni znak kolejnego okna �eby nie walczy� z ostatnim oknem
					for (int j = 0; j < klika.size(); j++) {
						if (klika[j] + prawo + 1 < vertexID && vertexesList[klika[j] + prawo + 1].sequenceNumer == vertexesList[klika[j]].sequenceNumer && vertexesList[klika[j] + prawo + 1].tab2[vertexesList[klika[j] + prawo + 1].tab2.size() - 1] == true) {
							if (vertexesList[klika[j] + prawo + 1].sequence[vertexesList[klika[j] + prawo + 1].sequence.size() - 1] != znak) {
								if (sekwencje[vertexesList[klika[j] + prawo + 1].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
									rozpraw = false;
									prawo--;
									break;
								}
								else {
									if (sekwencje[vertexesList[klika[j] + prawo + 1].sequenceNumer] == 1) {
										liczbaSekwencji--;
									}
									sekwencje[vertexesList[klika[j] + prawo + 1].sequenceNumer]--;
									klika.erase(klika.begin() + j);
									j--;	//bo przesuni�cie indeks�w w vectorze
								}
							}
						}
						else if (klika[j] + prawo + 1 < vertexID && vertexesList[klika[j] + prawo + 1].sequenceNumer == vertexesList[klika[j]].sequenceNumer && vertexesList[klika[j] + prawo + 1].tab2[vertexesList[klika[j] + prawo + 1].tab2.size() - 1] == false) {
							continue;//jak warunki spe�nion to fajno 
						}
						else {//nie mo�na ju� dalej rozszerza�
							rozpraw = false;
							prawo--;
							break;
						}
					}
					prawo++;
				}
				else if (i + prawo + 1 < vertexID && vertexesList[i + prawo + 1].sequenceNumer == vertexesList[i].sequenceNumer && vertexesList[i + prawo + 1].tab2[vertexesList[i + prawo + 1].tab2.size() - 1] == false && rozpraw) {
					for (int j = 0; j < klika.size(); j++) {
						if (klika[j] + prawo + 1 > vertexID) {
							if (klika[j] + prawo + 1 < vertexID && sekwencje[vertexesList[klika[j] + prawo].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
								rozpraw = false;
								prawo--;
								break;
							}
							else {
								if (sekwencje[vertexesList[klika[j]].sequenceNumer] == 1) {
									liczbaSekwencji--;
								}
								sekwencje[vertexesList[klika[j]].sequenceNumer]--;
								klika.erase(klika.begin() + j);
								j--;	//bo przesuni�cie indeks�w w vectorze
							}
						}
						else if (klika[j] + prawo + 1 < vertexID && vertexesList[klika[j] + prawo + 1].sequenceNumer != vertexesList[klika[j]].sequenceNumer) {
							if (sekwencje[vertexesList[klika[j] + prawo + 1].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
								rozpraw = false;
								prawo--;
								break;
							}
							else {
								if (sekwencje[vertexesList[klika[j] + prawo + 1].sequenceNumer] == 1) {
									liczbaSekwencji--;
								}
								sekwencje[vertexesList[klika[j] + prawo + 1].sequenceNumer]--;
								klika.erase(klika.begin() + j);
								j--;	//bo przesuni�cie indeks�w w vectorze
							}
						}
					}
					prawo++;
				}
				else {
					//bool rozszerzanie = true;
					//rozszerzanko w lewo
					if (i - lewo - 1 >= 0 && vertexesList[i - lewo - 1].sequenceNumer == vertexesList[i].sequenceNumer && vertexesList[i - lewo - 1].tab2[0] == true) {
						//je�li kolejny wierzcho�ek nalezy wci�� do tej samej sekwencji i istnieje wgl taki wierzcho�ek oraz element kt�ry chcemy rozpatrywa� nie uleg� delecji
						char znak = vertexesList[i - lewo - 1].sequence[0];	//zawsze bierzemy ostatni znak kolejnego okna �eby nie walczy� z ostatnim oknem
						for (int j = 0; j < klika.size(); j++) {
							if (klika[j] - lewo - 1 >= 0 && vertexesList[klika[j] - lewo - 1].sequenceNumer == vertexesList[klika[j]].sequenceNumer && vertexesList[klika[j] - lewo - 1].tab2[0] == true) {
								if (vertexesList[klika[j] - lewo - 1].sequence[0] != znak) {//WYWALA B��D NIE WIEM CZEMU
									if (sekwencje[vertexesList[klika[j] - lewo - 1].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
										mogeRozszerzac = false;
										lewo--;
										break;
									}
									else {
										if (sekwencje[vertexesList[klika[j] - lewo - 1].sequenceNumer] == 1) {
											liczbaSekwencji--;
										}
										sekwencje[vertexesList[klika[j] - lewo - 1].sequenceNumer]--;
										klika.erase(klika.begin() + j);
										j--;	//bo przesuni�cie indeks�w w vectorze
									}
								}
							}
							else if (klika[j] - lewo - 1 >= 0 && vertexesList[klika[j] - lewo - 1].sequenceNumer == vertexesList[klika[j]].sequenceNumer && vertexesList[klika[j] - lewo - 1].tab2[0] == false) {
								continue;//jak warunki spe�nion to fajno 
							}
							else {//nie mo�na ju� dalej rozszerza�
								mogeRozszerzac = false;
								lewo--;
								break;
							}
						}
						lewo++;
					}
					else if (i - lewo - 1 >= 0 && vertexesList[i - lewo - 1].sequenceNumer == vertexesList[i].sequenceNumer && vertexesList[i - lewo - 1].tab2[0] == false) {
						for (int j = 0; j < klika.size(); j++) {
							if (klika[j] - lewo - 1 < 0) {
								if (sekwencje[vertexesList[klika[j] - lewo].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
									mogeRozszerzac = false;
									lewo--;
									break;
								}
								else {
									if (sekwencje[vertexesList[klika[j] - lewo].sequenceNumer] == 1) {
										liczbaSekwencji--;
									}
									sekwencje[vertexesList[klika[j] - lewo].sequenceNumer]--;
									klika.erase(klika.begin() + j);
									j--;	//bo przesuni�cie indeks�w w vectorze
								}
							}
							else if (vertexesList[klika[j] - lewo - 1].sequenceNumer != vertexesList[klika[j]].sequenceNumer) {
								if (sekwencje[vertexesList[klika[j] - lewo - 1].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
									mogeRozszerzac = false;
									lewo--;
									break;
								}
								else {
									if (sekwencje[vertexesList[klika[j] - lewo - 1].sequenceNumer] == 1) {
										liczbaSekwencji--;
									}
									sekwencje[vertexesList[klika[j] - lewo - 1].sequenceNumer]--;
									klika.erase(klika.begin() + j);
									j--;	//bo przesuni�cie indeks�w w vectorze
								}
							}
						}
						lewo++;
					}
					else
						mogeRozszerzac = false;
				}
			}
			//por�wnaie z maksymaln� jak dot�d
			if (maxMotyw.size() == 0) {
				maxMotyw.push_back(prawo);
				maxMotyw.push_back(lewo);
				for (int j = 0; j < klika.size(); j++) {
					maxMotyw.push_back(klika[j]);//wpisanie w vector wierzcho�k�w z naszej super d�ugiego motywu
				}
			}
			else if (maxMotyw[0] + maxMotyw[1] < lewo + prawo) {
					maxMotyw.clear();
					maxMotyw.push_back(prawo);
					maxMotyw.push_back(lewo);
					for (int j = 0; j < klika.size(); j++) {
						maxMotyw.push_back(klika[j]);//wpisanie w vector wierzcho�k�w z naszej super d�ugiego motywu
					}
				
			}
		}
	}
}
*/


void makeItBiggerBetter() {
	for (int i = 0; i < nextList.size(); i++) {
		int lewo = 0, prawo = 0; bool rozpraw = true, mogeRozszerzac = true;
		if (nextList[i].size() >= minimumSekwencji) {//ponad po�owa sekwencji
			int liczbaSekwencji = 0;
			int*sekwencje = new int[seqNumber];
			vector<int>klika;
			for (int j = 0; j < seqNumber; j++) {
				sekwencje[j] = 0;
			}

			for (int j = 0; j < nextList[i].size(); j++) {
				vector<int>::iterator it;
				it = find(klika.begin(), klika.end(), nextList[i][j]);
				if (it != klika.end()) {	//spr czy ten by�o dodane czy nie
					continue;	//jak tak to 
				}
				else {
					klika.push_back(nextList[i][j]);
					sekwencje[vertexesList[nextList[i][j]].sequenceNumer]++;	//zwi�ksza ilo�� wyst�pie� motywu w sekwencji o danym indeksie
					if (sekwencje[vertexesList[nextList[i][j]].sequenceNumer] == 1)	//je�li pierwszy raz wyst�pi ta sekwencja to zwi�ksz ilo�� sekwencji branych pod uwag�
						liczbaSekwencji++;
				}
			}
			for (int j = 0; j < nextList[nextList[i][0]].size(); j++) {
				vector<int>::iterator it;
				it = find(klika.begin(), klika.end(), nextList[nextList[i][0]][j]);
				if (it != klika.end()) {	//spr czy ten by�o dodane czy nie
					continue;	//jak tak to 
				}
				else {
					klika.push_back(nextList[nextList[i][0]][j]);
					sekwencje[vertexesList[nextList[nextList[i][0]][j]].sequenceNumer]++;	//zwi�ksza ilo�� wyst�pie� motywu w sekwencji o danym indeksie
					if (sekwencje[vertexesList[nextList[nextList[i][0]][j]].sequenceNumer] == 1)	//je�li pierwszy raz wyst�pi ta sekwencja to zwi�ksz ilo�� sekwencji branych pod uwag�
						liczbaSekwencji++;
				}
			}
			if (liczbaSekwencji < minimumSekwencji)//ponad polowa sekwecji
				continue;
			if (maxMotyw.size() == 0) {
				maxMotyw.push_back(0); maxMotyw.push_back(0);
				for (int c = 0; c < klika.size(); c++) {	//c++ �mieszne
					maxMotyw.push_back(klika[c]);
				}
			}
			/////strukturki zrobion////rozszerzanie w�a�ciwe
			while (mogeRozszerzac) {
				if (i + prawo + 1 < vertexID && vertexesList[i].sequenceNumer == vertexesList[i + prawo + 1].sequenceNumer && rozpraw) {
 					vector<int>klikaHalp;
					if (nextList[i + prawo + 1].size() >= minimumSekwencji - 1) {	//czyli nale�y do jakiej� kilki i to mo�liwe �e obejmuj�cej potzrebne rzeczy
						for (int j = 0; j < nextList[i + prawo + 1].size(); j++) {
							vector<int>::iterator it;
							it = find(klikaHalp.begin(), klikaHalp.end(), nextList[i + prawo + 1][j]);
							if (it != klikaHalp.end()) {	//spr czy ten by�o dodane czy nie
								continue;	//jak tak to 
							}
							else {
								klikaHalp.push_back(nextList[i + prawo + 1][j]);
							}
						}
						for (int j = 0; j < nextList[nextList[i + prawo + 1][0]].size(); j++) {
							vector<int>::iterator it;
							it = find(klikaHalp.begin(), klikaHalp.end(), nextList[nextList[i + prawo + 1][0]][j]);
							if (it != klikaHalp.end()) {	//spr czy ten by�o dodane czy nie
								continue;	//jak tak to 
							}
							else {
								klikaHalp.push_back(nextList[nextList[i + prawo + 1][0]][j]);
							}
						}
						//klika pomocnicza zrobion
					}
					else {//skoro nie nale�y do �aadnej kliki to nie b�dzie nale�a� do wsp�lnej z innymi
						rozpraw = false;
						prawo--;
					}
					if (rozpraw) {
						for (int j = 0; j < klika.size(); j++) {
							if (klika[j] != i && vertexesList[klika[j]].id + prawo + 1 < vertexID && vertexesList[klika[j] + prawo + 1].sequenceNumer == vertexesList[klika[j]].sequenceNumer) {
								vector<int>::iterator it;
								it = find(klikaHalp.begin(), klikaHalp.end(), vertexesList[klika[j] + prawo + 1].id);
									if (it != klikaHalp.end()) {	//spr czy nale�y do kliki
								continue;	//jak tak to sprawd� kolejny
								}
								else {
									if (sekwencje[vertexesList[klika[j] + prawo].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
										rozpraw = false;
										prawo--;	//bo nie mo�e by� rozszerzone o t� pozycj�
										break;
									}
									else {
										if (sekwencje[vertexesList[klika[j]].sequenceNumer] == 1) {
											liczbaSekwencji--;
										}
										sekwencje[vertexesList[klika[j]].sequenceNumer]--;
										klika.erase(klika.begin() + j);
										j--;	//bo przesuni�cie indeks�w w vectorze
									}
								}
							}
							else if(klika[j] != i || (klika[j] == i && (vertexesList[klika[j]].id + prawo + 1 >= vertexID || vertexesList[klika[j] + prawo + 1].sequenceNumer != vertexesList[klika[j]].sequenceNumer))){	//gdy wychodzimy poza sekwencj� lub zakres indeks�w
								if (sekwencje[vertexesList[klika[j] + prawo].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
									rozpraw = false;
									prawo--;	//bo nie mo�e by� rozszerzone o t� pozycj�
									break;
								}
								else {
									if (sekwencje[vertexesList[klika[j]].sequenceNumer] == 1) {
										liczbaSekwencji--;
									}
									sekwencje[vertexesList[klika[j]].sequenceNumer]--;
									klika.erase(klika.begin() + j);
									j--;	//bo przesuni�cie indeks�w w vectorze
								}
							}
						}
						prawo++;
					}
				}
				else {
					//rozszerzanko w lewo
					if (i - lewo - 1 > 0 && vertexesList[i].sequenceNumer == vertexesList[i - lewo - 1].sequenceNumer) {
						vector<int>klikaHalp;
						if (nextList[i - lewo - 1].size() >= minimumSekwencji - 1) {	//czyli nale�y do jakiej� kilki i to mo�liwe �e obejmuj�cej potzrebne rzeczy
							for (int j = 0; j < nextList[i - lewo - 1].size(); j++) {
								vector<int>::iterator it;
								it = find(klikaHalp.begin(), klikaHalp.end(), nextList[i - lewo - 1][j]);
								if (it != klikaHalp.end()) {	//spr czy ten by�o dodane czy nie
									continue;	//jak tak to 
								}
								else {
									klikaHalp.push_back(nextList[i - lewo - 1][j]);
								}
							}
							for (int j = 0; j < nextList[nextList[i - lewo - 1][0]].size(); j++) {
								vector<int>::iterator it;
								it = find(klikaHalp.begin(), klikaHalp.end(), nextList[nextList[i - lewo - 1][0]][j]);
								if (it != klikaHalp.end()) {	//spr czy ten by�o dodane czy nie
									continue;	//jak tak to 
								}
								else {
									klikaHalp.push_back(nextList[nextList[i - lewo - 1][0]][j]);
								}
							}
							//klika pomocnicza zrobion
						}
						else {//skoro nie nale�y do �aadnej kliki to nie b�dzie nale�a� do wsp�lnej z innymi
							break;
						}
						for (int j = 0; j < klika.size(); j++) {
							if (klika[j] != i && vertexesList[klika[j]].id - lewo - 1 > 0 && vertexesList[klika[j] - lewo - 1].sequenceNumer == vertexesList[klika[j]].sequenceNumer) {
								vector<int>::iterator it;
								it = find(klikaHalp.begin(), klikaHalp.end(), vertexesList[klika[j] - lewo - 1].id);
								if (it != klikaHalp.end()) {	//spr czy nale�y do kliki
									continue;	//jak tak to sprawd� kolejny
								}
								else {
									if (sekwencje[vertexesList[klika[j] - lewo].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
										mogeRozszerzac = false;
										lewo--;	//bo nie mo�e by� rozszerzone o t� pozycj�
										break;
									}
									else {
										if (sekwencje[vertexesList[klika[j]].sequenceNumer] == 1) {
											liczbaSekwencji--;
										}
										sekwencje[vertexesList[klika[j]].sequenceNumer]--;
										klika.erase(klika.begin() + j);
										j--;	//bo przesuni�cie indeks�w w vectorze
									}
								}
							}
							else {
								if (sekwencje[vertexesList[klika[j] - lewo].sequenceNumer] - 1 <= 0 && liczbaSekwencji - 1 < minimumSekwencji) {//je�li zmniejszenie kliki o kolejn� sekwencj� zniszczy�by za�o�enia rozszerzaj w lewo
									mogeRozszerzac = false;
									lewo--;	//bo nie mo�e by� rozszerzone o t� pozycj�
									break;
								}
								else {
									if (sekwencje[vertexesList[klika[j]].sequenceNumer] == 1) {
										liczbaSekwencji--;
									}
									sekwencje[vertexesList[klika[j]].sequenceNumer]--;
									klika.erase(klika.begin() + j);
									j--;	//bo przesuni�cie indeks�w w vectorze
								}
							}
						}
						lewo++;
					}
					else {
						mogeRozszerzac = false;
					}
				}
			}
			//por�wnaie z maksymaln� jak dot�d
			if (maxMotyw[0] + maxMotyw[1] < lewo + prawo) {
				maxMotyw.clear();
				maxMotyw.push_back(prawo);
				maxMotyw.push_back(lewo);
				for (int j = 0; j < klika.size(); j++) {
					maxMotyw.push_back(klika[j]);//wpisanie w vector wierzcho�k�w z naszej super d�ugiego motywu
				}

			}
		}
	}
}


void outputResults() {
	if (maxMotyw.size() == 0) {
		cout << "UPS! COS POSZLO NIE TAK, ZADNA KLIKA NIE ZOSTALA ZNALEZIONA" << endl;
	}
	else {

		vector<vector<int>>sekwencje;
		for (int j = 0; j < seqNumber; j++) {
			vector<int>nieWazne;
			sekwencje.push_back(nieWazne);
		}

		for (int i = 2; i < maxMotyw.size(); i++) {	//i=2 bo dwie pierwsze liczby to prawo/lewo
			sekwencje[vertexesList[maxMotyw[i]].sequenceNumer].push_back(maxMotyw[i]);
		}
		/*for (int i = 0; i < maxMotyw.size(); i++) {
			cout << maxMotyw[i] << " ";
		}*/
		cout << endl;
		for (int i = 0; i < seqNumber; i++) {
			if (sekwencje[i].size() > 0) {
				cout << "SEKWENCJA " << i << ":" << endl;
				for (int j = 0; j < sekwencje[i].size(); j++) {
					int prawo = 0;
					int lewo = maxMotyw[1];
					int okno = 0;
				cout << "\tPOZYCJA " << (vertexesList[sekwencje[i][j]].sequencePosition - lewo) << ":";
					while (lewo > 0) {
						cout << seqList[vertexesList[sekwencje[i][j]].sequenceNumer][vertexesList[sekwencje[i][j] - lewo].sequencePosition];
						lewo--;
					}
					while (okno < window) {
						cout << seqList[vertexesList[sekwencje[i][j]].sequenceNumer][vertexesList[sekwencje[i][j]].sequencePosition + okno];
						okno++;
					}
					while (prawo < maxMotyw[0]) {
						cout << seqList[vertexesList[sekwencje[i][j]].sequenceNumer][vertexesList[sekwencje[i][j] + prawo].sequencePosition + window];
						prawo++;
					}
					cout << endl;
				}
			}
		}
		/*for (int i = 2; i < maxMotyw.size(); i++) {
			int prawo = 0;
			int lewo = maxMotyw[1];
			int okno = 0;
			cout << maxMotyw[i] << ": ";
				while (lewo > 0) {
					cout << vertexesList[maxMotyw[i] - lewo].sequence[0];
					lewo--;
				}
				while (okno < window) {
					cout << seqList[vertexesList[maxMotyw[i]].sequenceNumer][vertexesList[maxMotyw[i]].sequencePosition + okno];
					okno++;
				}
				while (prawo < maxMotyw[0]) {
					cout << vertexesList[maxMotyw[i] + prawo].sequence[0];
					prawo++;
				}
				cout << endl;
		}*/
		//wypisz wszystkie kombinacje

	}
}

