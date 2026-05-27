#include"simulator.h"
#include "meniu.h"
#include "pacient.h"
#include "sisteme_corp.h"
#include "tratamente.h"
#include "tipuri_patogeni.h"
#include <iostream>
void Simulator::porneste() {
    std::cout << "=== BINE AI VENIT IN SIMULATORUL MEDICAL ===\n\n";
    int joaca_din_nou = 1;
    Meniu& meniu = Meniu::getInstance();
    while (joaca_din_nou == 1) {
        CentruDeComanda::getInstance().resetare();
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
            std::cout << "[Farmacie] Medicamente active: "
                      << Medicament::getNumarMedicamenteActive() << "\n";
            meniu.afiseazaMeniuPrincipal();
            int alegere;
            if (!(std::cin >> alegere)) break;
            switch (alegere) {
                case 1:
                    std::cout << "\n--- Timpul trece... ---\n";
                    pacient.trece_o_ora();
                    pacient.afiseazaStareOrgane();
                    ora++;
                    break;
                case 2: {
                    meniu.afiseazaFarmacie();
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
                    meniu.afiseazaCatalogBoli();
                    int alegere_boala;
                    std::cin >> alegere_boala;

                    if (alegere_boala == 1) {
                        Virus* covid = new Virus("SARS-CoV-2", 15.0, "Plamani", 1.2);
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
                    comandament.cereAnalizaLLM(
                        pacient.getOxigen(),
                        pacient.getTemperatura(),
                        pacient.getBataiInima(),
                        pacient.getToxicitate(),
                        pacient.getIntegritateOrgan("Plamani"),
                        pacient.getIntegritateOrgan("Inima"),
                        pacient.getIntegritateOrgan("Creier"),
                        pacient.getIntegritateOrgan("Rinichi"),
                        pacient.getIntegritateOrgan("Ficat"),
                        pacient.getIntegritateOrgan("Maduva"),
                        ficat_blocat,
                        stomac_blocat
                    );
                    break;
                }
                case 5: {
                    std::cout << "\n[Test Exceptii] Introduceti codul de stimulare (911): ";
                    int cod;
                    std::cin >> cod;
                    pacient.stimuleazaSistemImunitar(cod);
                    pacient.afiseazaRaportSistemImunitar();
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
        std::cout << "\nVrei sa incerci din nou? (1 - Da, 0 - Iesire): ";
        if (!(std::cin >> joaca_din_nou)) {
            break;
        }
    }
}