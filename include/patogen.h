#pragma once
#include"organ.h"
#include<string>
#include<vector>
#include"sisteme_corp.h"
class Patogen {
protected:
    //std::string organ_tinta;
    double putere_infectie;
    std::string nume;
    std::vector<std::pair<std::string, double>> organe_tinta;
    SecventaADN adn;
public:
    Patogen();
    Patogen(std::string nume, double putere, std::string organ);
    Patogen(const Patogen& altul);
    Patogen& operator=(const Patogen& altul);
    virtual ~Patogen() = default;
    double getPutere() const {
        return putere_infectie;
    }
    virtual void ataca(Organ* victima, double multiplicator_daune = 1.0) = 0;
    virtual Patogen* clone() const = 0;
    virtual void primesteTratament(double putere_medicament) = 0;
    virtual std::string getTip() const = 0;
    const std::vector<std::pair<std::string,double>>& getOrganTinta() const {
        return organe_tinta;
    }
    const std::string& getNume() const {
        return nume;
    }
    void setNume(const std::string& n) {
        nume = n;
    }
    void adaugaOrganTinta(const std::string& organ, double multiplicator) {
        organe_tinta.push_back({organ, multiplicator});
    }
    std::string getADN() const { return adn.getCod(); }
    void aplicaMutatieADN() { adn.suferaMutatie(); }
    void setADN(std::string cod_nou) {
        adn.setCod(cod_nou);
    }
};

