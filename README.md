Należy zaimplementować algorytm heurystyczny o złożoności wielomianowej realizujący następujące czynności:
− wczytanie instancji;
− utworzenie grafu z wierzchołkami odpowiadającymi wszystkim kilkuliterowym podciągom sekwencji z instancji (długość podciągów z zakresu od 4 do 7 liter ma być parametrem ustawianym przez użytkownika); każde wystąpienie tego samego podciągu to osobny wierzchołek w grafie, dla każdego wierzchołka należy zapamiętać numer jego sekwencji źródłowej oraz pozycję wewnątrz tej sekwencji;
− połączenie wierzchołków nieskierowanymi krawędziami, jeśli odpowiadają one takim samym podciągom występującym w różnych sekwencjach, lub podciągom prawie identycznym (jeden z nich zawiera się w drugim) po dopuszczeniu błędu typu delecja w przypadku nukleotydu o ocenie wiarygodności poniżej pewnego założonego poziomu (poziom ten ma być drugim parametrem ustawianym przez użytkownika);
− wyszukanie w grafie w sposób heurystyczny serii klik (lub struktur zbliżonych do klik) odpowiadających najdłuższemu lokalnemu dopasowaniu występującemu w większości z sekwencji w instancji; elementem tej procedury może być wyszukiwanie w sposób dokładny klik o maksymalnie 4 wierzchołkach; na ocenę dostateczną wystarczy wyszukać w sposób heurystyczny jedną największą strukturę zbliżoną do kliki;
− wypisanie rezultatu na wyjściu w sposób czytelny dla użytkownika. 

instancje wejsciowe: zobacz pliki: plik2-1.txt i plik2-2.txt
