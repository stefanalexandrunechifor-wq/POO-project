#pragma once
#include<string>
class Pacient;
class Medicament {
protected:
    std::string nume;
    double putere_vindecare;
    double daune_colaterale;
    int ore_active;
    static int numar_total_medicamente;
public:
    Medicament();
    Medicament(std::string nume, double putere, double daune, int ore);
    virtual ~Medicament();
    void scadeTimp() { ore_active--; }
    [[nodiscard]] int getOreActive() const { return ore_active; }
    virtual void administreaza(Pacient* pacient) = 0;
    std::string getName(){return nume;}
    [[nodiscard]] static int getNumarMedicamenteActive() {
        return numar_total_medicamente;
    }
};