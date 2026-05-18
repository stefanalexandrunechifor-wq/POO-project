#include"medicament.h"
Medicament::Medicament() : nume("Necunoscut"), putere_vindecare(0.0), daune_colaterale(0.0), ore_active(0) {}
Medicament::Medicament(std::string nume, double putere,double daune, int ore): nume(nume),putere_vindecare(putere),daune_colaterale(daune),ore_active(ore) {}
