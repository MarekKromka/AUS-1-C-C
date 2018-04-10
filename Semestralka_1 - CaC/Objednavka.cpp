#include "Objednavka.h"



Objednavka::Objednavka(Zakaznik* zakaznik, eTypProduktu typ, int mnozstvo, string datumDorucenia)
	:zakaznik_(zakaznik),mnozstvo_(mnozstvo),typProduktu_(typ),datumDorucenia_(datumDorucenia)
{
	
	cenaZaJednotku_ = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 4));   //  random cislo z intervalu 0-4
}


Objednavka::~Objednavka()
{
	
}

void Objednavka::setCena(float cena)
{
	cenaZaJednotku_ = cena;
}

void Objednavka::toString()
{
	
	cout << fixed << setprecision(2) << " Meno: " << zakaznik_->getMeno() + " R" << zakaznik_->getAdresa() << (((int)typProduktu_ == 0) ? " Lupienky \t\t" : " Hranolceky ") << mnozstvo_ << "kg \t" <<
		"Jednotkova cena =" << cenaZaJednotku_ << "e Total =" << cenaZaJednotku_ * mnozstvo_ << "e " << stringStav() << endl;
}

string Objednavka::stringStav()
{
	string pom;
	switch (stav_) {
	case eStavObjednavky::PRIJATA:
		pom = "PRIJATA";
		break;
	case eStavObjednavky::ZAMIETNUTA:
		pom = "ZAMIETNUTA";
		break;
	case eStavObjednavky::ZRUSENA:
		pom = "ZRUSENA";
		break;
	case eStavObjednavky::ZREALIZOVANA:
		pom = "ZREALIZOVANA";
		break;
	}
	return pom;
}

