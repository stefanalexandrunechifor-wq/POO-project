#include <iostream>
#include <vector>
#include <string>
#include "pacient.h"
#include "sisteme_corp.h"
#include "tratamente.h"
#include "tipuri_patogeni.h"
#include "exceptie.h"
void afiseazaMeniuPrincipal() {
    std::cout << "\n=========================================\n";
    std::cout << "           MENIU PRINCIPAL               \n";
    std::cout << "=========================================\n";
    std::cout << "1. Lasa timpul sa treaca (1 Ora)\n";
    std::cout << "2. Farmacie (Administreaza Tratament)\n";
    std::cout << "3. Boli (Infecteaza pacientul - mod test)\n";
    std::cout << "4. Cere analiza AI-ului (Consuma Jeton)\n";
    std::cout << "5. Stimulare Celule (Test Exceptii Custom)\n";
    std::cout << "9. Iesire din tura\n";
    std::cout << "=========================================\n";
    std::cout << "Alege actiunea: ";
}

void afiseazaFarmacie() {
    std::cout << "\n--- DULAPUL CU MEDICAMENTE ---\n";
    std::cout << "1. Tamiflu (Antiviral - Tinta: Ficat) | Putere: 50\n";
    std::cout << "2. Amoxicilina (Antibiotic - Tinta: Stomac) | Putere: 45\n";
    std::cout << "3. Paracetamol (Antipiretic - Tinta: Ficat) | Putere: 20\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Alege medicamentul pentru administrare: ";
}

void afiseazaCatalogBoli() {
    std::cout << "\n--- LABORATOR PATOGENI ---\n";
    std::cout << "1. SARS-CoV-2 (Virus agresiv)\n";
    std::cout << "2. Streptococ (Bacterie comuna)\n";
    std::cout<<"3. HIV (Boss Battle - Tinta: Maduva)\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Alege tulpina: ";
}

int main() {
    srand(time(0));
    std::cout << "=== BINE AI VENIT IN SIMULATORUL MEDICAL ===\n\n";

    Pacient pacient("Ion Popescu");
    CentruDeComanda& comandament = CentruDeComanda::getInstance();

    int ora = 1;
    bool joc_activ = true;

    while (pacient.esteViu() && joc_activ) {
        std::cout << "\n\n++++++++++++++++++++++++++++++++++++++++++++++\n";
        std::cout << "               ORA " << ora << "\n";
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++\n";

        std::cout << "[VITALE] Oxigen: " << pacient.getOxigen()
                  << "% | Temp: " << pacient.getTemperatura()
                  << "C | Puls: " << pacient.getBataiInima() << " BPM\n";

        afiseazaMeniuPrincipal();

        int alegere;
        if (!(std::cin >> alegere)) break;

        switch (alegere) {
            case 1:
                std::cout << "\n--- Timpul trece... ---\n";
                pacient.trece_o_ora();
                ora++;
                break;

            case 2: {
                afiseazaFarmacie();
                int alegere_med;
                std::cin >> alegere_med;
                Medicament* pastila_noua = nullptr;
                if (alegere_med == 1) pastila_noua = new Antiviral("Tamiflu", 50.0, 10.0, 4);
                else if (alegere_med == 2) pastila_noua = new Antibiotic("Amoxicilina", 45.0, 15.0, 6);
                else if (alegere_med == 3) pastila_noua = new Antipiretic("Paracetamol", 20.0, 5.0, 4);
                else if (alegere_med == 0) break; // Inapoi
                else std::cout << "[-] Selectie invalida.\n";
                if (pastila_noua != nullptr) {
                    pacient.adaugaTratament(pastila_noua);
                }
                break;
            }

            case 3: {
                afiseazaCatalogBoli();
                int alegere_boala;
                std::cin >> alegere_boala;

                if (alegere_boala == 1) {
                    Virus* covid = new Virus("SARS-CoV-2", 35.0, "Plamani", 0.2);
                    pacient.adaugaInfectie(covid);
                } else if (alegere_boala == 2) {
                    Bacterie* strep = new Bacterie("Streptococ", 25.0, "Stomac", 0.1, false);
                    pacient.adaugaInfectie(strep);
                } else if (alegere_boala == 3) {
                    Virus* hiv = new Virus("HIV", 60.0, "Maduva", 0.9);
                    pacient.adaugaInfectie(hiv);
                }
                break;
            }

            case 4: {
                bool ficat_blocat = (pacient.getOrgan("Ficat") != nullptr) && pacient.getOrgan("Ficat")->esteSubTratament();
                bool stomac_blocat = (pacient.getOrgan("Stomac") != nullptr) && pacient.getOrgan("Stomac")->esteSubTratament();
                comandament.cereAnalizaLLM(pacient.getOxigen(), pacient.getTemperatura(), ficat_blocat, stomac_blocat);
                break;
            }

            case 5: {
                std::cout << "\n[Test Exceptii] Introduceti codul de stimulare (911): ";
                int cod;
                std::cin >> cod;
                break;
            }

            case 9:
                std::cout << "\n[EXIT] Iesire din joc.\n";
                joc_activ = false;
                break;

            default:
                std::cout << "\n[-] Comanda invalida!\n";
                break;
        }
    }

    if (!pacient.esteViu()) {
        std::cout << "\n++++++++++++++++++++++++++++++++++++++++++++++\n";
        std::cout << "                 GAME OVER\n";
        std::cout << "++++++++++++++++++++++++++++++++++++++++++++++\n";
        std::cout << "Cauza decesului: " << pacient.genereazaRaportDeces() << "\n";
    }

    return 0;
}
