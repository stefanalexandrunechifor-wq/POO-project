#include"patogen.h"
Patogen::Patogen() : putere_infectie(0.0), nume("") {}
Patogen::Patogen(std::string nume, double putere, std::string organ)
: nume(nume),putere_infectie(putere) {
organe_tinta.push_back({organ, 1.0});}
Patogen::Patogen(const Patogen& altul)
: nume(altul.nume), putere_infectie(altul.putere_infectie), organe_tinta(altul.organe_tinta) {
}
Patogen& Patogen::operator=(const Patogen& altul) {
    if (this != &altul) {
        this->nume = altul.nume;
        this->putere_infectie = altul.putere_infectie;
        this->organe_tinta = altul.organe_tinta;
    }
    return *this;
}