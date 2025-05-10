#include "Zastavka.h"
#include "FilterAlgorithm.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include "Semestralka.h"

using namespace std;

void Semestralka::vypisVsetkyZastavky(const vector<Zastavka>& zastavkyNaVypis) {
	if (zastavkyNaVypis.size() > 0)
	{
        cout << "\n------------------------" << endl;
	}
    for (const auto& zastavka : zastavkyNaVypis) {
        zastavka.vypis();
    }
    cout << "Celkovy pocet zastavok: " << zastavkyNaVypis.size() << endl;
}

void Semestralka::nacitajVsetkyZastavky(string nazovSuboru)
{
    koren_ = &this->hierarchiaZastavok.emplaceRoot();
    koren_->data_ = Vrchol();

    BlokHierarchie* aktualnaObecVrchol = nullptr;
    BlokHierarchie* aktualnaUlicaVrchol = nullptr;
    string poslednaObec = "";
    string poslednaUlica = "";

    zastavky.reserve(2650);

    ifstream subor;
    subor.open(nazovSuboru);
    if (!subor.is_open()) {
        cerr << "Nepodarilo sa otvorit subor " << nazovSuboru << "\n";
        return;
    }

    string line;

    getline(subor, line);

    int riadok = 0;
    while (getline(subor, line))
    {
        Zastavka aktualnaZastavka;
        string tempString;
        stringstream inputString(line);

        // StopID
        getline(inputString, tempString, ';');
        aktualnaZastavka.id = stoi(tempString);

        // Street
        getline(inputString, aktualnaZastavka.ulica, ';');

        // Longitude
        getline(inputString, tempString, ';');
        if (!tempString.empty())
        {
            aktualnaZastavka.longitude = stod(tempString);

        }
        else
        {
            //zastavkaPtr.vypis();
            aktualnaZastavka.longitude = 1000000.0;
        }

        // Latitude
        tempString = "";
        getline(inputString, tempString, ';');
        if (!tempString.empty())
        {
            aktualnaZastavka.latitude = stod(tempString);

        }
        else
        {
            //zastavkaPtr.vypis();
            aktualnaZastavka.latitude = 1000000.0;
        }

        // Municipality
        tempString = "";
        getline(inputString, tempString, ';');
        if (!tempString.empty())
        {
            aktualnaZastavka.obec = tempString;

        }
        else
        {
            //zastavkaPtr.vypis();
            aktualnaZastavka.obec = "ChybaNazov";
        }
        //getline(inputString, aktualnaZastavka.obec);

        zastavky.push_back(aktualnaZastavka);
        Zastavka* zastavkaPtr = &zastavky.back();

        if (aktualnaZastavka.obec != poslednaObec)
        {
            aktualnaObecVrchol = &this->hierarchiaZastavok.emplaceSon(*koren_, this->hierarchiaZastavok.degree(*koren_));
            aktualnaObecVrchol->data_ = Vrchol(aktualnaZastavka.obec, TypVrcholu::OBEC);

            poslednaObec = aktualnaZastavka.obec;
            poslednaUlica = "";
            aktualnaUlicaVrchol = nullptr;
        }

        if (aktualnaZastavka.ulica != poslednaUlica)
        {
            aktualnaUlicaVrchol = &this->hierarchiaZastavok.emplaceSon(*aktualnaObecVrchol, this->hierarchiaZastavok.degree(*aktualnaObecVrchol));
            aktualnaUlicaVrchol->data_ = Vrchol(aktualnaZastavka.ulica, TypVrcholu::ULICA);

            poslednaUlica = aktualnaZastavka.ulica;
        }

        if (aktualnaUlicaVrchol != nullptr)
        {
            string idZastavky = to_string(aktualnaZastavka.id);
            BlokHierarchie& listovyVrchol = hierarchiaZastavok.emplaceSon(*aktualnaUlicaVrchol,
                this->hierarchiaZastavok.degree(*aktualnaUlicaVrchol));
            listovyVrchol.data_ = Vrchol(idZastavky, TypVrcholu::ZASTAVKA, zastavkaPtr);
            //cout << "Pridana zastavka do hierarchie " << zastavkaPtr->id << " " << zastavkaPtr->obec << endl;
        }
        else
        {
            cerr << "Vrchol ulice je nullptr " << "\n";
        }

        /*cout << riadok++ << " ";
        zastavkaPtr.vypis();*/

    }
    subor.close();
    //cout << "Hierarchia nacitana.\n Pocet vrcholov: " << this->hierarchiaZastavok.size() << endl;
    //cout << this->hierarchiaZastavok.degree(*koren_);
    //cout << this->hierarchiaZastavok.degree(*this->hierarchiaZastavok.accessSon(*koren_, 4));
    //cout << this->hierarchiaZastavok.accessSon(*koren_, 4)->data_.getNazov();
    //vypisSynov(*koren_);
}


void Semestralka::zobrazMenuPrvaCast() {

    int volba;
    string vstup;
    vector<Zastavka> filtrovaneZastavky;

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
            cout << "Koniec programu." << endl;
            return;
        case 1: {
            cout << "Zadajte nazov obce: \n";
            getline(cin, vstup);

            filtrovaneZastavky.clear();

            // isInMunicipality funkcia
            auto obecPredikat = [vstup](const Zastavka& z) -> bool {
                return z.obec == vstup;
                };
            auto pridajDoVysledku = [&filtrovaneZastavky](const Zastavka& z) {
                filtrovaneZastavky.push_back(z);
                };

            Algoritmus::algoritmus(zastavky.begin(), zastavky.end(), obecPredikat, pridajDoVysledku);

            cout << "Zastavky v obci " << vstup << ":" << endl;
            vypisVsetkyZastavky(filtrovaneZastavky);
            break;
        }

        case 2: {
            cout << "Zadajte nazov ulice: \n";
            getline(cin, vstup);

            filtrovaneZastavky.clear();

            // isOnStreet funkcia
            auto ulicaPredikat = [vstup](const Zastavka& z) -> bool {
                return z.ulica.find(vstup) != string::npos;
                };

            auto pridajDoVysledku = [&filtrovaneZastavky](const Zastavka& z) {
                filtrovaneZastavky.push_back(z);
                };

            Algoritmus::algoritmus(zastavky.begin(), zastavky.end(), ulicaPredikat, pridajDoVysledku);

            cout << "Zastavky na ulici " << vstup << ":" << endl;
            vypisVsetkyZastavky(filtrovaneZastavky);
            break;
        }

        case 3: {
            double minLat, maxLat, minLong, maxLong;
            cout << "Zadajte minimalnu zemepisnu sirku (latitude): \n";
            cin >> minLat;
            cout << "Zadajte maximalnu zemepisnu sirku (latitude): \n";
            cin >> maxLat;
            cout << "Zadajte minimalnu zemepisnu dlzku (longitude): \n";
            cin >> minLong;
            cout << "Zadajte maximalnu zemepisnu dlzku (longitude): \n";
            cin >> maxLong;

            filtrovaneZastavky.clear();

            // isInRegion funkcia
            auto isInRegionPredikat = [minLat, maxLat, minLong, maxLong](const Zastavka& z) -> bool {
                return z.latitude >= minLat && z.latitude <= maxLat &&
                    z.longitude >= minLong && z.longitude <= maxLong;
                };

            auto pridajDoVysledku = [&filtrovaneZastavky](const Zastavka& z) {
                filtrovaneZastavky.push_back(z);
                };

            Algoritmus::algoritmus(zastavky.begin(), zastavky.end(), isInRegionPredikat, pridajDoVysledku);

            cout << "Zastavky v zadanej geografickej oblasti:" << endl;
            vypisVsetkyZastavky(filtrovaneZastavky);
            break;
        }

        case 4:
            cout << "Vsetky zastavky:" << endl;
            vypisVsetkyZastavky(zastavky);
            break;

        default:
            cout << "Neplatna volba. Skuste znova." << endl;
        }


        cout << endl;
    }
}

void Semestralka::vypisSynov(BlokHierarchie& vrchol)
{
    cout << "Synovia vrchola " << vrchol.data_.getNazov() << " \n";
    for (size_t i = 0; i < this->hierarchiaZastavok.degree(vrchol); ++i)
    {
        auto syn = this->hierarchiaZastavok.accessSon(vrchol, i);
        cout << "[" << i << "] " << syn->data_.getNazov() << endl;
    }
}

void Semestralka::zobrazMenuDruhaCast()
{
    int volba;
    string vstup;
    BlokHierarchie* aktualnaPozicia = koren_;


    while (true) {
        IteratorHierarchie zaciatok(&this->hierarchiaZastavok, aktualnaPozicia);

        cout << "\n========== MENU DRUHEJ UROVNE ==========" << endl;
        cout << "Aktualna pozicia: " << aktualnaPozicia->data_.getNazov() << endl;
        cout << "Typ: ";

        if (aktualnaPozicia->data_.getTyp() == TypVrcholu::DOPRAVCA) {
            cout << "Dopravca (koren)" << endl;
        }
        else if (aktualnaPozicia->data_.getTyp() == TypVrcholu::OBEC) {
            cout << "Obec" << endl;
        }
        else if (aktualnaPozicia->data_.getTyp() == TypVrcholu::ULICA) {
            cout << "Ulica" << endl;
        }

        cout << "1. Prejst na nadradenu uroven" << endl;
        cout << "2. Vybrat syna" << endl;
        cout << "3. Filtrovat podla Zemepisnej dlzky" << endl;
        cout << "4. Filtrovat podla Obce" << endl;
        cout << "5. Filtrovat podla Ulice" << endl;
        cout << "0. Navrat do hlavneho menu" << endl;
        cout << "=========================================" << endl;
        cout << "Zadajte cislo volby: ";
        cin >> volba;

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (volba) {
        case 0:
            return;

        case 1:
            if (!this->hierarchiaZastavok.accessParent(*aktualnaPozicia)) {
                cout << "\nSte na najvyssej urovni" << endl;
                break;
            }
            aktualnaPozicia = this->hierarchiaZastavok.accessParent(*aktualnaPozicia);
            break;

        case 2: {
            size_t pocetSynov = this->hierarchiaZastavok.degree(*aktualnaPozicia);
            if (pocetSynov == 0) {
                cout << "Vrchol je List, teda nema synov" << endl;
                break;
            }

            vypisSynov(*aktualnaPozicia);

            size_t index;
            cout << "Zadajte index syna: ";
            cin >> index;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (index >= pocetSynov) {
                cout << "Neplatny index!" << endl;
                break;
            }

            aktualnaPozicia = this->hierarchiaZastavok.accessSon(*aktualnaPozicia, index);
            break;
        }

        case 3: {
            this->filtrovaneZastavky.clear();
            IteratorHierarchie begin(&this->hierarchiaZastavok, aktualnaPozicia);

            double minLat, maxLat, minLong, maxLong;
            cout << "Zadajte minimalnu zemepisnu sirku (latitude): \n";
            cin >> minLat;
            cout << "Zadajte maximalnu zemepisnu sirku (latitude): \n";
            cin >> maxLat;
            cout << "Zadajte minimalnu zemepisnu dlzku (longitude): \n";
            cin >> minLong;
            cout << "Zadajte maximalnu zemepisnu dlzku (longitude): \n";
            cin >> maxLong;

            // isInRegion funkcia
            auto isInRegionPredikat = [minLat, maxLat, minLong, maxLong](const Vrchol& v) -> bool {

                return v.getZastavka() != nullptr && v.getZastavka()->latitude >= minLat && 
                    v.getZastavka()->latitude <= maxLat && v.getZastavka()->longitude >= minLong && 
                    v.getZastavka()->longitude <= maxLong;
            };

            auto pridajDoVysledku = [this](const Vrchol& v) {
                if (v.getZastavka() != nullptr) {
                    this->filtrovaneZastavky.push_back(*(v.getZastavka()));
                }
            };

            Algoritmus::algoritmus(begin, this->hierarchiaZastavok.end(), isInRegionPredikat, pridajDoVysledku);

            vypisVsetkyZastavky(this->filtrovaneZastavky);
            break;
        }

        case 4: {
            std::string obecNazov;
            std::cout << "Zadajte nazov obce: ";
            getline(std::cin, obecNazov);
            this->filtrovaneZastavky.clear();
            IteratorHierarchie begin(&this->hierarchiaZastavok, aktualnaPozicia);


            // isInMunicipality funkcia
            auto obecPredikat = [&](const Vrchol& v) -> bool {
                if (v.getTyp() == TypVrcholu::ZASTAVKA && v.getZastavka() != nullptr) {
                    return v.getZastavka()->obec.find(obecNazov) != string::npos;
                }
                return false;
            };

        	auto pridajDoVysledku = [this](const Vrchol& v) {
                if (v.getZastavka() != nullptr) {
                    this->filtrovaneZastavky.push_back(*(v.getZastavka()));
                }
            };

            Algoritmus::algoritmus(begin, this->hierarchiaZastavok.end(), obecPredikat, pridajDoVysledku);
            vypisVsetkyZastavky(this->filtrovaneZastavky);
            break;
        }

        case 5: {
            string hladanyText;
        	cout << "Zadajte ulicu: ";
        	getline(cin, hladanyText);

            this->filtrovaneZastavky.clear();

            cout << hladanyText << endl;
            IteratorHierarchie begin(&this->hierarchiaZastavok, aktualnaPozicia);

            // isOnStreet funkcia
            auto ulicaPredikat = [&hladanyText](const Vrchol& v) -> bool {
	            if (v.getTyp() == TypVrcholu::ZASTAVKA && v.getZastavka() != nullptr)
	            {
                    return v.getZastavka()->ulica.find(hladanyText) != string::npos;
	            }
            };
            auto pridajDoVysledku = [this](const Vrchol& v) {
                if (v.getZastavka() != nullptr) {
                    this->filtrovaneZastavky.push_back(*(v.getZastavka()));
                }
            };

            Algoritmus::algoritmus(begin, this->hierarchiaZastavok.end(), ulicaPredikat, pridajDoVysledku);

            vypisVsetkyZastavky(this->filtrovaneZastavky);
            break;
        }
        default:
            cout << "Neplatna volba. Skuste znova." << endl;
        }
    }
}


