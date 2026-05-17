#include"pacient.h"
#include"organ.h"
#include"patogen.h"
#include"medicament.h"
#include"sistem_imunitar.h"
#include<string>
#include<iostream>
#include<random>
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
    // TODO: Adaugi febra generata de fiecare patogen
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
    //febra+sistem imunitar
    double putere_totala_boli = 0;
    for (Patogen* boala : infectii_active) {
        putere_totala_boli += boala->getPutere();
    }

    mediu.setTemperatura(36.5 + (putere_totala_boli * 0.01));
    std::cout << "[Vitals] Temperatura pacientului: " << mediu.getTemperatura() << " grade C.\n";

    double bonus_imunitate = 1.0;
    if (mediu.getTemperatura() >= 38.0 && mediu.getTemperatura() < 40.0) {
        std::cout << "[!] Febra moderata: Sistemul imunitar este stimulat!\n";
        bonus_imunitate = 1.5;
    }
    else if (mediu.getTemperatura() >= 40.0) {
        std::cout << "[!!!] FEBRA CRITICA: Creierul ia daune de la caldura!\n";
        Organ* creier = getOrgan("Creier");
        if (creier != nullptr) {
            creier->adaugaInfectie(15.0);
        }
        bonus_imunitate = 1.2;
    }

    imunitate.lanseazaAtac(infectii_active, bonus_imunitate);
    //atc patogeni+inmultire
    int numar_boli_initiale = infectii_active.size();

    for (int i = 0; i < numar_boli_initiale; i++) {
        Patogen* boala = infectii_active[i];
        if (boala->getPutere() <= 0) {
            continue;
        }
        std::vector<std::pair<std::string, double>> lista_tinte = boala->getOrganTinta();

        for (auto const& tinta : lista_tinte) {
            std::string nume_organ = tinta.first;
            double multiplicator = tinta.second;

            Organ* organ_gasit = this->getOrgan(nume_organ);

            if (organ_gasit != nullptr) {
                boala->ataca(organ_gasit, multiplicator);
            } else {
                std::cout << "[-] Boala " << boala->getNume()
                          << " nu poate ataca " << nume_organ
                          << " (Organul a fost complet distrus!).\n";
            }
        }

        if (boala->getPutere() > 20.0) {
            Patogen* clona_noua = boala->clone();

            // puterea scade fata de cel original (fiind ca un pui)
            double putere_taiata = clona_noua->getPutere() * 0.70;
            clona_noua->primesteTratament(putere_taiata);

            infectii_active.push_back(clona_noua);
            std::cout << "[!!!] Alerta: Boala " << boala->getNume() << " s-a reprodus!\n";
        }
    }

    // Actioneaza medicamentele (o singura data pe ora, nu per patogen)
    for (int i = 0; i < (int)tratamente_active.size(); i++) {
        Medicament* med = tratamente_active[i];
        med->administreaza(this);
        med->scadeTimp();

        if (med->getOreActive() <= 0) {
            std::cout << "Efectul medicamentului " << med->getName() << " a trecut.\n";
            delete med;
            tratamente_active.erase(tratamente_active.begin() + i);
            i--;
        }
        //actualizare a semnelor vitale
        double oxigen_curent = this->getOxigen();

        if (oxigen_curent < 80.0 && oxigen_curent >= 50.0) {
            std::cout << "[!] AVERTISMENT: Oxigen scazut (" << oxigen_curent << "%)! Pacientul hiperventileaza!\n";
        }
        else if (oxigen_curent < 50.0) {
            std::cout << "[!!!] ALERTA CRITICA: HIPOXIE (" << oxigen_curent << "%)! Organele cedeaza din lipsa de oxigen!\n";
            for (auto const& pereche : organe) {
                Organ* org = pereche.second;
                if (org != nullptr && org->getNume() != "Plamani") {
                    org->adaugaInfectie(5000.0);
                }
            }
        }
    }
    for (auto const& pereche : organe) {
        Organ* org = pereche.second;
        if (org != nullptr) {
            org->evolutieInfectie();
            org->aplicaRegenerare();
        }
    }
    for (auto const& pereche : organe) {
        if (pereche.second != nullptr) {
            pereche.second->curataReceptor();
        }
    }
    imunitate.regenereazaArmata(this->getOxigen());
}
    Organ* Pacient::getOrgan(std::string nume) {
        if(organe.find(nume) != organe.end()) return organe[nume];
        return nullptr;
    }

void Pacient::VerificaMutatiiSimple(Patogen* boala) {
    // srand() se apeleaza o singura data in main(), nu la fiecare verificare
    int randomNum = rand() % 2;
    std::string organ_a = "Inima";
    std::string organ_b = "Creier";
    for (int i = (int)infectii_active.size() - 1; i >= 0; i--) {
        Patogen* patogen = infectii_active[i]; // redenumit pentru a evita shadow
        if (patogen->getPutere() > 150.0) {
            patogen->setNume(patogen->getNume() + " Varianta Delta");
            std::string nou_organ = (randomNum == 0) ? organ_a : organ_b;
            patogen->adaugaOrganTinta(nou_organ, 1.5);
            std::cout << "[Mutatie] " << patogen->getNume()
                      << " ataca acum si " << nou_organ << "!\n";
        }
    }
}
bool Pacient::esteViu() const {
    if (getOxigen() <= 0.0) {
        return false;
    }
    for (auto const& pereche : organe) {
        Organ* org = pereche.second;
        if (org != nullptr && org->getIntegritate() <= 0.0) {
            return false;
        }
    }
    return true;
}
std::string Pacient::genereazaRaportDeces() const {
    if (getOxigen() <= 0.0) {
        return "Asfixiere severa (Hipoxie). Oxigenul a ajuns la 0%.";
    }
    for (auto const& pereche : organe) {
        Organ* org = pereche.second;
        if (org != nullptr && org->getIntegritate() <= 0.0) {
            return "Cedare organ vital: " + org->getNume() + " distrus complet.";
        }
    }
    return "Cauza necunoscuta.";
}
void Pacient::VerificaMutatiiComplexe(Patogen* boala) {

}
void Pacient::adaugaInfectie(Patogen* boala) {
    if (boala != nullptr) {
        infectii_active.push_back(boala);
        std::cout << "[!] Pacientul a fost infectat cu " << boala->getNume() << "!\n";
    }
}