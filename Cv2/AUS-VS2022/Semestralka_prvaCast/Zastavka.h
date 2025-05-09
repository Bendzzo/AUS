#pragma once
#include <iostream>
#include <string>


struct Zastavka {
    int id;
    double longitude, latitude;
    std::string ulica, obec;

    void vypis() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Longitude: " << longitude << std::endl;
        std::cout << "Latitude: " << latitude << std::endl;
        std::cout << "Ulica: " << ulica << std::endl;
        std::cout << "Obec: " << obec << std::endl;
        std::cout << "------------------------" << std::endl;
    }
};