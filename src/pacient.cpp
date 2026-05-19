#include"pacient.h"
#include"organ.h"
#include"patogen.h"
#include"medicament.h"
#include"sistem_imunitar.h"
#include<string>
#include<iostream>
#include<random>
#include <algorithm>
#include <cstdlib>
int Pacient::id_generator = 1;
Pacient::Pacient(const std::string& nume_pacient) : id(id_generator++), nume(nume_pacient) {
    organe["Plamani"]   = new Organ("Plamani", 100.0, 0.02);
    organe["Sange"]     = new Organ("Sange", 100.0, 0.05);
    organe["Inima"]     = new Organ("Inima", 100.0, 0.00);
    organe["Ficat"]     = new Organ("Ficat", 100.0, 0.08);
    organe["Rinichi"]   = new Organ("Rinichi", 100.0, 0.03);
    organe["Stomac"]    = new Organ("Stomac", 100.0, 0.10);
    organe["Creier"]    = new Organ("Creier", 100.0, 0.00);
    organe["Maduva"]    = new Organ("Maduva", 100.0, 0.04);
    organe["Splina"]    = new Organ("Splina", 100.0, 0.03);
    organe["Piele"]     = new Organ("Piele", 100.0, 0.15);
}
Pacient::~Pacient() {
    for (const auto& pereche : organe) {
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
    double integritate_plamani = organe.at("Plamani")->getIntegritate();
    double integritate_creier = organe.at("Creier")->getIntegritate();
    double oxigen_curent = 98.0 * (integritate_plamani / 100.0);
    if (integritate_creier <= 30.0) {
        double capacitate_neurologica = integritate_creier / 30.0; // un procent care scade spre 0
        oxigen_curent = oxigen_curent * capacitate_neurologica;
    }
    return oxigen_curent;
}

double Pacient::getTemperatura() const {
    return mediu.getTemperatura();
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
    //febra + sistem imunitar
    double putere_totala_boli = 0;
    for (const Patogen* boala : infectii_active) {
        putere_totala_boli += boala->getPutere();
    }
    double crestere_febra = std::min(6.0, putere_totala_boli * 0.005);
    const double temperatura_tinta = 36.5 + crestere_febra;
    if (temperatura_tinta > mediu.getTemperatura()) {
        mediu.cresteFebra(temperatura_tinta - mediu.getTemperatura());
    } else {
        mediu.scadeFebra(mediu.getTemperatura() - temperatura_tinta);
    }
    mediu.adaugaToxicitate(putere_totala_boli * 0.0015);
    std::cout << "[Vitals] Temperatura pacientului: " << mediu.getTemperatura()
              << " grade C | Toxicitate: " << mediu.getToxicitate() << "\n";

    double bonus_imunitate = 0.1;
    if (mediu.getTemperatura() >= 38.0 && mediu.getTemperatura() < 40.0) {
        std::cout << "[!] Febra moderata: Sistemul imunitar este stimulat!\n";
        bonus_imunitate = 0.2;
    }
    else if (mediu.getTemperatura() >= 40.0) {
        std::cout << "[!!!] FEBRA CRITICA: Creierul ia daune de la caldura!\n";
        Organ* creier = getOrgan("Creier");
        if (creier != nullptr) {
            creier->adaugaInfectie(15.0);
        }
        bonus_imunitate = 0.15;
    }
    std::ranges::sort(infectii_active, [](const Patogen* a, const Patogen* b) { return a->getPutere() > b->getPutere(); });
    imunitate.lanseazaAtac(infectii_active, bonus_imunitate);

    // atac patogeni + inmultire
    int numar_boli_initiale = infectii_active.size();
    for (int i = 0; i < numar_boli_initiale; i++) {
        Patogen* boala = infectii_active[i];
        if (boala->getPutere() <= 0) continue;
        VerificaMutatiiSimple(boala);
        VerificaMutatiiComplexe(boala);
        std::vector<std::pair<std::string, double>> lista_tinte = boala->getOrganTinta();
        for (auto const& tinta : lista_tinte) {
            Organ* organ_gasit = this->getOrgan(tinta.first);
            if (organ_gasit != nullptr) {
                boala->ataca(organ_gasit, tinta.second);
            } else {
                std::cout << "[-] Boala " << boala->getNume() << " a distrus deja organul " << tinta.first << ".\n";
            }
        }
        // sa nu se poate multiplica la infinit, devine imposibil de oprit
        if (boala->getPutere() > 20.0 && infectii_active.size() < 10) {
            Patogen* clona_noua = boala->clone();
            double putere_taiata = clona_noua->getPutere() * 0.70;
            clona_noua->primesteTratament(putere_taiata);

            infectii_active.push_back(clona_noua);
            std::cout << "[!!!] Alerta: Boala " << boala->getNume() << " s-a reprodus!\n";
        }
    }

    // medicamente
    for (int i = 0; i < int(tratamente_active.size()); i++) {
        Medicament* med = tratamente_active[i];
        med->administreaza(this);
        med->scadeTimp();

        if (med->getOreActive() <= 0) {
            std::cout << "Efectul medicamentului " << med->getName() << " a trecut.\n";
            delete med;
            tratamente_active.erase(tratamente_active.begin() + i);
            i--;
        }
    }

    //semne vitale noi
    double oxigen_curent = this->getOxigen();

    if (oxigen_curent < 80.0 && oxigen_curent >= 50.0) {
        std::cout << "[!] AVERTISMENT: Oxigen scazut (" << oxigen_curent << "%)! Pacientul hiperventileaza!\n";
    }
    else if (oxigen_curent < 50.0) {
        std::cout << "[!!!] ALERTA CRITICA: HIPOXIE (" << oxigen_curent << "%)! Organele cedeaza din lipsa de oxigen!\n";
        for (auto const& pereche : organe) {
            Organ* org = pereche.second;
            if (org != nullptr && org->getNume() != "Plamani") {
                org->adaugaInfectie(30.0);
            }
        }
    }

    for (auto const& pereche : organe) {
        Organ* org = pereche.second;
        if (org != nullptr) {
            org->evolutieInfectie();
            org->aplicaRegenerare();
            org->curataReceptor();
        }
    }

    imunitate.regenereazaArmata(this->getOxigen());
}
    Organ* Pacient::getOrgan(const std::string& organ_nume) {
        if (organe.find(organ_nume) != organe.end()) return organe[organ_nume];
        return nullptr;
    }

void Pacient::VerificaMutatiiSimple(Patogen* patogen) const {
    if (patogen == nullptr || patogen->getPutere() <= 150.0) {
        return;
    }

    const std::string adn_vechi = patogen->getADN();
    patogen->aplicaMutatieADN();
    std::string adn_nou = patogen->getADN();

    if (adn_nou == adn_vechi && !adn_nou.empty()) {
        adn_nou[0] = (adn_nou[0] == 'A') ? 'C' : 'A';
        patogen->setADN(adn_nou);
        adn_nou = patogen->getADN();
    }

    if (patogen->getNume().find("Varianta Delta") == std::string::npos) {
        patogen->setNume(patogen->getNume() + " Varianta Delta");
    }
    const std::string nou_organ = (std::rand() % 2 == 0) ? "Inima" : "Creier";
    patogen->adaugaOrganTinta(nou_organ, 1.5);

    std::cout << "[Mutatie simpla] " << patogen->getNume()
              << " si-a modificat ADN-ul (" << adn_vechi << " -> " << adn_nou
              << ") si ataca acum si " << nou_organ << "!\n";
}
bool Pacient::esteViu() const {
    if (getOxigen() <= 0.0 || mediu.getTemperatura()>42.5) {
        return false;
    }
    for (auto const& pereche : organe) {
        const Organ* org = pereche.second;
        if (org != nullptr && org->getIntegritate() <= 0.0) {
            return false;
        }
    }
    return true;
}
std::string Pacient::genereazaRaportDeces() const {
    if (mediu.getTemperatura() >= 42.5) {
        return "Soc hipertermic (Febra fatala de peste 42.5C).";
    } else if (getOxigen() <= 0) {
        return "Hipoxie severa (Sufocare).";
    }
    if (getOxigen() <= 0.0) {
        return "Asfixiere severa (Hipoxie). Oxigenul a ajuns la 0%.";
    }
    for (auto const& pereche : organe) {
        const Organ* org = pereche.second;
        if (org != nullptr && org->getIntegritate() <= 0.0) {
            return "Cedare organ vital: " + org->getNume() + " distrus complet.";
        }
    }
    return "Cauza necunoscuta.";
}
void Pacient::VerificaMutatiiComplexe(Patogen* patogen) {
    if (patogen == nullptr || patogen->getPutere() < 220.0) {
        return;
    }

    const std::string adn_vechi = patogen->getADN();
    patogen->aplicaMutatieADN();
    patogen->aplicaMutatieADN();
    const std::string adn_nou = patogen->getADN();

    if (patogen->getNume().find("Omega") == std::string::npos) {
        patogen->setNume(patogen->getNume() + " Omega");
    }
    patogen->adaugaOrganTinta("Rinichi", 1.3);
    patogen->adaugaOrganTinta("Ficat", 1.2);

    std::cout << "[Mutatie complexa] " << patogen->getNume()
              << " a evoluat agresiv. ADN: " << adn_vechi
              << " -> " << adn_nou << ". Tintele au fost extinse.\n";
}
void Pacient::adaugaInfectie(Patogen* boala) {
    if (boala != nullptr) {
        infectii_active.push_back(boala);
        std::cout << "[!] Pacientul a fost infectat cu " << boala->getNume() << "!\n";
    }
}
void Pacient::adaugaTratament(Medicament* med) {
    if (med != nullptr) {
        tratamente_active.push_back(med);
        std::cout << "[Farmacie] Pacientul a luat " << med->getName()
                  << ". Efectul va dura " << med->getOreActive() << " ore.\n";
    }
}
void Pacient::stimuleazaSistemImunitar(int cod_secret) {
    imunitate.stimuleazaArmata(cod_secret);
}
void Pacient::afiseazaRaportSistemImunitar() const {
    imunitate.afiseazaRaportArmata();
}
void Pacient::afiseazaStareOrgane() const {
    std::cout << "\n--- Status organe ---\n";
    for (const auto& pereche : organe) {
        const Organ* organ = pereche.second;
        if (organ == nullptr) {
            continue;
        }
        std::cout << organ->getNume()
                  << " | Sanatoase: " << organ->getSanatoase()
                  << " | Infectate: " << organ->getInfectate()
                  << " | Moarte: " << organ->getMoarte() << "\n";
    }
}
