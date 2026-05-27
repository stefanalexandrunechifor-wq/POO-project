#include"meniu.h"
#include<iostream>

Meniu& Meniu::getInstance() {
    static Meniu instance;
    return instance;
}
void Meniu::afiseazaMeniuPrincipal() const {
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
void Meniu::afiseazaFarmacie() const {
    std::cout << "\n--- DULAPUL CU MEDICAMENTE ---\n";
    std::cout << "1. Tamiflu (Antiviral - Tinta: Ficat) | Putere: 50\n";
    std::cout << "2. Amoxicilina (Antibiotic - Tinta: Stomac) | Putere: 45\n";
    std::cout << "3. Paracetamol (Antipiretic - Tinta: Ficat) | Putere: 20\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Alege medicamentul pentru administrare: ";
}
void Meniu::afiseazaCatalogBoli() const {
    std::cout << "\n--- LABORATOR PATOGENI ---\n";
    std::cout << "1. SARS-CoV-2 (Virus agresiv)\n";
    std::cout << "2. Streptococ (Bacterie comuna)\n";
    std::cout << "3. HIV (Boss Battle - Tinta: Maduva)\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Alege tulpina: ";
}