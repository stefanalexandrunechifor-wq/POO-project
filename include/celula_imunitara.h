#pragma once
#include <string>
#include <iostream>
#include "patogen.h"

class CelulaImunitara {
protected:
    std::string nume;
    double putere_atac;

public:
    CelulaImunitara(std::string n, double putere) : nume(n), putere_atac(putere) {}
    virtual ~CelulaImunitara() = default;
    virtual void ataca(Patogen* boala, double bonus_febra = 1.0) = 0;
};

class Macrofag : public CelulaImunitara {
public:
    Macrofag() : CelulaImunitara("Macrofag", 15.0) {}

    void ataca(Patogen* boala, double bonus_febra = 1.0) override {
        double daune_finale = putere_atac * bonus_febra;
        boala->primesteTratament(daune_finale);
    }
};

class Celula_T : public CelulaImunitara {
public:
    Celula_T() : CelulaImunitara("Celula T", 25.0) {}

    void ataca(Patogen* boala, double bonus_febra = 1.0) override {
        if (boala->getTip() == "Virus") {
            std::cout << "[Imunitate] Celula T loveste critic un virus!\n";
            boala->primesteTratament(putere_atac * 2.0 * bonus_febra);
        } else {
            boala->primesteTratament(putere_atac * 0.2 * bonus_febra);
        }
    }
};

class Celula_B : public CelulaImunitara {
public:
    Celula_B() : CelulaImunitara("Celula B", 20.0) {}

    void ataca(Patogen* boala, double bonus_febra = 1.0) override {
        if (boala->getTip() == "Bacterie") {
            std::cout << "[Imunitate] Celula B inunda bacteria cu anticorpi!\n";
            boala->primesteTratament(putere_atac * 2.0 * bonus_febra);
        } else {
            boala->primesteTratament(putere_atac * 0.2 * bonus_febra);
        }
    }
};