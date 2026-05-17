#include "sistem_imunitar.h"
#include <iostream>
SistemImunitar::SistemImunitar() {
    double oxigen_start = 100.0;
    armata.push_back(fabrica.produceCelula("Macrofag", oxigen_start));
    armata.push_back(fabrica.produceCelula("Macrofag", oxigen_start));
    armata.push_back(fabrica.produceCelula("Celula_T", oxigen_start));
    armata.push_back(fabrica.produceCelula("Celula_B", oxigen_start));
}
SistemImunitar::~SistemImunitar() {
    for (CelulaImunitara* c : armata) {
        delete c;
    }
}
void SistemImunitar::lanseazaAtac(std::vector<Patogen*>& infectii_active, double bonus_febra) {
    if (infectii_active.empty() || armata.empty()) return;

    std::cout << "\n=== Sistemul Imunitar riposteaza! ===\n";
    for (Patogen* patogen : infectii_active) {
        for (CelulaImunitara* celula : armata) {
            celula->ataca(patogen, bonus_febra);
        }
    }
}
void SistemImunitar::regenereazaArmata(double oxigen_curent) {
    if (armata.size() < 10) {
        CelulaImunitara* recrut_nou = fabrica.produceCelula("Macrofag", oxigen_curent);
        if (recrut_nou != nullptr) {
            armata.push_back(recrut_nou);
            std::cout << "[+] Sistemul Imunitar a primit intariri! Total celule: " << armata.size() << "\n";
        }
    }
}