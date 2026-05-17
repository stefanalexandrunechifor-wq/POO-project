#pragma once
#include "celula_imunitara.h"
class FabricaMaduva {
public:
    CelulaImunitara* produceCelula(std::string tip_celula, double oxigen_pacient) {
        if (oxigen_pacient < 40.0) {
            std::cout << "[!] Alerta: Oxigen prea scazut (" << oxigen_pacient << "%). Maduva nu poate produce " << tip_celula << "!\n";
            return nullptr;
        }
        if (tip_celula == "Macrofag") {
            return new Macrofag();
        }
        else if (tip_celula == "Celula_T") {
            return new Celula_T();
        }
        else if (tip_celula == "Celula_B") {
            return new Celula_B();
        }
        return nullptr;
    }
};
class SecventaADN {
private:
    std::string cod_genetic;
public:
    SecventaADN(std::string cod = "A-C-G-T");
    std::string getCod() const;
    void suferaMutatie() ;
    void setCod(std::string cod_nou) {
        cod_genetic = cod_nou;
    }
};
class MediuCorp {
private:
    double temperatura;
    double nivel_toxicitate;

public:
    MediuCorp() : temperatura(36.5), nivel_toxicitate(0.0) {}
    double getTemperatura() const { return temperatura; }
    double getToxicitate() const { return nivel_toxicitate; }
    void cresteFebra(double valoare) {
        temperatura += valoare;
        if (temperatura > 42.0) temperatura = 42.0;
    }
    void scadeFebra(double valoare) {
        temperatura -= valoare;
        if (temperatura < 36.5) temperatura = 36.5;
    }
    void adaugaToxicitate(double valoare) {
        nivel_toxicitate += valoare;
    }
    void setTemperatura(double t) { temperatura = t; }
};
class CentruDeComanda {

};