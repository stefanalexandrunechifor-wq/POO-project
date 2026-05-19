#pragma once
#include"organ.h"
#include<string>
#include<vector>
#include"secventa_adn.h"
class Patogen {
protected:
    //std::string organ_tinta;
    std::string nume;
    double putere_infectie;  // incarcatura infectioasa curenta
    double factor_virulenta; // cat de mult rau produce aceeasi incarcatura
    double rata_replicare;   // crestere naturala pe ora fara tratament
    std::vector<std::pair<std::string, double>> organe_tinta;
    SecventaADN adn;
public:
    Patogen();
    Patogen(std::string nume_patogen, double putere, std::string organ_tinta);
    Patogen(const Patogen& altul);
    Patogen& operator=(const Patogen& altul);
    virtual ~Patogen() = default;
    [[nodiscard]] double getPutere() const {
        return putere_infectie;
    }
    [[nodiscard]] double getVirulenta() const {
        return factor_virulenta;
    }
    [[nodiscard]] double getRataReplicare() const {
        return rata_replicare;
    }
    void setVirulenta(double noua_virulenta) {
        factor_virulenta = noua_virulenta;
    }
    void setRataReplicare(double rata) {
        rata_replicare = rata;
    }
    void ajusteazaIncarcatura(double delta);
    virtual void evolueazaNatural(bool sub_tratament);
    virtual void ataca(Organ* victima, double multiplicator_daune) = 0;
    [[nodiscard]] virtual Patogen* clone() const =0;
    virtual void primesteTratament(double putere_medicament) = 0;
    [[nodiscard]] virtual std::string getTip() const = 0;
    [[nodiscard]] const std::vector<std::pair<std::string,double>>& getOrganTinta() const {
        return organe_tinta;
    }
    [[nodiscard]] const std::string& getNume() const {
        return nume;
    }
    void setNume(const std::string& n) {
        nume = n;
    }
    void adaugaOrganTinta(const std::string& organ, double multiplicator) {
        organe_tinta.emplace_back(organ, multiplicator);
    }
    [[nodiscard]] std::string getADN() const { return adn.getCod(); }
    void aplicaMutatieADN() { adn.suferaMutatie(); }
    void setADN(const std::string& cod_nou) {
        adn.setCod(cod_nou);
    }
};

