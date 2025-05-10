#pragma once

#include <string>
#include "Zastavka.h"

enum class TypVrcholu {
    DOPRAVCA,
    OBEC,
    ULICA,
    ZASTAVKA
};

class Vrchol {
    std::string nazov;
    Zastavka* zastavkaPtr;
    TypVrcholu typ;

public:
    Vrchol(const std::string& nazov, const TypVrcholu typ, Zastavka* zastavka = nullptr)
        : nazov(nazov), zastavkaPtr(zastavka), typ(typ) {
    }

    Vrchol() : nazov("GRT"), zastavkaPtr(nullptr), typ(TypVrcholu::DOPRAVCA) {}

    const std::string& getNazov() const { return nazov; }
    Zastavka* getZastavka() const { return zastavkaPtr; }
    TypVrcholu getTyp() const { return typ; }

    bool operator==(const Vrchol& other) const
    {
	    if (nazov == other.nazov && zastavkaPtr == other.zastavkaPtr)
	    {
            return true;
	    }
        return false;
    }
};