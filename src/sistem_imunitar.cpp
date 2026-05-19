#include "sistem_imunitar.h"
#include"celula_imunitara.h"
#include <algorithm>
#include <iostream>
namespace {
double calculeazaDauneBaza(const CelulaImunitara* celula, const Patogen* patogen) {
    if (celula == nullptr || patogen == nullptr) {
        return 0.0;
    }
    const double putere = celula->getPutere();
    if (dynamic_cast<const Macrofag*>(celula) != nullptr) {
        return putere;
    }
    if (dynamic_cast<const Celula_T*>(celula) != nullptr) {
        return (patogen->getTip() == "Virus") ? putere * 2.0 : putere * 0.2;
    }
    if (dynamic_cast<const Celula_B*>(celula) != nullptr) {
        return (patogen->getTip() == "Bacterie") ? putere * 2.0 : putere * 0.2;
    }
    return putere;
}
}
SistemImunitar::SistemImunitar() : oboseala_imuna(0.0), ore_lupta_consecutiva(0) {
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
    if (infectii_active.empty() || armata.empty()) {
        oboseala_imuna = std::max(0.0, oboseala_imuna - 0.08);
        ore_lupta_consecutiva = 0;
        return;
    }
    if (bonus_febra > 1.0) {
        for (CelulaImunitara* celula : armata) {
            *celula += 0.6;
        }
        std::cout << "[!] Febra a stimulat moderat armata imunitara.\n";
    }

    double putere_totala_armata = 0.0;
    for (const CelulaImunitara* celula : armata) {
        if (celula != nullptr) {
            putere_totala_armata += celula->getPutere();
        }
    }

    const double factor_oboseala = std::max(0.35, 1.0 - oboseala_imuna);
    const double buget_imun_orar = putere_totala_armata * bonus_febra * factor_oboseala;

    double daune_teoretice = 0.0;
    for (const Patogen* patogen : infectii_active) {
        if (patogen == nullptr || patogen->getPutere() <= 0.0) {
            continue;
        }
        for (const CelulaImunitara* celula : armata) {
            daune_teoretice += calculeazaDauneBaza(celula, patogen);
        }
    }
    double factor_scalare = 1.0;
    const double daune_teoretice_cu_bonus = daune_teoretice * bonus_febra;
    if (daune_teoretice_cu_bonus > 0.0) {
        factor_scalare = std::min(1.0, buget_imun_orar / daune_teoretice_cu_bonus);
    }

    std::cout << "\n=== Sistemul Imunitar riposteaza! ===\n";
    for (Patogen* patogen : infectii_active) {
        if (patogen == nullptr || patogen->getPutere() <= 0.0) {
            continue;
        }
        for (CelulaImunitara* celula : armata) {
            celula->ataca(patogen, bonus_febra * factor_scalare);
        }
    }

    ore_lupta_consecutiva++;
    oboseala_imuna = std::min(0.55, oboseala_imuna + 0.015 * infectii_active.size() + 0.004 * ore_lupta_consecutiva);
}
void SistemImunitar::aplicaImunosupresie(double intensitate) {
    if (intensitate <= 0.0 || armata.empty()) {
        return;
    }
    const double intensitate_limitata = std::clamp(intensitate, 0.0, 0.6);
    for (CelulaImunitara* celula : armata) {
        if (celula == nullptr) {
            continue;
        }
        const std::string& nume = celula->getNume();
        if (nume != "Celula T" && nume != "Celula B") {
            continue;
        }
        const double putere_curenta = celula->getPutere();
        const double putere_minima = 2.0;
        const double reducere_propusa = putere_curenta * intensitate_limitata * 0.35;
        const double reducere_maxima = std::max(0.0, putere_curenta - putere_minima);
        const double reducere_finala = std::min(reducere_propusa, reducere_maxima);
        if (reducere_finala > 0.0) {
            *celula += -reducere_finala;
        }
    }
    oboseala_imuna = std::min(0.70, oboseala_imuna + intensitate_limitata * 0.30);
}
void SistemImunitar::regenereazaArmata(double oxigen_curent) {
    if (oxigen_curent > 70.0) {
        oboseala_imuna = std::max(0.0, oboseala_imuna - 0.06);
        if (ore_lupta_consecutiva > 0) {
            ore_lupta_consecutiva--;
        }
    } else {
        oboseala_imuna = std::max(0.0, oboseala_imuna - 0.02);
    }

    if (armata.size() < 10 && oxigen_curent > 55.0 && oboseala_imuna < 0.50) {
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
    std::cout << "Oboseala imunologica curenta: " << oboseala_imuna * 100.0 << "%\n";
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
