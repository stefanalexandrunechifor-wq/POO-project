#include "pacient.h"
#include "organ.h"
#include "patogen.h"
#include "medicament.h"
int Pacient::id_generator = 1;
Pacient::Pacient(std::string nume) : id(id_generator++),nume(nume) {
    organe["Plamani"]   = new Organ("Plamani", 10000000, 0.02);
    organe["Sange"]     = new Organ("Sange", 15000000, 0.05);
    organe["Inima"]     = new Organ("Inima", 5000000, 0.00);
    organe["Ficat"]     = new Organ("Ficat", 8000000, 0.08);
    organe["Rinichi"]   = new Organ("Rinichi", 4000000, 0.03);
    organe["Stomac"]    = new Organ("Stomac", 3000000, 0.10);
    organe["Creier"]    = new Organ("Creier", 12000000, 0.00);
    organe["Maduva"]    = new Organ("Maduva", 2000000, 0.04);
    organe["Splina"]    = new Organ("Splina", 1500000, 0.03);
    organe["Piele"]     = new Organ("Piele", 20000000, 0.15);
}
Pacient::~Pacient() {
    for (auto& pereche : organe) {
        delete pereche.second;
    }
    organe.clear();
    for (Patogen* p : infectii_active) {
        delete p;
    }
    infectii_active.clear();
    for (Medicament* m : tratamente_active) {
        delete m;
    }
    tratamente_active.clear();
}

double Pacient::getOxigen() const {
    double integritate = organe.at("Plamani")->getIntegritate();
    return 98.0 * (integritate / 100.0);
}

double Pacient::getTemperatura() const {
    double temp_baza = 36.5;
    // TODO: Adaugi febra generata de fiecare patogen
    return temp_baza;
}

int Pacient::getBataiInima() const {
    const int bpm_baza = 70;
    const double oxigen_ideal = 98.0;
    const double factor_stres = 2.5;

    double oxigen_actual = getOxigen();
    double deficit = oxigen_ideal - oxigen_actual;
    if (deficit < 0) deficit = 0;

    int bpm_actual = bpm_baza + static_cast<int>(deficit * factor_stres);
    if (bpm_actual > 220) return 220;

    return bpm_actual;
}

void Pacient::trece_o_ora() {
    // TODO: actioneaza medicamentele(daor cele active,si cu efectele pozitive si cele negative)
    // TODO: actioneaza PATOGENII(se si inmultesc cele ramase, si ataca)
    // TODO: sistemul imunitar isi face si el treaba
    // TODO: sa gandesti cum ar putea sa se imbine patogeni sa se creeze mutatii(mai intai vezi cateva predefinite)
}

Organ* Pacient::getOrgan(std::string nume) {
    if(organe.find(nume) != organe.end()) return organe[nume];
    return nullptr;
}