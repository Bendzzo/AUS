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

void vypisVsetkyVrcholy(const vector<Vrchol>& zastavkyNaVypis) {
    for (const auto& vrchol : zastavkyNaVypis) {
        cout << vrchol.getNazov() << endl;
    }
    cout << "Celkovy pocet zastavok: " << zastavkyNaVypis.size() << endl;
}

void Semestralka::nacitajVsetkyZastavky(string nazovSuboru)
{
    koren_ = &this->hierarchiaZastavok.emplaceRoot();
    koren_->data_ = Vrchol();

    BlokHierarchie* aktualnaObecNode = nullptr;
    BlokHierarchie* aktualnaUlicaNode = nullptr;
    string poslednaObec = "";
    string poslednaUlica = "";

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
            //zastavka.vypis();
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
            //zastavka.vypis();
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
            //zastavka.vypis();
            aktualnaZastavka.obec = "ChybaNazov";
        }
        //getline(inputString, aktualnaZastavka.obec);

        zastavky.push_back(aktualnaZastavka);
        Zastavka* zastavkaPtr = &zastavky.back();

        if (aktualnaZastavka.obec != poslednaObec)
        {
            aktualnaObecNode = &this->hierarchiaZastavok.emplaceSon(*koren_, this->hierarchiaZastavok.degree(*koren_));
            aktualnaObecNode->data_ = Vrchol(aktualnaZastavka.obec, TypVrcholu::OBEC, nullptr);

            poslednaObec = aktualnaZastavka.obec;
            poslednaUlica = "";
            aktualnaUlicaNode = nullptr;
        }

        if (aktualnaZastavka.ulica != poslednaUlica)
        {
            aktualnaUlicaNode = &this->hierarchiaZastavok.emplaceSon(*aktualnaObecNode, this->hierarchiaZastavok.degree(*aktualnaObecNode));
            aktualnaUlicaNode->data_ = Vrchol(aktualnaZastavka.ulica, TypVrcholu::ULICA, nullptr);

            poslednaUlica = aktualnaZastavka.ulica;
        }

        if (aktualnaUlicaNode != nullptr)
        {
            string idZastavky = to_string(aktualnaZastavka.id);
            BlokHierarchie& listovyVrchol = hierarchiaZastavok.emplaceSon(*aktualnaUlicaNode,
                this->hierarchiaZastavok.degree(*aktualnaUlicaNode));
            listovyVrchol.data_ = Vrchol(idZastavky, TypVrcholu::ZASTAVKA, zastavkaPtr);
        }
        else
        {
            cerr << "Vrchol ulice je nullptr " << "\n";
        }

        /*cout << riadok++ << " ";
        zastavka.vypis();*/

    }
    subor.close();
    //cout << "Hierarchia nacitana.\n Pocet vrcholov: " << this->hierarchiaZastavok.size() << endl;
    //cout << this->hierarchiaZastavok.degree(*koren_);
    //cout << this->hierarchiaZastavok.degree(*this->hierarchiaZastavok.accessSon(*koren_, 4));
    //cout << this->hierarchiaZastavok.accessSon(*koren_, 4)->data_.getNazov();
    //vypisSynov(*koren_);
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
            cout << "Koniec programu." << endl;
            return;

        case 1: {
            cout << "Zadajte nazov obce: \n";
            getline(cin, vstup);

            //isInMunicipality funkcia
            auto obecPredikat = [vstup](const Zastavka& z) -> bool {
                return z.obec == vstup;
                };

            filter.filter(zastavky.begin(), zastavky.end(), filtrovaneZastavky, obecPredikat);

            cout << "Zastavky v obci " << vstup << ":" << endl;
            vypisVsetkyZastavky(filtrovaneZastavky);
            break;
        }

        case 2: {
            cout << "Zadajte nazov ulice: \n";
            getline(cin, vstup);

            //isOnStreet funkcia
            auto ulicaPredikat = [vstup](const Zastavka& z) -> bool {
                return z.ulica.find(vstup) != string::npos;
                };

            filter.filter(zastavky.begin(), zastavky.end(), filtrovaneZastavky, ulicaPredikat);

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

            //isInRegion funkcia
            auto isInRegionPredikat = [minLat, maxLat, minLong, maxLong](const Zastavka& z) -> bool {
                return z.latitude >= minLat && z.latitude <= maxLat &&
                    z.longitude >= minLong && z.longitude <= maxLong;
                };

            filter.filter(zastavky.begin(), zastavky.end(), filtrovaneZastavky, isInRegionPredikat);

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
    FilterAlgorithm<vector<Zastavka>::iterator, vector<Zastavka>> filter;

    std::vector<Vrchol> vrcholy;

    while (true) {
        IteratorHierarchie zaciatok(&this->hierarchiaZastavok, aktualnaPozicia);
        vrcholy.clear();

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
        cout << "3. Filtrovat zastavky - podla zemepisnej dlzky" << endl;
        cout << "4. Filtrovat zastavky - podla obce" << endl;
        cout << "5. Filtrovat zastavky - podla ulice obsahujucej text" << endl;
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

   //     case 3: {
   //         double minLat, maxLat, minLong, maxLong;
   //         cout << "Zadajte minimalnu zemepisnu sirku (latitude): \n";
   //         cin >> minLat;
   //         cout << "Zadajte maximalnu zemepisnu sirku (latitude): \n";
   //         cin >> maxLat;
   //         cout << "Zadajte minimalnu zemepisnu dlzku (longitude): \n";
   //         cin >> minLong;
   //         cout << "Zadajte maximalnu zemepisnu dlzku (longitude): \n";
   //         cin >> maxLong;

   //         //isInRegion funkcia
   //         auto regionPredicate = [minLat, maxLat, minLong, maxLong](const Zastavka& z) -> bool {
   //             return z.latitude >= minLat && z.latitude <= maxLat &&
   //                 z.longitude >= minLong && z.longitude <= maxLong;
   //         };
			//for (size_t i = 0; i < this->hierarchiaZastavok.degree(*aktualnaPozicia); ++i)
			//{
   //             auto syn = this->hierarchiaZastavok.accessSon(*aktualnaPozicia, i)->data_.getZastavka();
   //             vrcholy.push_back(*syn);
			//}


   //         filter.filter(vrcholy.begin(), vrcholy.end(), filtrovaneZastavky, regionPredicate);

   //         cout << "Zastavky v zadanej geografickej oblasti:" << endl;
   //         vypisVsetkyZastavky(filtrovaneZastavky);
   //         break;
   //     }

        /*case 4: {
            string hladanaObec;
            cout << "Zadajte nazov obce: ";
            getline(cin, hladanaObec);

            vrcholy.clear();

            for (IteratorHierarchie it(&this->hierarchiaZastavok, aktualnaPozicia);
                it != this->hierarchiaZastavok.end(); ++it) {

                const Vrchol& vrchol = *it;
                const Zastavka* z = vrchol.getZastavka();

                if (z != nullptr && z->obec == hladanaObec) {
                    vrcholy.push_back(*z);
                }
            }

            cout << "Zastavky v obci '" << hladanaObec << "':" << endl;
            vypisVsetkyZastavky(vrcholy);
            break;
        }*/

        case 5: {
            string hladanyText;
            cout << "Zadajte text, ktory ma byt obsiahnuty v nazve ulice: ";
            getline(cin, hladanyText);

            vrcholy.clear();

            int index = 0;
            for (IteratorHierarchie it(&this->hierarchiaZastavok, aktualnaPozicia);
                it != this->hierarchiaZastavok.end(); ++it) {
                index++;
                Vrchol& vrchol = *it;
                const Zastavka* z = vrchol.getZastavka();


                if ((vrchol.getTyp() == TypVrcholu::ULICA && vrchol.getNazov().find(hladanyText) != string::npos)) {
                    vrcholy.push_back(vrchol);
                    vypisSynov(this->hierarchiaZastavok.accessSon(aktualnaPozicia, index));
                }
            }

            cout << "\nZastavky na ulici obsahujucej '" << hladanyText << "':" << endl;
            vypisVsetkyVrcholy(vrcholy);
            break;
        }


        default:
            cout << "Neplatna volba. Skuste znova." << endl;
        }
    }
}
