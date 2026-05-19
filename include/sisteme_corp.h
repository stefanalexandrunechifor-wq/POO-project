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
    void cereAnalizaLLM(double oxigen,
                        double febra,
                        int puls,
                        double toxicitate,
                        double integritate_plamani,
                        double integritate_inima,
                        double integritate_creier,
                        double integritate_rinichi,
                        double integritate_ficat,
                        double integritate_maduva,
                        bool ficat_ocupat,
                        bool stomac_ocupat) {
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
        int indicatori_critici = 0;
        if (oxigen < 60.0 || integritate_plamani < 25.0) {
            indicatori_critici++;
            std::cout << " [CRITIC] Insuficienta respiratorie: oxigen " << oxigen
                      << "%, plamani " << integritate_plamani << "%.\n";
        } else if (oxigen < 90.0 || integritate_plamani < 65.0) {
            std::cout << " [AVERTISMENT] Hipoxie progresiva. Afectare pulmonara in curs.\n";
        } else {
            std::cout << " [INFO] Sistem respirator stabil.\n";
        }

        if (febra >= 40.0 && integritate_creier < 75.0) {
            indicatori_critici++;
            std::cout << " [CRITIC] Encefalopatie febrila: febra " << febra
                      << "C + creier " << integritate_creier << "%.\n";
        } else if (febra >= 38.0) {
            std::cout << " [INFO] Febra moderata. Imunitatea e stimulata, dar creste stresul sistemic.\n";
        }

        if (puls > 145 && oxigen < 80.0) {
            indicatori_critici++;
            std::cout << " [CRITIC] Tahicardie decompensata (" << puls
                      << " BPM) pe fond de hipoxie.\n";
        } else if (puls > 120) {
            std::cout << " [AVERTISMENT] Tahicardie compensatorie (" << puls << " BPM).\n";
        } else if (puls < 45 && oxigen < 70.0) {
            indicatori_critici++;
            std::cout << " [CRITIC] Bradicardie periculoasa cu perfuzie insuficienta.\n";
        }

        if (toxicitate >= 18.0) {
            indicatori_critici++;
            std::cout << " [CRITIC] Toxicitate severa (" << toxicitate
                      << "). Risc de soc toxic/metabolic.\n";
        } else if (toxicitate >= 10.0) {
            std::cout << " [AVERTISMENT] Toxicitate crescuta. Monitorizare hepato-renala necesara.\n";
        }

        if (integritate_ficat < 35.0 && integritate_rinichi < 35.0) {
            indicatori_critici++;
            std::cout << " [CRITIC] Filtru metabolic compromis (Ficat + Rinichi in insuficienta).\n";
        } else if (integritate_ficat < 55.0 || integritate_rinichi < 55.0) {
            std::cout << " [AVERTISMENT] Functia de detoxifiere este in scadere.\n";
        }

        if (integritate_inima < 30.0 || integritate_creier < 25.0) {
            indicatori_critici++;
            std::cout << " [CRITIC] Organ vital aproape de cedare (inima/creier).\n";
        }

        if (integritate_maduva < 45.0) {
            std::cout << " [AVERTISMENT] Maduva osoasa afectata. Regenerarea imunitara incetinita.\n";
        }

        if (oxigen < 75.0 && integritate_inima < 60.0) {
            std::cout << " [LEGATURA] Hipoxia forteaza inima -> risc major de stop cardio-respirator.\n";
        }
        if (toxicitate >= 10.0 && (integritate_ficat < 60.0 || integritate_rinichi < 60.0)) {
            std::cout << " [LEGATURA] Toxinele se acumuleaza deoarece organele de filtrare sunt slabite.\n";
        }
        if (febra >= 39.0 && integritate_creier < 70.0) {
            std::cout << " [LEGATURA] Febra inalta accelereaza deteriorarea neurologica.\n";
        }
        if (integritate_maduva < 50.0 && oxigen < 85.0) {
            std::cout << " [LEGATURA] Maduva + hipoxie: imunitatea nu mai poate compensa eficient infectia.\n";
        }

        if (ficat_ocupat) {
            std::cout << " [STRATEGIE] ATENTIE: Ficatul proceseaza deja pastile. NU administrati Antivirale sau Antipiretice acum!\n";
        }
        if (stomac_ocupat) {
            std::cout << " [STRATEGIE] ATENTIE: Stomacul proceseaza deja pastile. NU administrati Antibiotice acum!\n";
        }
        if (ficat_ocupat && toxicitate >= 12.0) {
            std::cout << " [STRATEGIE] Urgenta: evitati supradozarea hepatica pana scade toxicitatea.\n";
        }

        if (indicatori_critici >= 3) {
            std::cout << " [ALERTA MAXIMA] Risc de insuficienta multi-organ in urmatoarele ore.\n";
        } else if (indicatori_critici == 2) {
            std::cout << " [ALERTA] Stare instabila. Interventie rapida recomandata.\n";
        } else if (indicatori_critici == 0) {
            std::cout << " [INFO] Fara markeri de colaps imediat, dar monitorizarea continua este obligatorie.\n";
        }

        std::cout << "======================================================\n";
    }
};
