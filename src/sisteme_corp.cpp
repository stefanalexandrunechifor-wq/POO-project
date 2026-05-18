#include<iostream>
#include<cstdlib>
#include"sisteme_corp.h"
#include "celula_imunitara.h"

    SecventaADN::SecventaADN(std::string cod ) : cod_genetic(cod) {}

    const std::string& SecventaADN::getCod() const {
        return cod_genetic;
    }

    void SecventaADN::suferaMutatie() {
        if (cod_genetic.empty()) return;
        char baze[] = {'A', 'C', 'G', 'T'};
        int index = (rand() % 4) * 2;
        cod_genetic[index] = baze[rand() % 4];
    }
CelulaImunitara* FabricaMaduva::produceCelula(std::string tip_celula, double oxigen_pacient) {
        if (oxigen_pacient < 20.0) {
            return nullptr; // nu e destul oxigen
        }

        if (tip_celula == "Macrofag") {
            return new Macrofag();
        } else if (tip_celula == "Celula_T") {
            return new Celula_T();
        } else if (tip_celula == "Celula_B") {
            return new Celula_B();
        }
        return nullptr;
    }