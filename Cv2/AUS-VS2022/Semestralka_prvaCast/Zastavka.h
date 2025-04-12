#pragma once
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
