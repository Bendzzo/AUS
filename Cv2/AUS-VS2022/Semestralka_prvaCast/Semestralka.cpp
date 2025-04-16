#include "Zastavka.h"
#include "FilterAlgorithm.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include "Semestralka.h"

void Semestralka::vypisVsetkyZastavky(const vector<Zastavka>& zastavkyNaVypis) {
	for (const auto& zastavka : zastavkyNaVypis) {
		zastavka.vypis();
	}
	cout << "Celkovy pocet zastavok: " << zastavkyNaVypis.size() << endl;
}

void Semestralka::nacitajVsetkyZastavky(string nazovSuboru)
{
    koren_ = &this->hierarchiaZastavok.emplaceRoot();
    BlokHierarchie* obec = nullptr;

    ifstream subor;
    subor.open(nazovSuboru);
    string line;

    getline(subor, line);

    int riadok = 0;
    while (getline(subor, line))
    {
        Zastavka zastavka;
        string tempString;
        stringstream inputString(line);

        // StopID
        getline(inputString, tempString, ';');
        zastavka.id = stoi(tempString);

        // Street
        getline(inputString, zastavka.ulica, ';');

        // Longitude
        getline(inputString, tempString, ';');
        if (!tempString.empty())
        {
            zastavka.longitude = stod(tempString);

        }
        else
        {
            //zastavka.vypis();
            zastavka.longitude = 1000000.0;
        }

        // Latitude
        tempString = "";
        getline(inputString, tempString, ';');
        if (!tempString.empty())
        {
            zastavka.latitude = stod(tempString);

        }
        else
        {
            //zastavka.vypis();
            zastavka.latitude = 1000000.0;
        }

        // Municipality
        getline(inputString, zastavka.obec);

        zastavky.push_back(zastavka);


        /*cout << riadok++ << " ";
        zastavka.vypis();*/

    }
    subor.close();
}


void Semestralka::zobrazMenuPrvaCast() {
    FilterAlgorithm<vector<Zastavka>::iterator, vector<Zastavka>> filter;

    int volba;
    string vstup;

    while (true) {
        cout << "========== MENU ==========" << endl;
        cout << "1. Filtrovat podla obce" << endl;
        cout << "2. Filtrovat podla ulice" << endl;
        cout << "3. Filtrovat podla geografickej oblasti" << endl;
        cout << "4. Vypisat vsetky zastavky" << endl;
        cout << "0. Koniec" << endl;
        cout << "==========================" << endl;
        cout << "Zadajte cislo volby: ";
        cin >> volba;

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (volba) {
        case 0:
            return;

        case 1: {
            cout << "Zadajte nazov obce: ";
            getline(cin, vstup);

            //isInMunicipality funkcia
            auto municipalityPredicate = [vstup](const Zastavka& z) -> bool {
                return z.obec == vstup;
                };

            filter.filter(zastavky.begin(), zastavky.end(), filtrovaneZastavky, municipalityPredicate);

            cout << "Zastavky v obci " << vstup << ":" << endl;
            vypisVsetkyZastavky(filtrovaneZastavky);
            break;
        }

        case 2: {
            cout << "Zadajte nazov ulice: ";
            getline(cin, vstup);

            //isOnStreet funkcia
            auto streetPredicate = [vstup](const Zastavka& z) -> bool {
                return z.ulica.find(vstup) != string::npos;
                };

            filter.filter(zastavky.begin(), zastavky.end(), filtrovaneZastavky, streetPredicate);

            cout << "Zastavky na ulici " << vstup << ":" << endl;
            vypisVsetkyZastavky(filtrovaneZastavky);
            break;
        }

        case 3: {
            double minLat, maxLat, minLong, maxLong;
            cout << "Zadajte minimálnu zemepisnú šírku (latitude): ";
            cin >> minLat;
            cout << "Zadajte maximálnu zemepisnú šírku (latitude): ";
            cin >> maxLat;
            cout << "Zadajte minimálnu zemepisnú dĺžku (longitude): ";
            cin >> minLong;
            cout << "Zadajte maximálnu zemepisnú dĺžku (longitude): ";
            cin >> maxLong;

            //isInRegion funkcia
            auto regionPredicate = [minLat, maxLat, minLong, maxLong](const Zastavka& z) -> bool {
                return z.latitude >= minLat && z.latitude <= maxLat &&
                    z.longitude >= minLong && z.longitude <= maxLong;
                };

            filter.filter(zastavky.begin(), zastavky.end(), filtrovaneZastavky, regionPredicate);

            cout << "Zastavky v zadanej geografickej oblasti:" << endl;
            vypisVsetkyZastavky(filtrovaneZastavky);
            break;
        }

        case 4:
            cout << "Všetky zastavky:" << endl;
            vypisVsetkyZastavky(zastavky);
            break;

        default:
            cout << "Neplatná voľba. Skúste znova." << endl;
        }


        cout << endl;
    }
}

void Semestralka::zobrazMenuDruhaCast()
{
    HierarchiaZastavok hierarchiaZastavok;
    hierarchiaZastavok.vytvorHierarchiu(zastavky);

    // Vytvor iterátor nad hierarchiou
    HierarchiaIterator iterator(&hierarchiaZastavok);

    int volba;
    std::string vstup;

    while (true) {
        std::cout << "\n========== MENU DRUHEJ ÚROVNE ==========" << std::endl;
        std::cout << "Aktuálna pozícia: " << iterator.getAktualnyVrchol().getNazov() << std::endl;
        std::cout << "Typ: ";

        if (iterator.getAktualnyVrchol().jeKoren()) {
            std::cout << "Dopravca (koreň)" << std::endl;
        }
        else if (iterator.getAktualnyVrchol().jeObec()) {
            std::cout << "Obec" << std::endl;
        }
        else if (iterator.getAktualnyVrchol().jeUlica()) {
            std::cout << "Ulica" << std::endl;
        }

        std::cout << "1. Prejsť na nadradenú úroveň" << std::endl;
        std::cout << "2. Vybrať syna" << std::endl;
        std::cout << "3. Filtrovať zastávky - podľa zemepisnej dĺžky" << std::endl;
        std::cout << "4. Filtrovať zastávky - podľa obce" << std::endl;
        std::cout << "5. Filtrovať zastávky - podľa ulice obsahujúcej text" << std::endl;
        std::cout << "0. Návrat do hlavného menu" << std::endl;
        std::cout << "=========================================" << std::endl;
        std::cout << "Zadajte číslo voľby: ";
        std::cin >> volba;

        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (volba) {
        case 0:
            return;

        case 1: // Prejsť na nadradenú úroveň
            if (!iterator.presunNaNadradenu()) {
                std::cout << "Už ste na najvyššej úrovni!" << std::endl;
            }
            break;

        case 2: { // Vybrať syna
            size_t pocetSynov = iterator.getPocetSynov();
            if (pocetSynov == 0) {
                std::cout << "Aktuálny vrchol nemá žiadnych synov!" << std::endl;
                break;
            }

            std::cout << "Dostupní synovia:" << std::endl;
            for (size_t i = 0; i < pocetSynov; i++) {
                BlokHierarchie* syn = hierarchiaZastavok.getSyn(iterator.getAktualnyBlok(), i);
                std::cout << i << ": " << syn->data_.getNazov() << std::endl;
            }

            size_t index;
            std::cout << "Zadajte index syna: ";
            std::cin >> index;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (index >= pocetSynov) {
                std::cout << "Neplatný index!" << std::endl;
                break;
            }

            iterator.presunNaSyna(index);
            break;
        }

        case 3: { // Filtrovať podľa zemepisnej dĺžky
            double maxLongitude;
            std::cout << "Zadajte maximálnu zemepisnú dĺžku: ";
            std::cin >> maxLongitude;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            auto longitudePredikat = [maxLongitude](const Zastavka& z) -> bool {
                return z.longitude < maxLongitude;
                };

            auto filtrovaneZastavky = iterator.filtrujZastavkyPodlaPredikatuVPodstrome(longitudePredikat);
            std::cout << "Zastávky so zemepisnou dĺžkou menšou ako " << maxLongitude << ":" << std::endl;
            vypisFilterovaneZastavky(filtrovaneZastavky);
            break;
        }

        case 4: { // Filtrovať podľa obce
            std::string hladanaObec;
            std::cout << "Zadajte názov obce: ";
            std::getline(std::cin, hladanaObec);

            auto obecPredikat = [hladanaObec](const Zastavka& z) -> bool {
                return z.obec == hladanaObec;
                };

            auto filtrovaneZastavky = iterator.filtrujZastavkyPodlaPredikatuVPodstrome(obecPredikat);
            std::cout << "Zastávky v obci " << hladanaObec << ":" << std::endl;
            vypisFilterovaneZastavky(filtrovaneZastavky);
            break;
        }

        case 5: { // Filtrovať podľa ulice obsahujúcej text
            std::string hladanyText;
            std::cout << "Zadajte text, ktorý má byť obsiahnutý v názve ulice: ";
            std::getline(std::cin, hladanyText);

            auto ulicaPredikat = [hladanyText](const Zastavka& z) -> bool {
                return z.ulica.find(hladanyText) != std::string::npos;
                };

            auto filtrovaneZastavky = iterator.filtrujZastavkyPodlaPredikatuVPodstrome(ulicaPredikat);
            std::cout << "Zastávky na ulici obsahujúcej '" << hladanyText << "':" << std::endl;
            vypisFilterovaneZastavky(filtrovaneZastavky);
            break;
        }

        default:
            std::cout << "Neplatná voľba. Skúste znova." << std::endl;
        }
    }
}
