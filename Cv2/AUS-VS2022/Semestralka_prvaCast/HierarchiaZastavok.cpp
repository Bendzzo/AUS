#include "HierarchiaZastavok.h"
#include <iostream>

HierarchiaZastavok::HierarchiaZastavok() {
    // Vytvor koreò hierarchie (GRT)
    auto& koren = hierarchia.emplaceRoot();
    koren.data_ = Vrchol();
}

void HierarchiaZastavok::vytvorHierarchiu(const std::vector<Zastavka>& zastavky) {
    // Pre každú zastávku
    for (auto& zastavka : zastavky) {
        // Ak ešte neexistuje vrchol pre obec, vytvor ho
        if (mapaObci.find(zastavka.obec) == mapaObci.end()) {
            auto& obecVrchol = hierarchia.emplaceSon(hierarchia.accessRoot(), hierarchia.degree(*hierarchia.accessRoot()));
            obecVrchol.data_ = Vrchol(zastavka.obec, hierarchia.accessRoot());
            mapaObci[zastavka.obec] = &obecVrchol;
        }

        // Ak ešte neexistuje vrchol pre ulicu v danej obci, vytvor ho
        if (mapaUlic[zastavka.obec].find(zastavka.ulica) == mapaUlic[zastavka.obec].end()) {
            auto& ulicaVrchol = hierarchia.emplaceSon(*mapaObci[zastavka.obec],
                hierarchia.degree(*mapaObci[zastavka.obec]));
            ulicaVrchol.data_ = Vrchol(zastavka.ulica, &zastavka);
            mapaUlic[zastavka.obec][zastavka.ulica] = &ulicaVrchol;
        }
    }
}

BlokHierarchie* HierarchiaZastavok::getKoren() const {
    return hierarchia.accessRoot();
}

BlokHierarchie* HierarchiaZastavok::getNadradeny(BlokHierarchie* vrchol) const {
    return hierarchia.accessParent(*vrchol);
}

BlokHierarchie* HierarchiaZastavok::getSyn(BlokHierarchie* vrchol, size_t index) const {
    if (index < hierarchia.degree(*vrchol)) {
        return hierarchia.accessSon(*vrchol, index);
    }
    return nullptr;
}

size_t HierarchiaZastavok::getPocetSynov(BlokHierarchie* vrchol) const {
    return hierarchia.degree(*vrchol);
}

std::vector<Zastavka*> HierarchiaZastavok::filtrujPodlaPredikatuVPodstrome(
    BlokHierarchie* koren,
    std::function<bool(const Zastavka&)> predikat) {

    std::vector<Zastavka*> vysledok;

    // Základný prípad - koreò je nullptr
    if (koren == nullptr) {
        return vysledok;
    }

    // Ak je koreò list (ulica) a má zastávku, otestuj predikát
    Zastavka* zastavka = koren->data_.getZastavka();
    if (zastavka != nullptr && predikat(*zastavka)) {
        vysledok.push_back(zastavka);
    }

    // Rekurzívne preh¾adaj synov
    size_t pocetSynov = hierarchia.degree(*koren);
    for (size_t i = 0; i < pocetSynov; i++) {
        BlokHierarchie* syn = hierarchia.accessSon(*koren, i);
        if (syn != nullptr) {
            auto podvysledok = filtrujPodlaPredikatuVPodstrome(syn, predikat);
            vysledok.insert(vysledok.end(), podvysledok.begin(), podvysledok.end());
        }
    }

    return vysledok;
}

// Implementácia iterátora
HierarchiaIterator::HierarchiaIterator(HierarchiaZastavok* hierarchia)
    : hierarchia(hierarchia), aktualnyVrchol(hierarchia->getKoren()) {
}

bool HierarchiaIterator::presunNaNadradenu() {
    BlokHierarchie* nadradeny = hierarchia->getNadradeny(aktualnyVrchol);
    if (nadradeny != nullptr) {
        aktualnyVrchol = nadradeny;
        return true;
    }
    return false;
}

bool HierarchiaIterator::presunNaSyna(size_t index) {
    BlokHierarchie* syn = hierarchia->getSyn(aktualnyVrchol, index);
    if (syn != nullptr) {
        aktualnyVrchol = syn;
        return true;
    }
    return false;
}

BlokHierarchie* HierarchiaIterator::getAktualnyBlok() const {
    return aktualnyVrchol;
}

Vrchol& HierarchiaIterator::getAktualnyVrchol() const {
    return aktualnyVrchol->data_;
}

size_t HierarchiaIterator::getPocetSynov() const {
    return hierarchia->getPocetSynov(aktualnyVrchol);
}

std::vector<Zastavka*> HierarchiaIterator::filtrujZastavkyPodlaPredikatuVPodstrome(
    std::function<bool(const Zastavka&)> predikat) {
    return hierarchia->filtrujPodlaPredikatuVPodstrome(aktualnyVrchol, predikat);
}