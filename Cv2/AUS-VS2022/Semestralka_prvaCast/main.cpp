#include "Zastavka.h"
#include "FilterAlgorithm.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>


vector<Zastavka> zastavky;
vector<Zastavka> filtrovaneZastavky;


void vypisVsetkyZastavky(const vector<Zastavka>& zastavkyNaVypis) {
	for (const auto& zastavka : zastavkyNaVypis) {
		zastavka.vypis();
	}
	cout << "Celkovy pocet zastavok: " << zastavkyNaVypis.size() << endl;
}

void zobrazMenuPrvaCast() {
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

void nacitajVsetkyZastavky(string nazovSuboru)
{
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

int main() {
    nacitajVsetkyZastavky("GRT_Stops.csv");
    zobrazMenuPrvaCast();
    return 0;
}