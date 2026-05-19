#include <iostream>
#include <algorithm>
#include "organ.h"
#include "tipuri_patogeni.h"

Bacterie::Bacterie(const std::string& nume_bacterie, double putere, const std::string& organ_tinta,
                   double tox, bool rezistenta)
    : Patogen(nume_bacterie, putere, organ_tinta), toxicitate(tox), rezistenta_antibiotic(rezistenta) {
    setVirulenta(1.0 + toxicitate * 0.6);
    setRataReplicare(0.03 + toxicitate * 0.05);
}

Virus::Virus(const std::string& nume_virus, double putere, const std::string& organ_tinta, double mutatie)
    : Patogen(nume_virus, putere, organ_tinta), rata_mutatie(mutatie), rezervor_latent(0.0), ore_fara_antivirale(0) {
    if (esteHIV()) {
        setVirulenta(0.45);
        setRataReplicare(0.10);
        rezervor_latent = 18.0;
    } else if (nume.find("SARS") != std::string::npos || nume.find("COVID") != std::string::npos) {
        setVirulenta(1.35);
        setRataReplicare(0.07);
    } else {
        setVirulenta(1.0);
        setRataReplicare(0.08);
    }
}
bool Virus::esteHIV() const {
    return nume.find("HIV") != std::string::npos;
}
void Virus::ataca(Organ* victima, double multiplicator_daune) {
    const double factor_mutatie = 1.0 + rata_mutatie * 0.35;
    const double daune = (putere_infectie * factor_virulenta * factor_mutatie) * multiplicator_daune;
    std::cout << "Virusul " << nume << " ataca organul " << victima->getNume()
              << " cu putere " << daune << "!\n";
    victima->adaugaInfectie(daune);
}

void Virus::primesteTratament(double putere_medicament) {
    double eficienta = putere_medicament;
    if (esteHIV()) {
        eficienta *= 0.75;
    }
    ajusteazaIncarcatura(-eficienta);
}
void Virus::evolueazaNatural(bool sub_tratament) {
    if (putere_infectie <= 0.0) {
        return;
    }
    if (esteHIV()) {
        if (sub_tratament) {
            ore_fara_antivirale = 0;
            rezervor_latent *= 0.98;
            ajusteazaIncarcatura(-(1.5 + putere_infectie * 0.04));
            return;
        }

        ore_fara_antivirale++;
        rezervor_latent = std::min(120.0, rezervor_latent + putere_infectie * 0.12 + 0.8);
        ajusteazaIncarcatura(std::max(1.0, putere_infectie * getRataReplicare()));

        if (putere_infectie < 16.0 && rezervor_latent > 12.0 && ore_fara_antivirale >= 3) {
            const double rebound = std::min(20.0, rezervor_latent * 0.15);
            ajusteazaIncarcatura(rebound);
            rezervor_latent = std::max(0.0, rezervor_latent - rebound * 0.5);
            std::cout << "[HIV] Reactivare din rezervor latent: +" << rebound << " incarcatura.\n";
        }
        return;
    }

    const double factor_mutatie = 1.0 + rata_mutatie * 0.30;
    if (sub_tratament) {
        ajusteazaIncarcatura(-putere_infectie * 0.02);
    } else {
        ajusteazaIncarcatura(putere_infectie * getRataReplicare() * factor_mutatie);
    }
}
Patogen* Virus::clone() const {
    Virus* clona = new Virus(*this);
    clona->aplicaMutatieADN();
    return clona;
}

void Bacterie::ataca(Organ* victima, double multiplicator_daune) {
    const double daune = (putere_infectie * factor_virulenta + toxicitate * 8.0) * multiplicator_daune;
    std::cout << "Bacteria " << nume << " elibereaza toxine in " << victima->getNume()
              << " cu putere " << daune << "!\n";

    victima->adaugaInfectie(daune);
}

void Bacterie::primesteTratament(double putere_medicament) {
    double daune_primite = putere_medicament;
    if (rezistenta_antibiotic == true) {
        std::cout << "Bacteria " << nume << " a rezistat tratamentului!\n";
        daune_primite = putere_medicament * 0.25;
    }
    ajusteazaIncarcatura(-daune_primite);
}
Patogen* Bacterie::clone() const {
    Bacterie* clona = new Bacterie(*this);
    clona->aplicaMutatieADN();
    return clona;
}
