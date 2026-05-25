BioSimulator: Patient Zero
-------
Un simulator medical text-based unde joci rolul centrului de comanda al unui organism uman. Trebuie sa te mentii 
in viata in timp ce sistemul imunitar se lupta cu agenti patogeni care care sufera mutatii, se multiplica si ataca orageneele,
prabusind semnele vitale. Administreaza tratamente, urmareste evolutia infectiei, consulta asisetentul AI pentru a te salva,
dar ai grija costa.
Esti la butoane pacientul a fosgt infectat. Ai un dulap plin cu medicamente, un sistem imunitar care are nevoie de oxigen ca sa functioneze
si un ceas care ticaie. Nu lasa oxigenul sa scada sub 50%!!!

Quickstart Guide
-----
* Compilator compatibil cu C++17 sau C++20 (GCC/Clang/MSVC)
* CMake (v3.20+)

Setup & Build
---

**Cloneaza repository-ul:**
```markdown
git clone https://github.com/stefanalexandrunechifor-wq/POO-project.git
cd POO-project
```
**Creeaza executabilul cu CMake:**
```markdown
cmake -S . -B build
cmake --build build
```
**Ruleaza simularea:**
```markdown
./build/oop
```
Gameplay Features & Walkthrough
---
**Semnele Vitale (The Core Stats)**
Spre deosebire de un joc clasic cu "HP", supravietuirea pacientului depinde de o reactie in lant a semnelor vitale. 
Pierzi controlul unuia, le pierzi pe toate.

| Parametru | Valoare Normala | Conditie de Game Over / Efect Critic |
| :--- | :--- | :--- |
| **Oxigen** | 98.0% | Scade sub **50.0%** -> Hipoxie extrema. Toate organele iau damage. La **0%** pacientul se sufoca. |
| **Temperatura** | 36.5 C | Peste **38.0 C** -> Imunitatea primeste bonus. La **42.5 C** -> Soc hipertermic (Game Over). |
| **Puls** | 70 BPM | Creste masiv daca scade oxigenul. La **220 BPM** inima cedeaza. |
| **Integritate Organe** | 100.0 | Orice organ vital (Plamani, Inima, Creier, etc.) ajuns la **0.0** provoaca moartea subita. |

### Mecanicile Principale (The Game Loop)

Batalia se duce prin meniul principal. 
Fiecare actiune conteaza, dar trecerea timpului este singurul moment in care mecanicile biologice prind viata.

* **Lasa timpul sa treaca (1 Ora):** 
* Inima simularii. Patogenii (virusi, bacterii) ataca organele tinta si se inmultesc daca sunt destul de puternici. 
Febra creste in functie de incarcatura virala. Celulele T si Macrofagele scaneaza corpul si contraataca, dar au nevoie de un nivel bun de oxigen pentru a genera trupe noi.

* **Farmacie (Dulapul cu Medicamente):** 
Administreaza Antivirale (Tamiflu), Antibiotice (Amoxicilina) sau Antipiretice (Paracetamol). 
Atentie: Medicamentele au o durata limitata de actiune si blocheaza receptorii organelor! 
Nu poti da medicamente infinite fara sa risti o toxicitate.

* **Laborator Boli (Infectare Manuala):** Declanseaza scenarii personalizate injectand tulpini specifice:
    * `SARS-CoV-2`: Distruge plamanii, prabuseste oxigenul si declanseaza tahicardie.
    * `Streptococ`: O bacterie comuna, excelenta pentru testarea reactiei la antibiotice.
    * `HIV (Boss Battle)`: Ataca direct Maduva, "taind" liniile de aprovizionare ale sistemului imunitar pe termen lung.

* **Cere analiza AI-ului:** Trimite toate datele biometrice ale pacientului (Oxigen, 
Puls, starea celor 10 organe, blocajele receptorilor) catre un LLM extern. AI-ul va citi contextul medical exact si iti va recomanda urmatorii pasi clinici.

* **Stimulare Celule (Test Exceptii):** Practic zona de cheat code(**PS:trebuie gasite  codurile**)