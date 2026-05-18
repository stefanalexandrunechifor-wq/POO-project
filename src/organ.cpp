#include "organ.h"

Organ::Organ(std::string nume, double total, double reg) : nume(nume), cSanatoase(total), cInfectate(0), cMoarte(0), rataRegenerare(reg), totalInitial(total) {}
void Organ::aplicaRegenerare() {
    double regenerare = cMoarte * rataRegenerare;
    cMoarte -= regenerare;
    cSanatoase += regenerare;
}
void Organ::evolutieInfectie() {
    double noi_infectari = cInfectate * 0.50;
    if (noi_infectari > cSanatoase) {
        noi_infectari = cSanatoase;
    }
    cSanatoase -= noi_infectari;
    cInfectate += noi_infectari;
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
bool Organ::primesteMedicament(std::string nume_medicament) {
    if (receptor_tratament.leagaMolecula(nume_medicament)) {
        return true;
    }
    return false;
}
void Organ::curataReceptor() {
    receptor_tratament.elibereazaReceptor();
}