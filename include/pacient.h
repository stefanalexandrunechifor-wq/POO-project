#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
class Organ;
class Patogen;
class Medicament;

class Pacient {
private:
    const int id;
    static int id_generator;
    std::unordered_map<std::string, Organ*> organe;
    std::vector<Patogen*> infectii_active;
    std::vector<Medicament*> tratamente_active;
    // Date istorice
    std::unordered_set<std::string> anticorpi;
    std::vector<std::string> carnet_vaccinari;

public:
    Pacient();
    ~Pacient();

    // Semne vitale și logică
    double getOxigen() const;
    double getTemperatura() const;
    int getBataiInima() const;
    void trece_o_ora();

    Organ* getOrgan(std::string nume);
};