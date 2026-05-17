#pragma once
#include"medicament.h"
class Antibiotic : public Medicament{
private:
public:
    Antibiotic(std::string n, double putere, double daune, int ore);
    void administreaza(Pacient* pacient) override;
};
class Antiviral : public Medicament {
private:
public:
    Antiviral(std::string n, double putere, double daune, int ore);
    void administreaza(Pacient* pacient) override;
};
class Antipiretic : public Medicament {
private:
public:
    Antipiretic(std::string n, double putere, double daune, int ore);
    void administreaza(Pacient* pacient) override;
};