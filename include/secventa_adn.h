#pragma once
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