#include "Zastavka.h"
#include "FilterAlgorithm.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Semestralka.h"

using namespace std;


void Semestralka::vypisZastavky(ds::amt::ImplicitSequence<Zastavka>& zastavkyNaVypis) {
	if (zastavkyNaVypis.size() > 0)
	{
        cout << "\n------------------------" << endl;
	}
    for (const auto& zastavka : zastavkyNaVypis) {
        zastavka.vypis();
    }
    cout << "Celkovy pocet zastavok: " << zastavkyNaVypis.size() << endl;
}

void Semestralka::vypisVsetkyZastavky()
{
    if (zastavky.size() > 0)
    {
        cout << "\n------------------------" << endl;
    }
    for (const auto& zastavka : zastavky) {
        zastavka.vypis();
    }
    cout << "Celkovy pocet zastavok: " << zastavky.size() << endl;
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

    tabulkaZastavok = new ZastavkaTable();

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

        //Nacitanie hierarchie
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


        //Nacitanie zastavok do tabulky
        try {
            tabulkaZastavok->insert(aktualnaZastavka.id, zastavkaPtr);
        }
        catch (const logic_error& e) {
            cerr << "Duplicitny kluc ID " << aktualnaZastavka.id << ": " << e.what() << endl;
        }

    }
    subor.close();
    //cout << "Hierarchia nacitana.\n Pocet vrcholov: " << this->hierarchiaZastavok.size() << endl;
    //cout << this->hierarchiaZastavok.degree(*koren_);
    //cout << this->hierarchiaZastavok.degree(*this->hierarchiaZastavok.accessSon(*koren_, 4));
    //cout << this->hierarchiaZastavok.accessSon(*koren_, 4)->data_.getNazov();
    //vypisSynov(*koren_);
    //cout << "Zastavky boli do tabulky nacitane. Pocet: " << tabulkaZastavok->size() << endl;
}


Zastavka* Semestralka::najdiZastavkuPodlaID(int& id)
{
    try
    {
        return tabulkaZastavok->find(id);
    }
    catch (const out_of_range& e)
    {
        return nullptr;
    }
}


void Semestralka::zobrazMenuPrvaCast() {

    int volba;
    string vstup;
    int zoradenie;

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
            ds::amt::ImplicitSequence<Zastavka> aktualneZastavky;
            aktualneZastavky.reserveCapacity(2650);

            cout << "Zadajte nazov obce: \n";
            getline(cin, vstup);

            /*if (filtrovaneZastavky.size() > 0)
            {
                filtrovaneZastavky = zastavkas;
            }*/
            cout << "Chcete tieto zastavky zoradit?\n[1]ano [2]nie" << endl;
            cin >> zoradenie;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (zoradenie != 1 && zoradenie != 2)
            {
	            cout << "Zly vyber. Automaticky sa zastavky nebudu zoradovat!" << endl;
                zoradenie = 2;
            }

            // isInMunicipality funkcia
            auto obecPredikat = [vstup](const Zastavka& z) -> bool {
                return z.obec.find(vstup) != string::npos;
                };

            auto pridajDoVysledku = [&aktualneZastavky](const Zastavka& z) {
                aktualneZastavky.insertLast().data_ = z;
            };

            Algoritmus::algoritmus(zastavky.begin(), zastavky.end(), obecPredikat, pridajDoVysledku);

            if (zoradenie == 1)
            {
                //this->filtrovaneZastavky = aktualneZastavky;
                zobrazMenuStvrtaCast(aktualneZastavky);
                break;
            }
            cout << "Zastavky v obci " << vstup << ":" << endl;
            vypisZastavky(aktualneZastavky);
            break;
        }

        case 2: {
            ds::amt::ImplicitSequence<Zastavka> aktualneZastavky;
            aktualneZastavky.reserveCapacity(2650);

            cout << "Zadajte nazov ulice: \n";
            getline(cin, vstup);

            cout << "Chcete tieto zastavky zoradit?\n[1]ano [2]nie" << endl;
            cin >> zoradenie;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (zoradenie != 1 && zoradenie != 2)
            {
                cout << "Zly vyber. Automaticky sa zastavky nebudu zoradovat!" << endl;
                zoradenie = 2;
            }

            // isOnStreet funkcia
            auto ulicaPredikat = [vstup](const Zastavka& z) -> bool {
                return z.ulica.find(vstup) != string::npos;
            };

            auto pridajDoVysledku = [&aktualneZastavky](const Zastavka& z) {
                aktualneZastavky.insertLast().data_ = z;
            };

            Algoritmus::algoritmus(zastavky.begin(), zastavky.end(), ulicaPredikat, pridajDoVysledku);

            if (zoradenie == 1)
            {
                zobrazMenuStvrtaCast(aktualneZastavky);
                break;
            }

            cout << "Zastavky na ulici " << vstup << ":" << endl;
            vypisZastavky(aktualneZastavky);
            break;
        }

        case 3: {
            ds::amt::ImplicitSequence<Zastavka> aktualneZastavky;
            aktualneZastavky.reserveCapacity(2650);

            double minLat, maxLat, minLong, maxLong;
            cout << "Zadajte minimalnu zemepisnu sirku (latitude): \n";
            cin >> minLat;
            cout << "Zadajte maximalnu zemepisnu sirku (latitude): \n";
            cin >> maxLat;
            cout << "Zadajte minimalnu zemepisnu dlzku (longitude): \n";
            cin >> minLong;
            cout << "Zadajte maximalnu zemepisnu dlzku (longitude): \n";
            cin >> maxLong;

            cout << "Chcete tieto zastavky zoradit?\n[1]ano [2]nie" << endl;
            cin >> zoradenie;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (zoradenie != 1 && zoradenie != 2)
            {
                cout << "Zly vyber. Automaticky sa zastavky nebudu zoradovat!" << endl;
                zoradenie = 2;
            }

            // isInRegion funkcia
            auto isInRegionPredikat = [minLat, maxLat, minLong, maxLong](const Zastavka& z) -> bool {
                return z.latitude >= minLat && z.latitude <= maxLat &&
                    z.longitude >= minLong && z.longitude <= maxLong;
                };

            auto pridajDoVysledku = [&aktualneZastavky](const Zastavka& z) {
                aktualneZastavky.insertLast().data_ = z;
            };

            Algoritmus::algoritmus(zastavky.begin(), zastavky.end(), isInRegionPredikat, pridajDoVysledku);

            if (zoradenie == 1)
            {
                zobrazMenuStvrtaCast(aktualneZastavky);
                break;
            }
            cout << "Zastavky v zadanej geografickej oblasti:" << endl;
            vypisZastavky(aktualneZastavky);
            break;
        }

        case 4:
            cout << "Vsetky zastavky:" << endl;
            vypisVsetkyZastavky();
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
    int zoradenie;


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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (index >= pocetSynov) {
                cout << "Neplatny index!" << endl;
                break;
            }

            aktualnaPozicia = this->hierarchiaZastavok.accessSon(*aktualnaPozicia, index);
            break;
        }

        case 3: {
            ds::amt::ImplicitSequence<Zastavka> aktualneZastavky;
            aktualneZastavky.reserveCapacity(2650);

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

            cout << "Chcete tieto zastavky zoradit?\n[1]ano [2]nie" << endl;
            cin >> zoradenie;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (zoradenie != 1 && zoradenie != 2)
            {
                cout << "Zly vyber. Automaticky sa zastavky nebudu zoradovat!" << endl;
                zoradenie = 2;
            }

            // isInRegion funkcia
            auto isInRegionPredikat = [minLat, maxLat, minLong, maxLong](const Vrchol& v) -> bool {

                return v.getZastavka() != nullptr && v.getZastavka()->latitude >= minLat && 
                    v.getZastavka()->latitude <= maxLat && v.getZastavka()->longitude >= minLong && 
                    v.getZastavka()->longitude <= maxLong;
            };

            auto pridajDoVysledku = [&aktualneZastavky](const Vrchol& v) {
                if (v.getZastavka() != nullptr) {
                    aktualneZastavky.insertLast().data_ = *(v.getZastavka());
                }
            };

            Algoritmus::algoritmus(begin, this->hierarchiaZastavok.end(), isInRegionPredikat, pridajDoVysledku);

            if (zoradenie == 1)
            {
                zobrazMenuStvrtaCast(aktualneZastavky);
                break;
            }
            vypisZastavky(aktualneZastavky);
            ;
            break;
        }

        case 4: {
            ds::amt::ImplicitSequence<Zastavka> aktualneZastavky;
            aktualneZastavky.reserveCapacity(2650);

            string obecNazov;
            cout << "Zadajte nazov obce: ";
            getline(cin, obecNazov);
            IteratorHierarchie begin(&this->hierarchiaZastavok, aktualnaPozicia);

        	cout << "Chcete tieto zastavky zoradit?\n[1]ano [2]nie" << endl;
            cin >> zoradenie;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (zoradenie != 1 && zoradenie != 2)
            {
                cout << "Zly vyber. Automaticky sa zastavky nebudu zoradovat!" << endl;
                zoradenie = 2;
            }

            // isInMunicipality funkcia
            auto obecPredikat = [&](const Vrchol& v) -> bool {
                if (v.getTyp() == TypVrcholu::ZASTAVKA && v.getZastavka() != nullptr) {
                    return v.getZastavka()->obec.find(obecNazov) != string::npos;
                }
                return false;
            };

        	auto pridajDoVysledku = [&aktualneZastavky](const Vrchol& v) {
                if (v.getZastavka() != nullptr) {
                    aktualneZastavky.insertLast().data_ = (*(v.getZastavka()));
                }
            };

            Algoritmus::algoritmus(begin, this->hierarchiaZastavok.end(), obecPredikat, pridajDoVysledku);

            if (zoradenie == 1)
            {
                zobrazMenuStvrtaCast(aktualneZastavky);
                break;
            }
            vypisZastavky(aktualneZastavky);
            break;
        }

        case 5: {
            ds::amt::ImplicitSequence<Zastavka> aktualneZastavky;
            aktualneZastavky.reserveCapacity(2650);

            string hladanyText;
        	cout << "Zadajte ulicu: ";
        	getline(cin, hladanyText);


            IteratorHierarchie begin(&this->hierarchiaZastavok, aktualnaPozicia);

            cout << "Chcete tieto zastavky zoradit?\n[1]ano [2]nie" << endl;
            cin >> zoradenie;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (zoradenie != 1 && zoradenie != 2)
            {
                cout << "Zly vyber. Automaticky sa zastavky nebudu zoradovat!" << endl;
                zoradenie = 2;
            }

            // isOnStreet funkcia
            auto ulicaPredikat = [&hladanyText](const Vrchol& v) -> bool {
	            if (v.getTyp() == TypVrcholu::ZASTAVKA && v.getZastavka() != nullptr)
	            {
                    return v.getZastavka()->ulica.find(hladanyText) != string::npos;
	            }
            };
            auto pridajDoVysledku = [&aktualneZastavky](const Vrchol& v) {
                if (v.getZastavka() != nullptr) {
                    aktualneZastavky.insertLast().data_ = *(v.getZastavka());
                }
            };

            Algoritmus::algoritmus(begin, this->hierarchiaZastavok.end(), ulicaPredikat, pridajDoVysledku);

            if (zoradenie == 1)
            {
                zobrazMenuStvrtaCast(aktualneZastavky);
                break;
            }
            vypisZastavky(aktualneZastavky);
            break;
        }
        default:
            cout << "Neplatna volba. Skuste znova." << endl;
        }
    }
}

void Semestralka::zobrazMenuTretiaCast()
{
    int volba;
    int idZastavky;

    while (true) {
        cout << "\n========== MENU TRETEJ UROVNE ==========" << endl;
        cout << "1. Vyhladat zastavku podla ID" << endl;
        cout << "0. Navrat do hlavneho menu" << endl;
        cout << "=========================================" << endl;
        cout << "Zadajte cislo volby: ";
        cin >> volba;

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (volba) {
        case 0:
            return;
        case 1: {
            cout << "Zadajte ID zastavky: ";
            cin >> idZastavky;

            // Clear input buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            Zastavka* najdenaZastavka = najdiZastavkuPodlaID(idZastavky);

            if (najdenaZastavka != nullptr)
            {
                cout << "\nNajdena zastavka s ID " << idZastavky << ":" << endl;
                cout << "------------------------" << endl;
                najdenaZastavka->vypis();
            }
            else
            {
                cout << "\nZastavka s ID " << idZastavky << " nebola najdena." << endl;
            }
            break;
        }
        default:
            cout << "Neplatna volba. Skuste znova." << endl;
        }
    }
}

void Semestralka::zobrazMenuStvrtaCast(ds::amt::ImplicitSequence<Zastavka>& zastavky)
{
    int volba;
    int idZastavky;

    ds::adt::QuickSort<Zastavka> quickSort;

    while (true) {
        cout << "\n========== MENU STVRTEJ UROVNE ==========" << endl;
        cout << "1. Zoradit nazvy abecedne" << endl;
        cout << "2. Zoradit podla ID" << endl;
        cout << "0. Navrat do hlavneho menu" << endl;
        cout << "=========================================" << endl;
        cout << "Zadajte cislo volby: ";
        cin >> volba;

        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (volba) {
        case 0:
            return;
        case 1: {
            auto porovnajNazov = [](const Zastavka& a, const Zastavka& b) -> bool {
	            if (a.obec != b.obec)
	            {
                    return a.obec < b.obec;
	            }
                return a.ulica < b.ulica;
            };

        	cout << "Zastavky zoradene abecedne: " << endl;
            quickSort.sort(zastavky, porovnajNazov);
            vypisZastavky(zastavky);
            return;
        }
        case 2: {
            auto porovnajID = [](const Zastavka& a, const Zastavka& b) -> bool {

                return a.id < b.id;
                };

            cout << "Zastavky zoradene podla ID: " << endl;
            quickSort.sort(zastavky, porovnajID);
            vypisZastavky(zastavky);
            return;
        }
        default:
            cout << "Neplatna volba. Skuste znova." << endl;
        }
    }
}


