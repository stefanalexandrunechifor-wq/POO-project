#include<iostream>
#include<cstdlib>
#include"sisteme_corp.h"

    SecventaADN::SecventaADN(std::string cod ) : cod_genetic(cod) {}

    std::string SecventaADN::getCod() const {
        return cod_genetic;
    }

    void SecventaADN::suferaMutatie() {
        if (cod_genetic.empty()) return;
        char baze[] = {'A', 'C', 'G', 'T'};
        int index = (rand() % 4) * 2;
        cod_genetic[index] = baze[rand() % 4];
    }
