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
    Zastavka* zastavka;
    TypVrcholu typ;

public:
    Vrchol(const std::string& nazov, const TypVrcholu typ, Zastavka* zastavka = nullptr)
        : nazov(nazov), zastavka(zastavka), typ(typ) {
    }

    Vrchol(const std::string& nazov, const TypVrcholu typ)
        : nazov(nazov), typ(typ), zastavka(nullptr) {
    }

    Vrchol() : nazov("GRT"), zastavka(nullptr), typ(TypVrcholu::DOPRAVCA) {}

    const std::string& getNazov() const { return nazov; }
    Zastavka* getZastavka() const { return zastavka; }
    TypVrcholu getTyp() const { return typ; }

    bool operator==(const Vrchol& other) const
    {
	    if (nazov == other.nazov && zastavka == other.zastavka)
	    {
            return true;
	    }
        return false;
    }
};