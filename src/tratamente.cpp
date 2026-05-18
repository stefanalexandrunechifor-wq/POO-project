#include "tratamente.h"
#include "pacient.h"
#include "organ.h"
#include "tipuri_patogeni.h"
#include <iostream>
Antibiotic::Antibiotic(const std::string& n, double putere, double daune, int ore)
    : Medicament(n, putere, daune, ore) {}

void Antibiotic::administreaza(Pacient* pacient) {
    Organ* stomac = pacient->getOrgan("Stomac");
    if (stomac != nullptr) {
        stomac->adaugaInfectie(daune_colaterale);
        std::cout << "[!] Atentie: Stomacul a suferit " << daune_colaterale << " daune de la " << this->getName() << ".\n";
        if (stomac->primesteMedicament(this->getName())) {
            std::cout << "[+] Stomacul a absorbit cu succes " << this->getName() << ".\n";
            for (Patogen* boala : pacient->getInfectii()) {
                Bacterie* b = dynamic_cast<Bacterie*>(boala);
                if (b != nullptr) {
                    b->primesteTratament(putere_vindecare);
                    std::cout << "  -> " << b->getNume() << " a fost atacata (" << putere_vindecare << " putere).\n";
                }
            }
        } else {
            std::cout << "[-] Eroare: Stomacul proceseaza deja alt tratament! " << this->getName() << " a iritat organul fara efect medical.\n";
        }
    } else {
        std::cout << "[-] Eroare critica: Pacientul nu are organul 'Stomac' pentru absorbtie!\n";
    }
}
Antiviral::Antiviral(const std::string& n, double putere, double daune, int ore)
    : Medicament(n, putere, daune, ore) {}

void Antiviral::administreaza(Pacient* pacient) {
    Organ* ficat = pacient->getOrgan("Ficat");
    if (ficat != nullptr) {
        ficat->adaugaInfectie(daune_colaterale);
        std::cout << "[!] Atentie: Ficatul a suferit " << daune_colaterale << " daune chimice de la " << this->getName() << ".\n";
        if (ficat->primesteMedicament(this->getName())) {
            std::cout << "[+] Antiviralul a fost absorbit si ataca boala!\n";

            for (Patogen* boala : pacient->getInfectii()) {
                Virus* v = dynamic_cast<Virus*>(boala);
                if (v != nullptr) {
                    v->primesteTratament(putere_vindecare);
                }
            }
        } else {
            std::cout << "[-] Eroare: Ficatul e blocat de alt tratament! " << this->getName() << " nu a avut niciun efect medical, dar a ranit organul!\n";
        }
    } else {
        std::cout << "[-] Eroare: Pacientul nu are Ficat!\n";
    }
}
Antipiretic::Antipiretic(const std::string& n, double putere, double daune, int ore)
    : Medicament(n, putere, daune, ore) {}

void Antipiretic::administreaza(Pacient* pacient) {
    std::cout << "\n=== Se administreaza antipireticul: " << nume << " ===\n";
    Organ* ficat = pacient->getOrgan("Ficat");
    if (ficat != nullptr) {
        ficat->adaugaInfectie(daune_colaterale);
        std::cout << "[!] Efect secundar: Ficatul a procesat toxinele (" << daune_colaterale << " daune chimice).\n";
        if (ficat->primesteMedicament(this->getName())) {
            bool a_gasit_boli = false;
            for (Patogen* boala : pacient->getInfectii()) {
                boala->primesteTratament(putere_vindecare);
                a_gasit_boli = true;
            }
            if (a_gasit_boli) {
                std::cout << "[+] Febra a scazut! Toate infectiile au fost slabite.\n";
            } else {
                std::cout << "[-] Pacientul nu are infectii. Medicamentul a fost absorbit degeaba.\n";
            }

        } else {
            std::cout << "[-] Eroare: Ficatul e suprasolicitat de alt tratament! Antipireticul te-a costat viata organului degeaba.\n";
        }
    } else {
        std::cout << "[-] Eroare: Pacientul nu are Ficat!\n";
    }
}
