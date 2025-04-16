#include <string>

#include "Zastavka.h"

//enum class TypVrcholu {
//    DOPRAVCA,
//    OBEC,
//    ULICA
//};

class Vrchol {
    std::string nazov;
    Zastavka* zastavka;
    //TypVrcholu typ;

public:
    Vrchol(const std::string& nazov, Zastavka* zastavka = nullptr)
        : nazov(nazov), zastavka(zastavka) {
    }

    Vrchol() : nazov("GRT"), zastavka(nullptr) {}

    const std::string& getNazov() const { return nazov; }
    Zastavka* getZastavka() const { return zastavka; }
    /*TypVrcholu getTyp() const { return typ; }*/

    bool operator==(const Vrchol& other) const
    {
	    if (nazov == other.nazov && zastavka == other.zastavka)
	    {
            return true;
	    }
        return false;
    }
};