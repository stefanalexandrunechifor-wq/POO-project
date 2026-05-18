#include "sistem_imunitar.h"
#include"celula_imunitara.h"
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
void SistemImunitar::lanseazaAtac(const std::vector<Patogen*>& infectii_active, double bonus_febra) {
    if (infectii_active.empty() || armata.empty()) return;
    if (bonus_febra > 1.0) {
        for (CelulaImunitara* celula : armata) {
            *celula += 2.0;
        }
        std::cout << "[!] Armata a folosit operatorul += pentru a-si creste puterea de baza!\n";
    }
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
        if (!armata.empty()) {
            const Macrofag* mac_original = dynamic_cast<Macrofag*>(armata[0]);
            (void)mac_original;
        }
    }
}
void SistemImunitar::stimuleazaArmata(int cod_secret) {
    std::cout << "\n[Sistem Imunitar] Se incearca stimularea celulelor cu codul: " << cod_secret << "...\n";
    for (CelulaImunitara* celula : armata) {
        if (celula != nullptr) {
            try {
                celula->primesteStimulent(cod_secret);
            }
            catch (const std::exception& e) {
                std::cout << "  -> Eroare la " << celula->getNume() << ": " << e.what() << "\n";
            }
        }
    }
}
void SistemImunitar::afiseazaRaportArmata() const {
    std::cout << "\n=== RAPORT ARMATA IMUNITARA ===\n";
    for (size_t i = 0; i < armata.size(); i++) {
        std::cout << "-" << *armata[i] << "\n";
    }
    if (armata.size() >= 2) {
        if (*armata[0] == *armata[1]) {
            std::cout << "[Analiza] Primele doua celule din armata sunt identice la specificatii!\n";
        }
    }
    std::cout << "===============================\n";
}
