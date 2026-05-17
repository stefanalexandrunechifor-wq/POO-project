#pragma once
#include <vector>
#include "celula_imunitara.h"
#include "sisteme_corp.h"
class SistemImunitar {
private:
    std::vector<CelulaImunitara*> armata;
    FabricaMaduva fabrica;
public:
    SistemImunitar();
    ~SistemImunitar();
    void lanseazaAtac(std::vector<Patogen*>& infectii_active, double bonus_febra = 1.0);
    void regenereazaArmata(double oxigen_curent);
};