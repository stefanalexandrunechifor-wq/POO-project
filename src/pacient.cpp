#include"pacient.h"
#include"organ.h"
#include"patogen.h"
#include"medicament.h"
#include"sistem_imunitar.h"
#include"tratamente.h"
#include"tipuri_patogeni.h"
#include<string>
#include<iostream>
#include<random>
#include <algorithm>
#include <cstdlib>
int Pacient::id_generator = 1;
Pacient::Pacient(const std::string& nume_pacient)
    : id(id_generator++), nume(nume_pacient), rezervor_hiv_global(0.0), ore_fara_antivirale(0) {
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
double Pacient::getToxicitate() const {
    return mediu.getToxicitate();
}
double Pacient::getIntegritateOrgan(const std::string& organ_nume) const {
    auto it = organe.find(organ_nume);
    if (it == organe.end() || it->second == nullptr) {
        return 0.0;
    }
    return it->second->getIntegritate();
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
bool Pacient::existaAntiviralActiv() const {
    for (const Medicament* medicament : tratamente_active) {
        if (dynamic_cast<const Antiviral*>(medicament) != nullptr) {
            return true;
        }
    }
    return false;
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

    bool are_antiviral_activ = existaAntiviralActiv();
    bool are_antibiotic_activ = false;
    bool are_antipiretic_activ = false;
    for (const Medicament* med : tratamente_active) {
        if (dynamic_cast<const Antibiotic*>(med) != nullptr) {
            are_antibiotic_activ = true;
        }
        if (dynamic_cast<const Antipiretic*>(med) != nullptr) {
            are_antipiretic_activ = true;
        }
    }

    double incarcatura_hiv = 0.0;
    for (const Patogen* boala : infectii_active) {
        if (boala != nullptr && boala->getTip() == "Virus" && boala->getNume().find("HIV") != std::string::npos) {
            incarcatura_hiv += boala->getPutere();
        }
    }
    if (incarcatura_hiv > 0.0) {
        if (are_antiviral_activ) {
            ore_fara_antivirale = 0;
            rezervor_hiv_global *= 0.97;
        } else {
            ore_fara_antivirale++;
            rezervor_hiv_global = std::min(180.0, rezervor_hiv_global + incarcatura_hiv * 0.08 + 0.5);
        }
        const double intensitate_imunosupresie =
            std::min(0.55, incarcatura_hiv / 220.0 + rezervor_hiv_global / 700.0);
        imunitate.aplicaImunosupresie(intensitate_imunosupresie);
        std::cout << "[HIV] Imunosupresie activa: -" << intensitate_imunosupresie * 100.0
                  << "% eficienta pe celulele adaptive.\n";
    } else {
        ore_fara_antivirale = 0;
        rezervor_hiv_global = std::max(0.0, rezervor_hiv_global - 1.5);
    }

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
    std::ranges::sort(infectii_active, [](const Patogen* a, const Patogen* b) {
        return (a->getPutere() * a->getVirulenta()) > (b->getPutere() * b->getVirulenta());
    });
    imunitate.lanseazaAtac(infectii_active, bonus_imunitate);

    // atac patogeni + inmultire
    const auto esteSubTratamentSpecific = [&](Patogen* boala) {
        if (boala == nullptr) {
            return false;
        }
        if (boala->getTip() == "Virus") {
            return are_antiviral_activ || are_antipiretic_activ;
        }
        if (boala->getTip() == "Bacterie") {
            return are_antibiotic_activ || are_antipiretic_activ;
        }
        return are_antipiretic_activ;
    };

    int numar_boli_initiale = infectii_active.size();
    for (int i = 0; i < numar_boli_initiale; i++) {
        Patogen* boala = infectii_active[i];
        if (boala->getPutere() <= 0) continue;
        VerificaMutatiiSimple(boala);
        VerificaMutatiiComplexe(boala);
        const bool boala_sub_tratament = esteSubTratamentSpecific(boala);
        boala->evolueazaNatural(boala_sub_tratament);
        std::vector<std::pair<std::string, double>> lista_tinte = boala->getOrganTinta();
        for (auto const& tinta : lista_tinte) {
            Organ* organ_gasit = this->getOrgan(tinta.first);
            if (organ_gasit != nullptr) {
                boala->ataca(organ_gasit, tinta.second);
            } else {
                std::cout << "[-] Boala " << boala->getNume() << " a distrus deja organul " << tinta.first << ".\n";
            }
        }
        // Multiplicarea e conditionata de resurse si de lipsa tratamentului eficient.
        if (!boala_sub_tratament && boala->getPutere() > 28.0 && infectii_active.size() < 12) {
            Patogen* clona_noua = boala->clone();
            clona_noua->ajusteazaIncarcatura(-(clona_noua->getPutere() * 0.80));

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
        const double severitate_hipoxie = std::clamp((50.0 - oxigen_curent) / 50.0, 0.0, 1.0);
        const double dauna_critica = 5.0 + severitate_hipoxie * 12.0;
        const char* organe_critice[] = {"Creier", "Inima", "Rinichi"};
        for (const char* nume_organ : organe_critice) {
            Organ* organ_critic = getOrgan(nume_organ);
            if (organ_critic != nullptr) {
                organ_critic->adaugaInfectie(dauna_critica);
            }
        }
        if (oxigen_curent < 35.0) {
            Organ* ficat = getOrgan("Ficat");
            if (ficat != nullptr) {
                ficat->adaugaInfectie(dauna_critica * 0.7);
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
    Organ* maduva = getOrgan("Maduva");
    const double integritate_maduva = (maduva != nullptr) ? maduva->getIntegritate() : 100.0;
    const double penalizare_hiv = std::clamp(rezervor_hiv_global / 300.0, 0.0, 0.45);
    const double oxigen_eficient = getOxigen() * (integritate_maduva / 100.0) * (1.0 - penalizare_hiv);
    imunitate.regenereazaArmata(oxigen_eficient);
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
    const double oxigen = getOxigen();
    const double temperatura = mediu.getTemperatura();
    const double toxicitate = mediu.getToxicitate();
    const int puls = getBataiInima();
    const double plamani = getIntegritateOrgan("Plamani");
    const double inima = getIntegritateOrgan("Inima");
    const double creier = getIntegritateOrgan("Creier");

    if (oxigen <= 0.0 || temperatura > 42.5) {
        return false;
    }
    if (toxicitate >= 26.0) {
        return false;
    }
    if (inima <= 8.0 || creier <= 8.0 || plamani <= 5.0) {
        return false;
    }
    if (puls >= 215 && oxigen < 55.0) {
        return false;
    }
    if (puls <= 30 && oxigen < 70.0) {
        return false;
    }
    int organe_vitale_sub_critice = 0;
    const double prag_sub_critic = 20.0;
    const char* organe_vitale[] = {"Plamani", "Inima", "Creier", "Rinichi", "Ficat"};
    for (const char* organ_nume : organe_vitale) {
        if (getIntegritateOrgan(organ_nume) < prag_sub_critic) {
            organe_vitale_sub_critice++;
        }
    }
    if (organe_vitale_sub_critice >= 3) {
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
    const double oxigen = getOxigen();
    const double temperatura = mediu.getTemperatura();
    const double toxicitate = mediu.getToxicitate();
    const int puls = getBataiInima();
    const double plamani = getIntegritateOrgan("Plamani");
    const double inima = getIntegritateOrgan("Inima");
    const double creier = getIntegritateOrgan("Creier");
    const double rinichi = getIntegritateOrgan("Rinichi");
    const double ficat = getIntegritateOrgan("Ficat");

    if (temperatura >= 42.5) {
        return "Soc hipertermic (Febra fatala de peste 42.5C).";
    }
    if (toxicitate >= 26.0) {
        return "Soc toxic sever (nivel toxicitate critic).";
    }
    if (inima <= 8.0 && oxigen < 70.0) {
        return "Stop cardio-respirator (inima decompensata pe fond hipoxic).";
    }
    if (creier <= 8.0 && temperatura >= 39.5) {
        return "Insuficienta neurologica majora (creier compromis + stres febril).";
    }
    if (plamani <= 5.0 || oxigen <= 0.0) {
        return "Hipoxie severa (Sufocare).";
    }
    if (puls >= 215 && oxigen < 55.0) {
        return "Aritmie maligna din tahicardie extrema si hipoxie.";
    }
    if (puls <= 30 && oxigen < 70.0) {
        return "Colaps circulator (bradicardie extrema cu perfuzie insuficienta).";
    }
    int organe_vitale_sub_critice = 0;
    const char* organe_vitale[] = {"Plamani", "Inima", "Creier", "Rinichi", "Ficat"};
    for (const char* organ_nume : organe_vitale) {
        if (getIntegritateOrgan(organ_nume) < 20.0) {
            organe_vitale_sub_critice++;
        }
    }
    if (organe_vitale_sub_critice >= 3) {
        return "Insuficienta multi-organ (cel putin 3 organe vitale sub prag critic).";
    }
    if (rinichi < 10.0 && ficat < 10.0 && toxicitate > 15.0) {
        return "Insuficienta metabolica terminala (ficat + rinichi).";
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
