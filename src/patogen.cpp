#include"patogen.h"
#include <utility>
Patogen::Patogen() : nume(""), putere_infectie(0.0)  {}
Patogen::Patogen(std::string nume_patogen, double putere, std::string organ_tinta)
    : nume(std::move(nume_patogen)), putere_infectie(putere) {
    organe_tinta.push_back({std::move(organ_tinta), 1.0});
}
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
