#pragma once
#include<organ.h>
#include<string>
class Patogen {
protected:
    std::string organ_tinta;
    double putere_infectie;
    std::string nume;
public:
    Patogen();
    Patogen(std::string nume, double putere, std::string organ);
    Patogen(const Patogen& altul);
    Patogen& operator=(const Patogen& altul);
    virtual ~Patogen() = default;
    virtual void ataca(Organ* victima) = 0;
    virtual Patogen* clone() const = 0;
    virtual void primesteTratament(double putere_medicament) = 0;
};