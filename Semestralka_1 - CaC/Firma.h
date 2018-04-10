#pragma once



#include "moje\Biofarmar.h"
#include "Vozidlo.h"
#include "Datum.h"
#include "Zakaznik.h"
#include "Objednavka.h"
#include "Konstanty.h"
#include "structures\priority_queue\priority_queue_linked_list.h"
#include "structures\priority_queue\heap.h"
#include "structures\queue\explicit_queue.h"

//#include "cereal/archives/binary.hpp"	// Serializacia a Deserializacia

#include <fstream>
#include <sstream>

#include <stdio.h>  

class evidenciaVozidla
{
public:
	evidenciaVozidla(Vozidlo * novyVoz, string datum);
	Vozidlo& getVozidlo();
	Vozidlo* dajVozidlo();
	string getDatum();
	void setDatum(string dat) {
		datumEvidencie_ = dat;
	}
	~evidenciaVozidla();


private:
	Vozidlo * voz_;
	string datumEvidencie_;
};

class evidenciaObjednavky
{
public:
	evidenciaObjednavky(Objednavka* novaObjednavka, string datum);
	Objednavka* getObjednavka();
	string getDatum();
	~evidenciaObjednavky();
	friend ostream& operator<< (ostream& os, evidenciaObjednavky& eObj);
	friend istream& operator>> (istream& is, evidenciaObjednavky& eObj);
private:
	Objednavka * objednavka_;
	string datumEvidencie_;

};

class Nakup {
public:
	Nakup(float mnozstvo,eTovary typTovaru);
	~Nakup();
	eTovary getTyp() {
		return typTovaru_;
	}
	float getMnozstvo() {
		return mnozstvo_;
	}
private:
	float mnozstvo_;
	eTovary typTovaru_;
};


class Firma
{
public:
	Firma();
	~Firma();
	void pridajBiofarmara(Biofarmar * novyFarmar);
	void pridajVozidlo(Vozidlo * noveVozidlo);
	void pridajZakaznika(Zakaznik * novyZakaznik);
	Objednavka* vytvorObjednavku(Zakaznik * zakaznik, eTypProduktu typ, int mnozstvo, string datumDorucenia);
	void pridajObjednavku(Objednavka* novaObjednavka);

	void vypisZoznamBiofarmarov(const eTovary tovar);
	void vypisZoznamVozidiel();
	void vypisanieObjednavokNasledujuciDen();
	void vypisanieZakaznikov(int region, string datumOd, string datumDo);
	void vypisanieZrealizovychObjednavok(string datumOd, string datumDo);
	void vypisanieZrusenychObjednavok(string datumOd, string datumDo);
	void najOblubenejsiFarmar(eTovary tovar);

	
	void kontrolaNaSedemDni();
	void kontrolaNaZajtra();
	bool kontrolaZasobZajtra(int pocetHranoliek, int pocetLupienkov);
	void kontrolaZasobNaSedem(int pocetHranoliek, int pocetLupienkov);
	void dokupSuroviny(eTovary druhTovaru, int mnozstvo);

	Zakaznik * najdiZakaznikaPodlaMena(string meno);
	void spracujObiednavkyNaZajtra();
	void vyrobProdukty(int pocetHranoliek, int pocetLupienkov);

	void dalsiDen();

	//TESTOVACIE
	void vypisanieVsetkychObjednavok();
	
	
	friend ostream& operator<< (ostream& os, Firma& firma);
	friend istream& operator>> (istream& is, Firma& firma);

private:
	structures::LinkedList<Biofarmar*> * arrayBiofarmarov;
	structures::ArrayList<evidenciaVozidla*> * arrayVozidiel;
	structures::LinkedList<evidenciaObjednavky*> * linkListObjednavok;
	structures::LinkedList<Zakaznik*> * linkListZakaznikov;
	structures::ExplicitQueue<Nakup *> * nakupenyTovar;
	
	bool skontrolujKapacituAuta(int mnozstvo, eTypProduktu typ,std::string datum);

	int pocetMraziacichAut_= 0;
	int pocetLupienkovychAut_ = 0;
	int kapacitaMraziarenskych_ = 0;
	int kapacitaLupienkovych_ = 0;
	float skladZemiakov_ = 0;
	float skladOleja_ = 0;
	float skladOchu_ = 0;

	void naskladniTovar();
};

