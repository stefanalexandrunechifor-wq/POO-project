#pragma once
#include <exception>
#include <string>
class SuprasolicitareReceptorException : public std::exception {
private:
    std::string mesaj;
public:
    SuprasolicitareReceptorException(std::string nume_obiect) {
        mesaj = "[CRITIC] Exceptie Medicala: Receptorul entitatii '" + nume_obiect + "' este deja blocat!";
    }
    const char* what() const noexcept override {
        return mesaj.c_str();
    }
};