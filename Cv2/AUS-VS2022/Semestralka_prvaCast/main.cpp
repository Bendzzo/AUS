#include "Zastavka.h"
#include "Semestralka.h"

int main() {
    std::vector<Zastavka> zastavky;
    std::vector<Zastavka> filtrovaneZastavky;
    Semestralka semestralka(zastavky, filtrovaneZastavky);

    semestralka.nacitajVsetkyZastavky("GRT_Stops.csv");
    semestralka.zobrazMenuPrvaCast();
    /*semestralka.zobrazMenuDruhaCast();*/
    return 0;
}
