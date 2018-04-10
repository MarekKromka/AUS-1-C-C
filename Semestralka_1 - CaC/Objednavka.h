#pragma once

#include "Zakaznik.h"
#include "Konstanty.h"
#include <time.h> //obe na srand, rand
#include <stdlib.h>




class Objednavka
{
public:
	Objednavka(Zakaznik* zakaznik_,eTypProduktu typ, int mnozstvo_, string datumDorucenia);
	~Objednavka();
	void setCena(float cena);
	string getDatumDorucenia() {
		return datumDorucenia_;
	}
	int getMnozstvo() {
		return mnozstvo_;
	}
	eTypProduktu getTyp() {
		return typProduktu_;
	}
	void toString();
	eStavObjednavky getStav() {
		return stav_;
	}
	void setStav(eStavObjednavky stav) {
		stav_ = stav;
	}
	float getCenaZaJednotku() {
		return cenaZaJednotku_;
	}
	Zakaznik* getZakaznik() {
		return zakaznik_;
	}
	void setCenaZaJednotku(float cena) {
		cenaZaJednotku_ = cena;
	}
	void setZakaznik(Zakaznik * zak) {
		zakaznik_ = zak;
	}
private:
	
	Zakaznik* zakaznik_;
	eTypProduktu typProduktu_;
	int mnozstvo_;
	float cenaZaJednotku_;
	string datumDorucenia_;
	eStavObjednavky stav_;
	string stringStav();
};

