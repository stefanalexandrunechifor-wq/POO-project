#pragma once
#include <string>
#include "receptor.h"
class Organ {
private:
    std::string nume;
    double cSanatoase, cInfectate, cMoarte;
    double rataRegenerare;
    double totalInitial;
    Receptor<std::string> receptor_tratament;
public:
    Organ(std::string organ_nume, double total, double reg);
    void aplicaRegenerare();
    void evolutieInfectie();
    void adaugaInfectie(double cantitate);
    [[nodiscard]] double getIntegritate() const {
        return (cSanatoase / totalInitial) * 100.0;
    }
    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] double getSanatoase() const { return cSanatoase; }
    [[nodiscard]] double getInfectate() const { return cInfectate; }
    [[nodiscard]] double getMoarte() const { return cMoarte; }
    bool primesteMedicament(const std::string& nume_medicament);
    void curataReceptor() ;
    [[nodiscard]] bool esteSubTratament() const { return receptor_tratament.verificaDacaEOcupat(); }
};
