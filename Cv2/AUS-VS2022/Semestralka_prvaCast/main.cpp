#include "libds/heap_monitor.h"

#include "Zastavka.h"
#include "Semestralka.h"


int main() {
    initHeapMonitor();

    std::vector<Zastavka> zastavky;
    Semestralka semestralka(zastavky);

    semestralka.nacitajVsetkyZastavky("GRT_Stops.csv");

    int volba;
    while (true) {
        std::cout << "========== MENU ==========" << std::endl;
        std::cout << "1. Prva uroven" << std::endl;
        std::cout << "2. Druha uroven" << std::endl;
        std::cout << "3. Tretia uroven" << std::endl;
        //std::cout << "4. Stvrta uroven" << std::endl;
        std::cout << "0. Koniec" << std::endl;
        std::cout << "==========================" << std::endl;
        std::cout << "Zadajte cislo volby: ";
        std::cin >> volba;

        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (volba)
        {
        case 0:
        {
            std::cout << "Koniec" << std::endl;
            return 0;
        }
        case 1:
        {
            semestralka.zobrazMenuPrvaCast();
            break;
        }
        case 2:
        {
            semestralka.zobrazMenuDruhaCast();
            break;
        }
        case 3:
        {
            semestralka.zobrazMenuTretiaCast();
            break;
        }
        /*case 4:
        {
            semestralka.zobrazMenuStvrtaCast();
            break;
        }*/
        default:
        {
            std::cout << "Nespravna volba, skuste este raz" << std::endl;
            break;
        }
        }
    }
}
