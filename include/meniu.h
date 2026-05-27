#pragma once
class Meniu {
private:
    Meniu() = default;
public:
    Meniu(const Meniu&) =delete;
    Meniu& operator=(const Meniu&) = delete;
    static Meniu& getInstance();
    void afiseazaMeniuPrincipal() const;
    void afiseazaFarmacie() const;
    void afiseazaCatalogBoli() const;
};