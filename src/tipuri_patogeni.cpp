#include<iostream>
#include"organ.h"
#include"tipuri_patogeni.h"

    Virus::Virus(std::string nume, double putere, std::string organ, double mutatie)
        : Patogen(nume, putere, organ), rata_mutatie(mutatie) {
    }
    void Virus::ataca(Organ* victima) {
        std::cout << "Virusul " << nume << " ataca organul " << victima->getNume() << "!\n";
        double daune = putere_infectie * (1.0 + rata_mutatie);
        victima->adaugaInfectie(daune);
    }
    void Virus::primesteTratament(double putere_medicament) {
        this->putere_infectie -= putere_medicament;
        if (this->putere_infectie < 0) {
            this->putere_infectie = 0;
        }
    }
    Patogen* Virus::clone() const {
        return new Virus(*this);
    }

    void Bacterie::ataca(Organ* victima) {
        std::cout << "Bacteria " << nume << "elibereaza toxine in " << victima->getNume() << "!\n";
        double daune = putere_infectie + toxicitate;
        victima->adaugaInfectie(daune);
    }
    void Bacterie::primesteTratament(double putere_medicament) {
        double daune_primite = putere_medicament;

        // Daca are scutul activ, medicamentul are doar 10% eficienta
        if (rezistenta_antibiotic == true) {
            std::cout << "Bacteria " << nume << " a rezistat tratamentului!\n";
            daune_primite = putere_medicament * 0.10;
        }

        // Scadem viata bacteriei
        this->putere_infectie -= daune_primite;
        if (this->putere_infectie < 0) this->putere_infectie = 0;
    }
    Patogen* Bacterie::clone() const {
        return new Bacterie(*this);
    }
