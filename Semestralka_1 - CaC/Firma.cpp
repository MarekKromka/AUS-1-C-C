#include "Firma.h"



Firma::Firma()
{
	arrayBiofarmarov = new structures::LinkedList<Biofarmar*>();
	arrayVozidiel = new structures::ArrayList<evidenciaVozidla*>();
	linkListObjednavok = new structures::LinkedList<evidenciaObjednavky*>();
	linkListZakaznikov = new structures::LinkedList<Zakaznik*>();
	nakupenyTovar = new structures::ExplicitQueue<Nakup*>();
}



void Firma::pridajBiofarmara(Biofarmar * novyFarmar)
{
	int index = 0;
	for (Biofarmar* var : *arrayBiofarmarov) {
		if (novyFarmar->getMeno() <= var->getMeno()) {
			arrayBiofarmarov->insert(novyFarmar, index);
			return;
		}
		index++;
	}
	arrayBiofarmarov->add(novyFarmar);

}

void Firma::pridajZakaznika(Zakaznik * novyZakaznik)
{
	int index = 0;
	for (Zakaznik* var : *linkListZakaznikov) {
		if (novyZakaznik->getMeno() <= var->getMeno()) {
			linkListZakaznikov->insert(novyZakaznik, index);
			return;
		}
		index++;
	}
	linkListZakaznikov->add(novyZakaznik);
}

void Firma::pridajVozidlo(Vozidlo * noveVozidlo)
{
	evidenciaVozidla * pom = new evidenciaVozidla(noveVozidlo, Datum::getAktualnyDatum());
	arrayVozidiel->add(pom);
	
	if (noveVozidlo->getTyp() == eVozidla::MRAZENE) {
		pocetMraziacichAut_++;
		kapacitaMraziarenskych_ += noveVozidlo->getNosnost();
	}
	else {
		pocetLupienkovychAut_++;
		kapacitaLupienkovych_ += noveVozidlo->getNosnost();
	}
}

void Firma::pridajObjednavku(Objednavka* novaObjednavka)
{
	if (novaObjednavka != nullptr) {
		string datumDorucenia = novaObjednavka->getDatumDorucenia();
		time_t pomocna = Datum::string_to_time_t(datumDorucenia);
		time_t aktDatum = Datum::string_to_time_t(Datum::getAktualnyDatum());
		aktDatum += 7 * 24 * 60 * 60;

		if (pomocna >= aktDatum)
		{
			evidenciaObjednavky * nova = new evidenciaObjednavky(novaObjednavka, Datum::getAktualnyDatum());
			linkListObjednavok->add(nova); 
			cout << "Objednavka uspesne pridana" << endl;
			(skontrolujKapacituAuta(novaObjednavka->getMnozstvo(), novaObjednavka->getTyp(), novaObjednavka->getDatumDorucenia()))
				? nova->getObjednavka()->setStav(eStavObjednavky::PRIJATA) : nova->getObjednavka()->setStav(eStavObjednavky::ZAMIETNUTA);


		}
		else {
			cout << "Objednavka sa nepridala do systemu z dovodu zadania datumu dorucenia skor ako 7 dni." << endl;
			delete novaObjednavka;
		}
	}
}


Objednavka * Firma::vytvorObjednavku(Zakaznik * zakaznik, eTypProduktu typ, int mnozstvo, string datumDorucenia)
{
	int pom = linkListZakaznikov->getIndexOf(zakaznik);
	if (pom == -1) {
		cout << "Zakaznik neexistuje / nieje v databaze." << endl;
		return nullptr;
	}
	else {
		Objednavka * o = new Objednavka(zakaznik, typ, mnozstvo, datumDorucenia);
		return o;
	}
}

void Firma::kontrolaNaSedemDni()
{	// SPOCITA POCET OBJEDNANYCH TOVAROV
	//string * pom;
	int pocetHranoliek = 0;
	int pocetLupienkov = 0;

	for (evidenciaObjednavky* var : *linkListObjednavok) {
		Objednavka * pom = var->getObjednavka();
		if (pom->getStav() == eStavObjednavky::PRIJATA) {
			if (Datum::string_to_time_t(pom->getDatumDorucenia()) > (Datum::string_to_time_t(Datum::getAktualnyDatum()) + 24 * 60 * 60)
				&& Datum::string_to_time_t(pom->getDatumDorucenia()) <= (Datum::string_to_time_t(Datum::getAktualnyDatum()) + 7 * 24 * 60 * 60)) {
				if (pom->getTyp() == eTypProduktu::HRANOLCEKY) {
					pocetHranoliek += pom->getMnozstvo();
				}
				else {
					pocetLupienkov += pom->getMnozstvo();
				}
			}
		}
	}

	kontrolaZasobNaSedem(pocetHranoliek, pocetLupienkov);

}

void Firma::kontrolaNaZajtra()
{
	int pocetHranoliek = 0;
	int pocetLupienkov = 0;

	for (evidenciaObjednavky* var : *linkListObjednavok) {
		Objednavka * pom = var->getObjednavka();
		if (Datum::string_to_time_t(pom->getDatumDorucenia()) == (Datum::string_to_time_t(Datum::getAktualnyDatum()) + 24 * 60 * 60)) {
			if (pom->getStav() == eStavObjednavky::PRIJATA) {
				pom->setStav(eStavObjednavky::PRIPRAVENA);
				if (pom->getTyp() == eTypProduktu::HRANOLCEKY) {
					pocetHranoliek += pom->getMnozstvo();
				}
				else {
					pocetLupienkov += pom->getMnozstvo();
				}
			}
		}
	}
	kontrolaZasobZajtra(pocetHranoliek, pocetLupienkov);

}

bool Firma::kontrolaZasobZajtra(int pocetHranoliek, int pocetLupienkov)
{

	structures::PriorityQueue<Objednavka *> * priorityCena = new structures::Heap<Objednavka*>();
	for (evidenciaObjednavky * var : *linkListObjednavok) {
		priorityCena->push(var->getObjednavka()->getCenaZaJednotku() * var->getObjednavka()->getMnozstvo(), var->getObjednavka());
	}

	Recept * recept = new Recept();
	float potrebujemOleja = recept->kolkoOlejaHranolky(pocetHranoliek) + recept->kolkoOlejaLup(pocetLupienkov);
	float potrebujemZemiakov = recept->kolkoZemHranolky(pocetHranoliek) + recept->kolkoZemLup(pocetLupienkov);
	float potrebujemOchu = recept->kolkoOchuLup(pocetLupienkov);

	while (skladOleja_ - potrebujemOleja < 0)
	{
		Objednavka * pom = priorityCena->peek();
		priorityCena->pop()->setStav(eStavObjednavky::ZRUSENA);
		if (pom->getTyp() == eTypProduktu::HRANOLCEKY) {
			potrebujemOleja = potrebujemOleja - recept->kolkoOlejaHranolky(pom->getMnozstvo());
			potrebujemZemiakov = potrebujemZemiakov - recept->kolkoZemHranolky(pom->getMnozstvo());
			pocetHranoliek -= pom->getMnozstvo();
		}
		else {
			potrebujemOleja = potrebujemOleja - recept->kolkoOlejaLup(pom->getMnozstvo());
			potrebujemZemiakov = potrebujemZemiakov - recept->kolkoZemLup(pom->getMnozstvo());
			potrebujemOchu = potrebujemOchu - recept->kolkoOchuLup(pom->getMnozstvo());
			pocetLupienkov -= pom->getMnozstvo();
		}
		
	}

	while (skladZemiakov_ - potrebujemZemiakov < 0)
	{
		Objednavka * pom = priorityCena->peek();
		priorityCena->pop()->setStav(eStavObjednavky::ZRUSENA);
		if (pom->getTyp() == eTypProduktu::HRANOLCEKY) {
			potrebujemOleja = potrebujemOleja - recept->kolkoOlejaHranolky(pom->getMnozstvo());
			potrebujemZemiakov = potrebujemZemiakov - recept->kolkoZemHranolky(pom->getMnozstvo());
			pocetHranoliek -= pom->getMnozstvo();
		}
		else {
			potrebujemOleja = potrebujemOleja - recept->kolkoOlejaLup(pom->getMnozstvo());
			potrebujemZemiakov = potrebujemZemiakov - recept->kolkoZemLup(pom->getMnozstvo());
			potrebujemOchu = potrebujemOchu - recept->kolkoOchuLup(pom->getMnozstvo());
			pocetLupienkov -= pom->getMnozstvo();
		}

	}

	while (skladOchu_ - potrebujemOchu < 0)
	{
		Objednavka * pom = priorityCena->peek();
		priorityCena->pop()->setStav(eStavObjednavky::ZRUSENA);
		if (pom->getTyp() == eTypProduktu::HRANOLCEKY) {
			potrebujemOleja = potrebujemOleja - recept->kolkoOlejaHranolky(pom->getMnozstvo());
			potrebujemZemiakov = potrebujemZemiakov - recept->kolkoZemHranolky(pom->getMnozstvo());
			pocetHranoliek -= pom->getMnozstvo();
		}
		else {
			potrebujemOleja = potrebujemOleja - recept->kolkoOlejaLup(pom->getMnozstvo());
			potrebujemZemiakov = potrebujemZemiakov - recept->kolkoZemLup(pom->getMnozstvo());
			potrebujemOchu = potrebujemOchu - recept->kolkoOchuLup(pom->getMnozstvo());
			pocetLupienkov -= pom->getMnozstvo();
		}

	}

	vyrobProdukty(pocetHranoliek, pocetLupienkov);
	delete recept;
	delete priorityCena;
	return true;

}

void Firma::kontrolaZasobNaSedem(int pocetHranoliek, int pocetLupienkov)
{
	Recept * recept = new Recept();
	float potrebujemOleja = recept->kolkoOlejaHranolky(pocetHranoliek) + recept->kolkoOlejaLup(pocetLupienkov);
	float potrebujemZemiakov = recept->kolkoZemHranolky(pocetHranoliek) + recept->kolkoZemLup(pocetLupienkov);
	float potrebujemOchu = recept->kolkoOchuLup(pocetLupienkov);
	if (skladOleja_ < potrebujemOleja) {
		dokupSuroviny(eTovary::OLEJ, potrebujemOleja - skladOleja_);
	}

	if (skladZemiakov_ < potrebujemZemiakov) {
		dokupSuroviny(eTovary::ZEMIAKY, potrebujemZemiakov - skladZemiakov_);
	}

	if (skladOchu_ < potrebujemOchu) {
		dokupSuroviny(eTovary::OCHUCOVALO, potrebujemOchu - skladOchu_);
	}

	delete recept;
}

void Firma::dokupSuroviny(eTovary druhTovaru, int mnozstvo)
{
	
	structures::PriorityQueue<Biofarmar *> * priorityCena = new structures::Heap<Biofarmar*>();
	for (Biofarmar * var : *arrayBiofarmarov) {
		if (var->coVyrabas(druhTovaru)) {
			priorityCena->push(var->prepocitajPriemer(druhTovaru), var);
		}
	}

	while (mnozstvo > 0 && !priorityCena->isEmpty()) {
		int j = priorityCena->size();
		Biofarmar *predajca = priorityCena->pop();
		int i = priorityCena->size();
		if (predajca->coVyrabas(druhTovaru)) {
			float skladBiofarmara = predajca->kolkoMasTovaru(druhTovaru);
			if (skladBiofarmara > mnozstvo) {
				nakupenyTovar->push(new Nakup(mnozstvo, druhTovaru));
				predajca->zaznamenajNakup(druhTovaru);
				predajca->setSklad(-mnozstvo, druhTovaru);
				mnozstvo = 0;
			}
			else {
				nakupenyTovar->push(new Nakup(skladBiofarmara, druhTovaru));
				predajca->zaznamenajNakup(druhTovaru);
				predajca->setSklad(-skladBiofarmara, druhTovaru);
				mnozstvo = mnozstvo - skladBiofarmara;
			}
			
		}

	}
	/*while (!priorityCena->isEmpty())
	{
		priorityCena->pop();
	}*/
	delete priorityCena;
}

void Firma::vypisanieObjednavokNasledujuciDen()
{

	for (evidenciaObjednavky* var : *linkListObjednavok) {
		time_t zajtrajsok = (Datum::string_to_time_t(Datum::getAktualnyDatum()) + 24 * 60 * 60);
		if (Datum::string_to_time_t(var->getObjednavka()->getDatumDorucenia()) == zajtrajsok ) {
			cout << "Zoznam objednavok na zajtra (" + Datum::time_t_to_string(zajtrajsok) + ") : \n" << "Evidovana - " << var->getDatum() + " \t\t" ;
			var->getObjednavka()->toString();
		}
	}
}

void Firma::vypisanieZakaznikov(int region, string datumOd, string datumDo)
{
	time_t odkedy = Datum::string_to_time_t(datumOd);
	time_t dokedy = Datum::string_to_time_t(datumDo);
	for (Zakaznik* var : *linkListZakaznikov) {
		if (var->getAdresa() == region) {
			var->vypis();
			cout << "Za obdobie " << datumOd << " - " << datumDo << endl;
			float done = 0;
			float denied = 0;
			int d = 0;
			int dWeight = 0;
			float canceled = 0;
			int c = 0;
			int cWeight = 0;

			for (evidenciaObjednavky* obj : *linkListObjednavok) {
				if (obj->getObjednavka()->getZakaznik() == var) {
					if (Datum::string_to_time_t(obj->getDatum()) >= odkedy && Datum::string_to_time_t(obj->getDatum()) <= dokedy) {
						if (obj->getObjednavka()->getStav() == eStavObjednavky::ZREALIZOVANA) {
							done += (obj->getObjednavka()->getMnozstvo() * obj->getObjednavka()->getCenaZaJednotku());
						}
						if (obj->getObjednavka()->getStav() == eStavObjednavky::ZAMIETNUTA) {
							denied += (obj->getObjednavka()->getMnozstvo() * obj->getObjednavka()->getCenaZaJednotku());
							dWeight += obj->getObjednavka()->getMnozstvo();
							d++;
						}
						if (obj->getObjednavka()->getStav() == eStavObjednavky::ZRUSENA) {
							canceled += (obj->getObjednavka()->getMnozstvo() * obj->getObjednavka()->getCenaZaJednotku());
							cWeight += obj->getObjednavka()->getMnozstvo();
							c++;
						}
					}
				}
				
			}
			cout << "Prijem spolocnosti: " << done << " Zamietnutych(prijem, pocet, hmotnost): " << denied << ", " << d << ", "
					<< dWeight << " Zrusenych(prijem, pocet, hmotnost): " << canceled << ", " << c << ", " << cWeight << endl;
		}
	}
}



void Firma::vypisanieZrealizovychObjednavok(string datumOd, string datumDo)
{
	//TODO 09: TREBA POTRIEDIT PODLA DATUMU DORUCENIA
	structures::Heap<evidenciaObjednavky*>* pom = new structures::Heap<evidenciaObjednavky*>();
	time_t odkedy = Datum::string_to_time_t(datumOd);
	time_t dokedy = Datum::string_to_time_t(datumDo);
	for (evidenciaObjednavky *var : *linkListObjednavok) {
		if (Datum::string_to_time_t(var->getObjednavka()->getDatumDorucenia()) >= odkedy && Datum::string_to_time_t(var->getObjednavka()->getDatumDorucenia()) <= dokedy) {
			if (var->getObjednavka()->getStav() == eStavObjednavky::ZREALIZOVANA) {
				pom->push(Datum::string_to_time_t(var->getObjednavka()->getDatumDorucenia()), var);
				
			}
		}
	}
	while (!pom->isEmpty()) {
		evidenciaObjednavky * var = pom->pop();
		cout << var->getObjednavka()->getDatumDorucenia();
		var->getObjednavka()->toString();
	}
	delete pom;
}

void Firma::vypisanieZrusenychObjednavok(string datumOd, string datumDo)
{
	//NETREBA TRIEDIT NIC
	time_t odkedy = Datum::string_to_time_t(datumOd);
	time_t dokedy = Datum::string_to_time_t(datumDo);
	for (evidenciaObjednavky *var : *linkListObjednavok) {
		if (Datum::string_to_time_t(var->getObjednavka()->getDatumDorucenia()) >= odkedy &&
			Datum::string_to_time_t(var->getObjednavka()->getDatumDorucenia()) <= dokedy) {
			if (var->getObjednavka()->getStav() == eStavObjednavky::ZAMIETNUTA || var->getObjednavka()->getStav() == eStavObjednavky::ZRUSENA) {
				cout << var->getDatum();
				var->getObjednavka()->toString();
			}
		}
	}
}

void Firma::najOblubenejsiFarmar(eTovary tovar)
{
	structures::PriorityQueue<Biofarmar*> * bio = new structures::Heap<Biofarmar*>();
	
	for (Biofarmar *var : *arrayBiofarmarov) {
		int priorita = 0;
		if (var->coVyrabas(tovar)) {
			
			for (size_t i = 0; i < 30; i++)
			{
				if (var->bolNakup(tovar, i)) {
					priorita++;
				}
			}
			bio->push(30 - priorita, var);
		}
	}
	cout << "Najviac nakupov od ";
	bio->pop()->toString();
	while (!bio->isEmpty())
	{
		bio->pop();
	}
	delete bio;
}

Zakaznik * Firma::najdiZakaznikaPodlaMena(string meno)
{
	for (Zakaznik* var : *linkListZakaznikov) {
		if (meno == var->getMeno()) {
			return var;
		}
	}
	cout << "Zakaznik s danym menom sa nenasiel!" << endl;
	return nullptr;
}

void Firma::vyrobProdukty(int pocetHranoliek, int pocetLupienkov)
{
	
	// Odcitanie surovin
	Recept * recept = new Recept();
	skladOleja_ -= (recept->kolkoOlejaLup(pocetLupienkov) + recept->kolkoOlejaHranolky(pocetHranoliek));
	skladZemiakov_ -= (recept->kolkoZemLup(pocetLupienkov) + recept->kolkoZemHranolky(pocetHranoliek));
	skladOchu_ -= recept->kolkoOchuLup(pocetLupienkov);

	// mozno cez priority queue naplnat no bude to naopak, staci odpocitat 8 od regionu
	// Objednavky na zajtra, zoradene podla regionu
	structures::PriorityQueue<Objednavka*> * objednavky = new structures::Heap<Objednavka*>();
	for (evidenciaObjednavky* obj : *linkListObjednavok) {
		if (Datum::string_to_time_t(obj->getObjednavka()->getDatumDorucenia()) == (Datum::string_to_time_t(Datum::getAktualnyDatum())) &&
			obj->getObjednavka()->getStav() == eStavObjednavky::PRIPRAVENA) {
			objednavky->push(8 - obj->getObjednavka()->getZakaznik()->getAdresa(), obj->getObjednavka());
		}
	}
	// Plnenie vozidiel,odovzdanie a vratenie sa aut
	// rozdelenie skupin aut
	structures::ArrayList<Vozidlo*> * hranolkoveAuta = new structures::ArrayList<Vozidlo*>();
	structures::ArrayList<Vozidlo*> * lupienkoveAuta = new structures::ArrayList<Vozidlo*>();
	if (objednavky->size() > 0) {
		
		for (evidenciaVozidla * voz : *arrayVozidiel) {
			if (voz->getVozidlo().getTyp() == eVozidla::MRAZENE) {
				hranolkoveAuta->add(voz->dajVozidlo());
			}
			else {
				lupienkoveAuta->add(voz->dajVozidlo());
			}
		}
	}

	int pocetObjednavok = objednavky->size();
	int kapacitaAutHran = kapacitaMraziarenskych_;
	int kapacitaAutLup = kapacitaLupienkovych_;
	int index = 0;
	int aktNostnostHran = eNosnostAuta::NOSTNOST_MRAZ;
	int aktNostnostLup = eNosnostAuta::NOSTNOST_LUP;
	int aktRegion = 0;
	for (size_t i = 0; i < pocetObjednavok; i++)
	{
		Objednavka * pom = objednavky->pop();
		
		if (pom->getTyp() == eTypProduktu::HRANOLCEKY) {
			if (pom->getMnozstvo() <= aktNostnostHran) {
				if (aktRegion != pom->getZakaznik()->getAdresa()) {
					(*hranolkoveAuta)[index]->pridajNaklady(eNosnostAuta::NAKLADY_MRAZ);
					aktRegion = pom->getZakaznik()->getAdresa();
				}
				aktNostnostHran -= pom->getMnozstvo();
			}
			else { // zmena auta lebo sa to nezmesti do jedneho
				if (aktRegion != pom->getZakaznik()->getAdresa()) {
					(*hranolkoveAuta)[index]->pridajNaklady(eNosnostAuta::NAKLADY_MRAZ);
				}
				index++;
				(*hranolkoveAuta)[index]->pridajNaklady(eNosnostAuta::NAKLADY_MRAZ);
				aktNostnostHran = eNosnostAuta::NOSTNOST_MRAZ - (pom->getMnozstvo() - aktNostnostHran);
			}
			kapacitaAutHran -= pom->getMnozstvo(); // len pre ukazku ako sa odcita s celkovej kapacity vsetkych aut
			
		}
		else {
			if (pom->getMnozstvo() <= aktNostnostLup) {
				if (aktRegion != pom->getZakaznik()->getAdresa()) {
					(*lupienkoveAuta)[index]->pridajNaklady(eNosnostAuta::NAKLADY_LUP);
					aktRegion = pom->getZakaznik()->getAdresa();
				}
				aktNostnostLup -= pom->getMnozstvo();
			}
			else { // zmena auta lebo sa to nezmesti do jedneho
				if (aktRegion != pom->getZakaznik()->getAdresa()) {
					(*lupienkoveAuta)[index]->pridajNaklady(eNosnostAuta::NAKLADY_LUP);
				}
				index++;
				(*lupienkoveAuta)[index]->pridajNaklady(eNosnostAuta::NAKLADY_LUP);
				aktNostnostLup = eNosnostAuta::NOSTNOST_LUP - (pom->getMnozstvo() - aktNostnostLup);
			}
			kapacitaAutLup -= pom->getMnozstvo();
		}

		pom->setStav(eStavObjednavky::ZREALIZOVANA);

	}
	delete hranolkoveAuta;
	delete lupienkoveAuta;
	delete objednavky;
	delete recept;
}

void Firma::vypisanieVsetkychObjednavok()
{
	for (evidenciaObjednavky * var : *linkListObjednavok) {
		var->getObjednavka()->toString();
	}
}

void Firma::dalsiDen()
{
	naskladniTovar();
	this->kontrolaNaSedemDni();
	this->kontrolaNaZajtra(); // tu sa aj vyraba
	for (Biofarmar* var : *arrayBiofarmarov) {
		var->dalsiDen();
	}
}

istream& operator>> (istream& is, Firma& firma) {
	int pocetFarm = 0;
	is >> pocetFarm;
	for (size_t i = 0; i < pocetFarm; i++)
	{
		Biofarmar * test = new Biofarmar("",true,true,true);
		is >> *test;
		firma.arrayBiofarmarov->add(test);
	}
	int pocetVoz = 0;
	is >> pocetVoz;
	for (size_t i = 0; i < pocetVoz; i++)
	{
		int pomocna = 0;
		string spz = "";
		string datumEv = "";
		is >> pomocna;
		evidenciaVozidla * pom = new evidenciaVozidla(new Vozidlo((eVozidla)pomocna),"");
		is >> pomocna;
		pom->getVozidlo().setPrevadzkoveNaklady(pomocna);
		is >> spz;
		pom->getVozidlo().setSPZ(spz);
		is >> pomocna;
		pom->getVozidlo().pridajNaklady(pomocna);
		is >> datumEv;
		pom->setDatum(datumEv);
		firma.arrayVozidiel->add(pom);
	}
	int pocetZak = 0;
	is >> pocetZak;
	for (size_t i = 0; i < pocetZak; i++)
	{
		int region = 0;
		string meno = "";
		is >> meno >> region;
		Zakaznik * pom = new Zakaznik(meno, region);
		firma.linkListZakaznikov->add(pom);
	}
	int pocetObj = 0;
	is >> pocetObj;
	for (size_t i = 0; i < pocetObj; i++)
	{
		string menoZakaznika = "";
		is >> menoZakaznika;
		evidenciaObjednavky * prazda = new evidenciaObjednavky(nullptr, "");
		is >> *prazda;
		prazda->getObjednavka()->setZakaznik(firma.najdiZakaznikaPodlaMena(menoZakaznika));
		firma.linkListObjednavok->add(prazda);
	}
	is >> firma.pocetMraziacichAut_ >> firma.pocetLupienkovychAut_ >> firma.kapacitaMraziarenskych_ >> firma.kapacitaLupienkovych_ >>
		firma.skladZemiakov_ >> firma.skladOleja_ >> firma.skladOchu_;

	cout << "Nacitanie prebehlo uspesne \n";
	return is;
}



ostream& operator<< (ostream& os, Firma& firma) {
	int pocetBio = firma.arrayBiofarmarov->size();
	os << pocetBio << " ";
	for (Biofarmar * var : *firma.arrayBiofarmarov) {
		os << *var;
	}
	int pocetVoz = firma.arrayVozidiel->size();
	os << pocetVoz << " ";
	for (evidenciaVozidla * voz : *firma.arrayVozidiel) {
		os << (int)voz->getVozidlo().getTyp() << " ";
		os << voz->getVozidlo().getPrevazdkoveNaklady() << " ";
		os << voz->getVozidlo().getSPZ() << " ";
		os << voz->getVozidlo().getNaklady() << " ";
		os << voz->getDatum() << "\n";
	}
	int pocetZak = firma.linkListZakaznikov->size();
	os << pocetZak << " ";
	for (Zakaznik * zak : *firma.linkListZakaznikov)
	{
		os << zak->getMeno() << " " << zak->getAdresa() << "\n";
	}
	int pocetObj = firma.linkListObjednavok->size();
	os << pocetObj << " ";
	for (evidenciaObjednavky * obj : *firma.linkListObjednavok) {
		os << *obj;
	}
	os << firma.pocetMraziacichAut_ << " " << firma.pocetLupienkovychAut_ << " " << firma.kapacitaMraziarenskych_ << " " <<
		firma.kapacitaLupienkovych_ << " " << firma.skladZemiakov_ << " " << firma.skladOleja_ << " " << firma.skladOchu_ << "\n";

	return os;
}

ostream & operator<<(ostream & os, evidenciaObjednavky & eObj)
{
	os << eObj.getObjednavka()->getZakaznik()->getMeno() << " ";
	os << (int)eObj.getObjednavka()->getTyp() << " " << eObj.getObjednavka()->getMnozstvo() << " " << eObj.getObjednavka()->getCenaZaJednotku() << " "
		<< eObj.getObjednavka()->getDatumDorucenia() << " " << (int)eObj.getObjednavka()->getStav() << " ";
	os << eObj.getDatum() << "\n";
	return os;
}

istream & operator>>(istream & is, evidenciaObjednavky & eObj)
{
	string datumDo = ""; // meno , datum dorucenia a datumEvidencie
	int typProd = 0; // adresa a typProduktu
	int mnozstvo = 0;
	int stav = 0;
	float cena = 0.00;
	is >> typProd >> mnozstvo >> cena >> datumDo >> stav;
	Objednavka * pom = new Objednavka(nullptr, (eTypProduktu)typProd, mnozstvo, datumDo);
	pom->setCenaZaJednotku(cena);
	pom->setStav((eStavObjednavky)stav);
	eObj.objednavka_ = pom;
	is >> datumDo;
	eObj.datumEvidencie_ = datumDo;
	return is;
}

bool Firma::skontrolujKapacituAuta(int mnozstvo, eTypProduktu typ,string datum)
{
	int pomocna = 0;
	if ((typ == eTypProduktu::HRANOLCEKY && pocetMraziacichAut_ == 0) || (typ == eTypProduktu::LUPIENKY && pocetLupienkovychAut_ == 0)) {
		return false;
	}

	for (evidenciaObjednavky* var : *linkListObjednavok) {
		if (Datum::string_to_time_t(var->getObjednavka()->getDatumDorucenia()) == Datum::string_to_time_t(datum)) {
			if (var->getObjednavka()->getTyp() == typ && var->getObjednavka()->getStav() != eStavObjednavky::ZAMIETNUTA ) {
				pomocna += var->getObjednavka()->getMnozstvo();
			}
		}
	}

	if (typ == eTypProduktu::HRANOLCEKY) {
		if (pomocna <= kapacitaMraziarenskych_) {
			return true;
		}
	}
	else {
		if (pomocna <= kapacitaLupienkovych_) {
			return true;
		}
	}
	cout << "Prekrocena kapacita aut objednavka zamietnuta" << endl;
	return false;
}

void Firma::naskladniTovar()
{
	while (!nakupenyTovar->isEmpty()) {
		Nakup * pom = nakupenyTovar->pop();
		if (pom->getTyp() == eTovary::ZEMIAKY) {
			skladZemiakov_ += pom->getMnozstvo();
		}
		if (pom->getTyp() == eTovary::OLEJ) {
			skladOleja_ += pom->getMnozstvo();
		}
		if (pom->getTyp() == eTovary::OCHUCOVALO) {
			skladOchu_ += pom->getMnozstvo();
		}
		delete pom;
	}
}


void Firma::vypisZoznamBiofarmarov(const eTovary tovar)
{
	for (Biofarmar* var : *arrayBiofarmarov) {
		if (var->coVyrabas(tovar)) {
			var->toString();
		}
	}

}

void Firma::vypisZoznamVozidiel()
{
	for (evidenciaVozidla* var : *arrayVozidiel) {
		var->getVozidlo().toString();
		cout << "\tDatum evidencie: " << var->getDatum() << endl;

	}

}




Firma::~Firma()
{
	// treba premazat vsetky objekty ktore som kam naplnil
	for (evidenciaVozidla *var : *arrayVozidiel) {
		delete var;
	}
	for (Zakaznik * zak : *linkListZakaznikov) {
		delete zak;
	}
	for (evidenciaObjednavky *var : *linkListObjednavok) {
		delete var;
	}
	delete arrayVozidiel;
	delete linkListObjednavok;
	
	delete linkListZakaznikov;
	for (Biofarmar *var : *arrayBiofarmarov) {
		delete var;
	}
	delete arrayBiofarmarov;

	while (!nakupenyTovar->isEmpty()) {
		Nakup * n = nakupenyTovar->pop();
		delete n;
	}
	delete nakupenyTovar;
}


// EVIDENCIE
//vozidla
evidenciaVozidla::evidenciaVozidla(Vozidlo * novyVoz, string datum) :
	voz_(novyVoz),datumEvidencie_(datum)
{
}

Vozidlo & evidenciaVozidla::getVozidlo()
{
	return *voz_;
}

Vozidlo * evidenciaVozidla::dajVozidlo()
{
	return voz_;
}



string evidenciaVozidla::getDatum()
{
	return datumEvidencie_;
}

evidenciaVozidla::~evidenciaVozidla()
{
	delete voz_;
}

//objednavky
evidenciaObjednavky::evidenciaObjednavky(Objednavka* novaObjednavka, string datum) :
	objednavka_(novaObjednavka),datumEvidencie_(datum)
{
}

Objednavka * evidenciaObjednavky::getObjednavka()
{
	return objednavka_;
}

string evidenciaObjednavky::getDatum()
{
	return datumEvidencie_;
}

evidenciaObjednavky::~evidenciaObjednavky()
{
	delete objednavka_;
}
// Obalovacia trieda ked nakupim tovar naskladnim ho az vecer cize z neho nemozem vyrabat este v ten den
Nakup::Nakup(float mnozstvo, eTovary typTovaru) :
	mnozstvo_(mnozstvo), typTovaru_(typTovaru)
{
}

Nakup::~Nakup()
{

}
