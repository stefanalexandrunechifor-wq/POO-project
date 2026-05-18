#pragma once
#include <string>

class SecventaADN {
private:
    std::string cod_genetic;
public:
    explicit SecventaADN(std::string cod = "A-C-G-T");
    [[nodiscard]] const std::string& getCod() const;
    void suferaMutatie();
    void setCod(const std::string& cod_nou) {
        cod_genetic = cod_nou;
    }
};