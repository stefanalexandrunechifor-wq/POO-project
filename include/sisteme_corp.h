#pragma once
#include "celula_imunitara.h"
#include <string>
class CelulaImunitara;
class FabricaMaduva {
public:
    CelulaImunitara* produceCelula(const std::string& tip_celula, double oxigen_pacient);
};
class MediuCorp {
private:
    double temperatura;
    double nivel_toxicitate;

public:
    MediuCorp() : temperatura(36.5), nivel_toxicitate(0.0) {}
    [[nodiscard]] double getTemperatura() const { return temperatura; }
    [[nodiscard]] double getToxicitate() const { return nivel_toxicitate; }
    void cresteFebra(double valoare) {
        temperatura += valoare;
        if (temperatura > 42.0) temperatura = 42.0;
    }
    void scadeFebra(double valoare) {
        temperatura -= valoare;
        if (temperatura < 36.5) temperatura = 36.5;
    }
    void adaugaToxicitate(double valoare) {
        nivel_toxicitate += valoare;
    }
    void setTemperatura(double t) { temperatura = t; }
};
class CentruDeComanda {
private:
    int jetoane_ai;
    CentruDeComanda() : jetoane_ai(3) {}

public:
    CentruDeComanda(const CentruDeComanda&) = delete;
    CentruDeComanda& operator=(const CentruDeComanda&) = delete;
    static CentruDeComanda& getInstance() {
        static CentruDeComanda instanta;
        return instanta;
    }
    void cereAnalizaLLM(double oxigen, double febra, bool ficat_ocupat, bool stomac_ocupat) {
        if (jetoane_ai <= 0) {
            std::cout << "\n[!] EROARE: Spitalul a ramas fara fonduri. Asistentul este indisponibil!\n";
            return;
        }
        jetoane_ai--;
        std::cout << "\n======================================================\n";
        std::cout << "  [CentruDeComanda] Analiza Sistem Expert Medical... \n";
        std::cout << "  Consultari ramase: " << jetoane_ai << "\n";
        std::cout << "======================================================\n";
        std::cout << "-> GENERARE RAPORT DE RISC:\n\n";
        if (oxigen < 60.0) {
            std::cout << " [CRITIC] Nivel oxigen (" << oxigen << "%). Sistem respirator in colaps!\n";
        } else if (oxigen < 90.0) {
            std::cout << " [AVERTISMENT] Hipoxie usoara. Boala afecteaza Plamanii.\n";
        } else {
            std::cout << " [INFO] Sistem respirator stabil.\n";
        }
        if (febra >= 40.0) {
            std::cout << " [CRITIC] Temperatura extrema (" << febra << "C). Risc de daune pe Creier!\n";
        } else if (febra >= 38.0) {
            std::cout << " [INFO] Febra moderata. Sistemul imunitar primeste un bonus de atac.\n";
        }
        if (ficat_ocupat) {
            std::cout << " [STRATEGIE] ATENTIE: Ficatul proceseaza deja pastile. NU administrati Antivirale sau Antipiretice acum!\n";
        }
        if (stomac_ocupat) {
            std::cout << " [STRATEGIE] ATENTIE: Stomacul proceseaza deja pastile. NU administrati Antibiotice acum!\n";
        }

        std::cout << "======================================================\n";
    }
};
