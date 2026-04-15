#pragma once
#include <string>
class Organ {
private:
    std::string nume;
    double cSanatoase, cInfectate, cMoarte;
    double rataRegenerare;
    double totalInitial;
public:
    Organ(std::string nume, double total, double reg);
    void aplicaRegenerare();
    void evolutieInfectie();
    void adaugaInfectie(double cantitate);
    double getIntegritate() const {
        return (cSanatoase / totalInitial) * 100.0;
    }
    std::string getNume() const { return nume; }
    double getSanatoase() const { return cSanatoase; }
    double getInfectate() const { return cInfectate; }
    double getMoarte() const { return cMoarte; }
};