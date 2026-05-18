#pragma once
#include <iostream>
#include "patogen.h"

class Virus : public Patogen {
private:
    double rata_mutatie;
public:
    Virus(const std::string& nume_virus, double putere, const std::string& organ_tinta, double mutatie);
    void ataca(Organ* victima, double multiplicator_daune) override;
    void primesteTratament(double putere_medicament) override;
    [[nodiscard]] Patogen* clone() const override;
    [[nodiscard]] std::string getTip() const override { return "Virus"; }
};

class Bacterie : public Patogen {
private:
    double toxicitate;
    bool rezistenta_antibiotic;
public:
    Bacterie(const std::string& nume_bacterie, double putere, const std::string& organ_tinta,
             double toxicitate, bool rezistenta_antibiotic);
    void ataca(Organ* victima, double multiplicator_daune ) override;
    void primesteTratament(double putere_medicament) override;
    [[nodiscard]] Patogen* clone() const override;
    [[nodiscard]] std::string getTip() const override { return "Bacterie"; }
};
