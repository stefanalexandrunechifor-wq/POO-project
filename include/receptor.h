#pragma once
template <typename T>
class Receptor {
private:
    T molecula_prinsa;
    bool este_ocupat;

public:
    // Constructor
    Receptor() {
        este_ocupat = false;
    }
    bool leagaMolecula(const T& moleculaNoua) {
        if (!este_ocupat) {
            molecula_prinsa = moleculaNoua;
            este_ocupat = true;
            return true;
        }
        return false;
    }

    bool verificaDacaEOcupat() const {
        return este_ocupat;
    }
};