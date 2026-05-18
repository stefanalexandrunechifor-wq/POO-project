#pragma once
#include <iostream>
#include "patogen.h"

class Virus : public Patogen {
private:
    double rata_mutatie;
public:
    Virus(std::string nume, double putere, std::string organ, double mutatie);
    void ataca(Organ* victima, double multiplicator_daune) override;
    void primesteTratament(double putere_medicament) override;
    Patogen* clone() const override;
    [[nodiscard]] std::string getTip() const override { return "Virus"; }
};

class Bacterie : public Patogen {
private:
    double toxicitate;
    bool rezistenta_antibiotic;
public:
    Bacterie(std::string nume, double putere, std::string organ,
             double toxicitate, bool rezistenta_antibiotic);
    void ataca(Organ* victima, double multiplicator_daune ) override;
    void primesteTratament(double putere_medicament) override;
    [[nodiscard]] Patogen* clone() const override;
    [[nodiscard]] std::string getTip() const override { return "Bacterie"; }
};