#pragma once
#include <string>
#include <iostream>
#include "patogen.h"
#include "receptor.h"
#include "exceptie.h"
class Patogen;
class CelulaImunitara {
protected:
    std::string nume;
    double putere_atac;
    Receptor<int> receptor_anticorpi;

public:
    CelulaImunitara() : nume("Celula Necunoscuta"), putere_atac(10.0) {}
    CelulaImunitara(std::string n, double putere) : nume(std::move(n)), putere_atac(putere) {}    CelulaImunitara(const CelulaImunitara& alta) = default;
    CelulaImunitara& operator=(const CelulaImunitara& alta) = default;
    virtual ~CelulaImunitara() = default;
    virtual void ataca(Patogen* boala, double bonus_febra) = 0;
    void primesteStimulent(int cod_secret) {
        if (receptor_anticorpi.leagaMolecula(cod_secret)) {
            this->putere_atac += 25.0;
            std::cout << "[+] Celula " << this->nume << " a absorbit stimulentul " << cod_secret << "!\n";
        } else {
            throw SuprasolicitareReceptorException(this->nume);
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const CelulaImunitara& c) {
        os << "Tip Celula: " << c.nume << " | Putere: " << c.putere_atac;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, CelulaImunitara& c) {
        std::cout << "Nume celula: ";
        is >> c.nume;
        std::cout << "Putere atac: ";
        is >> c.putere_atac;
        return is;
    }
    CelulaImunitara& operator+=(double bonus_putere) {
        this->putere_atac += bonus_putere;
        return *this;
    }
    [[nodiscard]] const std::string& getNume() const { return nume; }
    [[nodiscard]] double getPutere() const { return putere_atac; }
};

inline bool operator==(const CelulaImunitara& stanga, const CelulaImunitara& dreapta) {
    return (stanga.getNume() == dreapta.getNume() && stanga.getPutere() == dreapta.getPutere());
}

class Macrofag : public CelulaImunitara {
public:
    Macrofag() : CelulaImunitara("Macrofag", 15.0) {}

    void ataca(Patogen* boala, double bonus_febra) override {
        double daune_finale = putere_atac * bonus_febra;
        boala->primesteTratament(daune_finale);
    }
};

class Celula_T : public CelulaImunitara {
public:
    Celula_T() : CelulaImunitara("Celula T", 25.0) {}

    void ataca(Patogen* boala, double bonus_febra ) override {
        if (boala->getTip() == "Virus") {
            std::cout << "[Imunitate] Celula T loveste critic un virus!\n";
            if (boala->getNume() == "HIV")
                boala->primesteTratament((putere_atac * 1.0 * bonus_febra));
            else
            boala->primesteTratament(putere_atac * 2.0 * bonus_febra);
        } else {
            boala->primesteTratament(putere_atac * 0.2 * bonus_febra);
        }
    }
};

class Celula_B : public CelulaImunitara {
public:
    Celula_B() : CelulaImunitara("Celula B", 20.0) {}

    void ataca(Patogen* boala, double bonus_febra) override {
        if (boala->getTip() == "Bacterie") {
            std::cout << "[Imunitate] Celula B inunda bacteria cu anticorpi!\n";
            boala->primesteTratament(putere_atac * 2.0 * bonus_febra);
        } else {
            boala->primesteTratament(putere_atac * 0.2 * bonus_febra);
        }
    }
};
