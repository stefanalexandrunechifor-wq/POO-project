#include"patogen.h"
#include <algorithm>
#include <utility>
Patogen::Patogen() : nume(""), putere_infectie(0.0), factor_virulenta(1.0), rata_replicare(0.05)  {}
Patogen::Patogen(std::string nume_patogen, double putere, std::string organ_tinta)
    : nume(std::move(nume_patogen)), putere_infectie(putere), factor_virulenta(1.0), rata_replicare(0.05) {
    organe_tinta.push_back({std::move(organ_tinta), 1.0});
}
Patogen::Patogen(const Patogen& altul)
: nume(altul.nume), putere_infectie(altul.putere_infectie),
  factor_virulenta(altul.factor_virulenta), rata_replicare(altul.rata_replicare),
  organe_tinta(altul.organe_tinta) {
}
Patogen& Patogen::operator=(const Patogen& altul) {
    if (this != &altul) {
        this->nume = altul.nume;
        this->putere_infectie = altul.putere_infectie;
        this->factor_virulenta = altul.factor_virulenta;
        this->rata_replicare = altul.rata_replicare;
        this->organe_tinta = altul.organe_tinta;
    }
    return *this;
}
void Patogen::ajusteazaIncarcatura(double delta) {
    putere_infectie = std::clamp(putere_infectie + delta, 0.0, 350.0);
}
void Patogen::evolueazaNatural(bool sub_tratament) {
    if (putere_infectie <= 0.0) {
        return;
    }
    const double factor_evolutie = sub_tratament ? -0.03 : rata_replicare;
    ajusteazaIncarcatura(putere_infectie * factor_evolutie);
}
