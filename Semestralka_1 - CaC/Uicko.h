#pragma once
#include "Firma.h"

class Uicko
{
public:
	Uicko();
	~Uicko();
	void hlavneMenu();
	void menuBiofarmar();
	void menuVozidla();
	void menuZakaznici();
	void menuObjednavky();
	Biofarmar* vytvorBiofarmara(string meno, bool z, bool o, bool och);
	Vozidlo* vytvorVozidlo(eVozidla typ);
	Zakaznik* vytvorZakaznika(string meno, int region);

	
private:
	Datum * d;
	Firma * firma;
};

