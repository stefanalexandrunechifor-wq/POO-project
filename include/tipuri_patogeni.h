#pragma once
#include<iostream>
#include"patogen.h"
class Virus : public Patogen{
private:
    double rata_mutatie;
public:
    Virus(std::string nume, double putere, std::string organ, double mutatie);
    void ataca(Organ* victima) override ;
    void primesteTratament(double putere_medicament) override;
    Patogen* clone() const override ;
};
class Bacterie : public Patogen {
private:
    double toxicitate;
    bool rezistenta_antibiotic;
public:
    void ataca(Organ* victima) override;
    void primesteTratament(double putere_medicament) override;
    Patogen* clone() const override;

};