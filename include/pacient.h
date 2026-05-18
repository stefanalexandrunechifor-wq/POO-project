#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include"sistem_imunitar.h"
class Organ;
class Patogen;
class Medicament;

class Pacient {
private:
    const int id;
    static int id_generator;
    MediuCorp mediu;
    std::string nume;
    std::unordered_map<std::string, Organ*> organe;
    std::vector<Patogen*> infectii_active;
    std::vector<Medicament*> tratamente_active;
    // Date istorice
    std::unordered_set<std::string> anticorpi;
    std::vector<std::string> carnet_vaccinari;
    SistemImunitar imunitate;

public:
    explicit Pacient(const std::string& nume_pacient);
    ~Pacient();

    // Semne vitale și logica
    double getOxigen() const;
    double getTemperatura() const;
    int getBataiInima() const;
    void trece_o_ora();
    const std::vector<Patogen*>& getInfectii() const {
        return infectii_active;
    }
    Organ* getOrgan(const std::string& organ_nume);
    void VerificaMutatiiSimple(Patogen*) const;
    void VerificaMutatiiComplexe(Patogen*);
    void adaugaInfectie(Patogen* boala);
    bool esteViu() const;
    std::string genereazaRaportDeces() const;
    void adaugaTratament(Medicament* med);
};
