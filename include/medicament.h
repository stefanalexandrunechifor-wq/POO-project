#pragma once
#include<string>
class Pacient;
class Medicament {
protected:
    std::string nume;
    double putere_vindecare;
    double daune_colaterale;
    int ore_active;
public:
    Medicament();
    Medicament(std::string nume, double putere,double daune, int ore);
    virtual ~Medicament() = default;
    void scadeTimp() { ore_active--; }
    int getOreActive() const { return ore_active; }
    virtual void administreaza(Pacient* pacient) = 0;
};