#include"patogen.h"
Patogen::Patogen(std::string nume, double putere, std::string organ)
: nume(nume),putere_infectie(putere),organ_tinta(organ){}
Patogen::Patogen(const Patogen& altul)
: nume(altul.nume), putere_infectie(altul.putere_infectie), organ_tinta(altul.organ_tinta) {
}
Patogen& Patogen::operator=(const Patogen& altul) {
    if (this != &altul) {
        this->nume = altul.nume;
        this->putere_infectie = altul.putere_infectie;
        this->organ_tinta = altul.organ_tinta;
    }
    return *this;
}