#include <string>

#include "Zastavka.h"

enum class TypVrcholu {
    DOPRAVCA,
    OBEC,
    ULICA
};

class Vrchol {
    std::string nazov;
    Zastavka* zastavka;
    TypVrcholu typ;

public:
    Vrchol(const std::string& nazov, TypVrcholu typ, Zastavka* zastavka = nullptr)
        : nazov(nazov), typ(typ), zastavka(zastavka) {
    }

    Vrchol() : nazov("GRT"), typ(TypVrcholu::DOPRAVCA), zastavka(nullptr) {}

    const std::string& getNazov() const { return nazov; }
    Zastavka* getZastavka() const { return zastavka; }
    TypVrcholu getTyp() const { return typ; }
};