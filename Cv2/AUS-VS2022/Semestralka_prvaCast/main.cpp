#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Zastavka
{
	int id;
	double longitude, latitude;
	string ulica, obec;

	void vypis()
	{
		cout << "ID: " << id << endl;
		cout << "Longitude: " << longitude << endl;
		cout << "Latitude: " << latitude << endl;
		cout << "Ulica: " << ulica << endl;
		cout << "Obec: " << obec << endl;
		cout << "------------------------" << endl;
	}

	bool isInMunicipality(string nazovObce)
	{
		if (this->obec == nazovObce)
		{
			return true;
		}
		return false;
	}

	bool isOnStreet(string nazovUlice)
	{
		if (this->ulica == nazovUlice)
		{
			return true;
		}
		return false;
	}

	bool isInRegion(double maxLongitude, double maxLatitude, double minLongitude, double minLatitude)
	{
		if (this->longitude <= maxLongitude && this->latitude <= maxLatitude
			&& this->longitude >= minLongitude && this->latitude >= minLatitude)
		{
			return true;
		}
		return false;
	}
};

vector<Zastavka> zastavky;
vector<Zastavka> vObci;
vector<Zastavka> naUlici;
vector<Zastavka> vRegione;


void vypisVsetkyVObci(string nazovObce)
{
	vObci.clear();
	cout << "Zastavky ktore sa nachadzaju v obci " << nazovObce << endl;
	cout << "______________________" << endl;
	for (auto zastavka : zastavky)
	{
		if (zastavka.isInMunicipality(nazovObce))
		{
			vObci.push_back(zastavka);
			zastavka.vypis();
		}
	}
	cout << "Pocet zastavok: " << vObci.size() << endl;
	cout << "______________________" << endl;
}

void vypisVsetkyNaUlici(string nazovUlice)
{
	naUlici.clear();
	cout << "Zastavky ktore sa nachadzaju na ulici " << nazovUlice << endl;
	cout << "______________________" << endl;
	for (auto zastavka : zastavky)
	{
		if (zastavka.isOnStreet(nazovUlice))
		{
			naUlici.push_back(zastavka);
			zastavka.vypis();
		}
	}
	cout << "Pocet zastavok: " << naUlici.size() << endl;
	cout << "______________________" << endl;
}

void vypisVsetkyVRegione(double maxLongitude, double maxLatitude, double minLongitude, double minLatitude)
{
	zastavky.clear();
	cout << "Zastavky ktore sa nachadzaju v medzi suradnicami: "
		<< maxLongitude << " " << maxLatitude
		<< " " << minLongitude << " " << minLatitude << "[maxLongitude][maxLatitude][minLongitude][minLatitude] " << endl;

	cout << "______________________" << endl;
	for (auto zastavka : zastavky)
	{
		if (zastavka.isInRegion(maxLongitude, maxLatitude, minLongitude, minLatitude))
		{
			vRegione.push_back(zastavka);
			zastavka.vypis();
		}
	}
	cout << "Pocet zastavok: " << vRegione.size() << endl;
	cout << "______________________" << endl;
}

void vypisVsetkyZastavky()
{
	for (auto zastavka : zastavky)
	{
		zastavka.vypis();
	}
	cout << "Celkovy pocet zastavok: " << zastavky.size() << endl;
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
			zastavka.longitude = 1000000.0;
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

	vypisVsetkyNaUlici("Winston Blvd");
	//vypisVsetkyVObci("Wilmot");
	//vypisVsetkyVRegione();

	return 0;
}