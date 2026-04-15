#include "organ.h"

Organ::Organ(std::string nume, double total, double reg) : nume(nume), cSanatoase(total), totalInitial(total), rataRegenerare(reg), cInfectate(0), cMoarte(0) {}
void Organ::aplicaRegenerare() {
    double regenerare = cMoarte * rataRegenerare;
    cMoarte -= regenerare;
    cSanatoase += regenerare;
}
void Organ::evolutieInfectie() {
    // In fiecare ora, 10% din celulele infectate mor
    double decese = cInfectate * 0.10;
    cInfectate -= decese;
    cMoarte += decese;
}

void Organ::adaugaInfectie(double cantitate) {
    if (cSanatoase >= cantitate) {
        cSanatoase -= cantitate;
        cInfectate += cantitate;
    } else {
        // Daca nu mai sunt sanatoase, infecteaza tot ce a ramas
        cInfectate += cSanatoase;
        cSanatoase = 0;
    }
}