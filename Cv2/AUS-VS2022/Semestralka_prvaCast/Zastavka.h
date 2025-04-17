#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Zastavka {
    int id;
    double longitude, latitude;
    string ulica, obec;

    void vypis() const {
        cout << "ID: " << id << endl;
        cout << "Longitude: " << longitude << endl;
        cout << "Latitude: " << latitude << endl;
        cout << "Ulica: " << ulica << endl;
        cout << "Obec: " << obec << endl;
        cout << "------------------------" << endl;
    }

    Zastavka* getZastavka()
    {
        return this;
    }
public:
    string getUlica()
    {
        return ulica;
    }
};