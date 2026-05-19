#pragma once
#include <iostream>
#include "patogen.h"

class Virus : public Patogen {
private:
    double rata_mutatie;
    double rezervor_latent;
    int ore_fara_antivirale;
public:
    Virus(const std::string& nume_virus, double putere, const std::string& organ_tinta, double mutatie);
    Virus(const Virus&) = default;
    Virus& operator=(const Virus&) = default;
    void ataca(Organ* victima, double multiplicator_daune) override;
    void primesteTratament(double putere_medicament) override;
    void evolueazaNatural(bool sub_tratament) override;
    [[nodiscard]] Patogen* clone() const override;
    [[nodiscard]] std::string getTip() const override { return "Virus"; }
    [[nodiscard]] bool esteHIV() const;
};

class Bacterie : public Patogen {
private:
    double toxicitate;
    bool rezistenta_antibiotic;
public:
    Bacterie(const std::string& nume_bacterie, double putere, const std::string& organ_tinta,
             double toxicitate, bool rezistenta_antibiotic);
    Bacterie(const Bacterie&) = default;
    Bacterie& operator=(const Bacterie&) = default;
    void ataca(Organ* victima, double multiplicator_daune ) override;
    void primesteTratament(double putere_medicament) override;
    [[nodiscard]] Patogen* clone() const override;
    [[nodiscard]] std::string getTip() const override { return "Bacterie"; }
};

