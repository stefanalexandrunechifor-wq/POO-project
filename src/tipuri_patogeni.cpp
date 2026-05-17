#include <iostream>
#include "organ.h"
#include "tipuri_patogeni.h"

Bacterie::Bacterie(std::string nume, double putere, std::string organ,
                   double tox, bool rezistenta)
    : Patogen(nume, putere, organ), toxicitate(tox), rezistenta_antibiotic(rezistenta) {}

Virus::Virus(std::string nume, double putere, std::string organ, double mutatie)
    : Patogen(nume, putere, organ), rata_mutatie(mutatie) {
}
void Virus::ataca(Organ* victima, double multiplicator_daune) {
    double daune = (putere_infectie * (1.0 + rata_mutatie)) * multiplicator_daune;
    std::cout << "Virusul " << nume << " ataca organul " << victima->getNume()
              << " cu putere " << daune << "!\n";
    victima->adaugaInfectie(daune);
}

void Virus::primesteTratament(double putere_medicament) {
    this->putere_infectie -= putere_medicament;
    if (this->putere_infectie < 0) {
        this->putere_infectie = 0;
    }
}
Patogen* Virus::clone() const {
    Virus* clona = new Virus(*this);
    clona->aplicaMutatieADN();
    return clona;
}

void Bacterie::ataca(Organ* victima, double multiplicator_daune) {
    double daune = (putere_infectie + toxicitate) * multiplicator_daune;
    std::cout << "Bacteria " << nume << " elibereaza toxine in " << victima->getNume()
              << " cu putere " << daune << "!\n";

    victima->adaugaInfectie(daune);
}

void Bacterie::primesteTratament(double putere_medicament) {
    double daune_primite = putere_medicament;
    if (rezistenta_antibiotic == true) {
        std::cout << "Bacteria " << nume << " a rezistat tratamentului!\n";
        daune_primite = putere_medicament * 0.10;
    }
    this->putere_infectie -= daune_primite;
    if (this->putere_infectie < 0) {
        this->putere_infectie = 0;
    }
}
Patogen* Bacterie::clone() const {
    Bacterie* clona = new Bacterie(*this);
    clona->aplicaMutatieADN();
    return clona;
}