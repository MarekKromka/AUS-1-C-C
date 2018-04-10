#include "Uicko.h"

#include <fstream>

Uicko::Uicko()
{
	d = Datum::getInstance();
	firma = new Firma();
}


Uicko::~Uicko()
{
	delete firma;
	delete d;
}

void Uicko::hlavneMenu()
{
	Zakaznik *pom;
	ofstream out;
	ifstream in;
	char moznost = 0;
	cout << "++++++++++ Vytajte v Informacnom systeme firmy C&C ++++++++++" << endl;
	cout << "Date: " << Datum::getAktualnyDatum() << endl;
	cout << "Zvolte si moznost :\n\n" << "1. Sprava Biofarmarov\n" << "2. Sprava Vozidiel\n" << "3. Zakaznici\n" << "4. Objednavky\n\n"
		<< "6. Posun den\n" << "7. Koniec\n"<< "8. Test\n\n" << "9. Uloz\n" << "0. Nacitaj\n" << endl;
	cin >> moznost;
	switch (moznost)
	{
	case '1':
		system("cls");
		menuBiofarmar();
		break;
	case '2':
		system("cls");
		menuVozidla();
		break;
	case '3':
		system("cls");
		menuZakaznici();
		break;
	case '4':
		system("cls");
		menuObjednavky();
		break;
	case '6':
		system("cls");
		d->posunDen();
		firma->dalsiDen();
		hlavneMenu();
		break;
	case '7':
		system("cls");
		break;
	case '8':
		system("cls");
		firma->pridajBiofarmara(vytvorBiofarmara("Marek", true, false, true));
		firma->pridajBiofarmara(vytvorBiofarmara("Michal", false, false, true));
		firma->pridajBiofarmara(vytvorBiofarmara("Andrej", true, true, true));
		firma->pridajBiofarmara(vytvorBiofarmara("Peter", true, false, false));
		firma->pridajBiofarmara(vytvorBiofarmara("Igor", false, true, false));
		firma->pridajBiofarmara(vytvorBiofarmara("Martin", false, true, true));

		firma->pridajZakaznika(vytvorZakaznika("Zakaznik1", 8));
		firma->pridajZakaznika(vytvorZakaznika("Zakaznik2", 8));
		firma->pridajZakaznika(vytvorZakaznika("Zakaznik3", 5));
		firma->pridajZakaznika(vytvorZakaznika("Zakaznik4", 2));

		firma->pridajVozidlo(vytvorVozidlo(eVozidla::NEMRAZENE));
		firma->pridajVozidlo(vytvorVozidlo(eVozidla::MRAZENE));

		pom = firma->najdiZakaznikaPodlaMena("Zakaznik1");
		firma->pridajObjednavku(firma->vytvorObjednavku(pom, eTypProduktu::LUPIENKY, 200, "10/01/2000"));
		pom = firma->najdiZakaznikaPodlaMena("Zakaznik2");
		firma->pridajObjednavku(firma->vytvorObjednavku(pom, eTypProduktu::HRANOLCEKY, 6500, "09/01/2000"));
		pom = firma->najdiZakaznikaPodlaMena("Zakaznik3");
		firma->pridajObjednavku(firma->vytvorObjednavku(pom, eTypProduktu::HRANOLCEKY, 3000, "09/01/2000"));
		pom = firma->najdiZakaznikaPodlaMena("Zakaznik4");
		firma->pridajObjednavku(firma->vytvorObjednavku(pom, eTypProduktu::LUPIENKY, 50, "10/01/2000"));

		hlavneMenu();
		break;
	case '9':
		system("cls");
		out.open("ulozenie");
		out << *firma;
		out.close();
		hlavneMenu();
		break;
	case '0':
		system("cls");
		in.open("ulozenie");
		in >> *firma;
		in.close();
		hlavneMenu();
		break;
	default:
		system("cls");
		hlavneMenu();
		break;
	}
}




Biofarmar * Uicko::vytvorBiofarmara(string meno, bool z,bool o,bool och)
{
	return new Biofarmar(meno, z, o, och);
}

Vozidlo * Uicko::vytvorVozidlo(eVozidla typ)
{
	return new Vozidlo(typ);
}

Zakaznik * Uicko::vytvorZakaznika(string meno, int region)
{
	return new Zakaznik(meno,region);
}


void Uicko::menuBiofarmar()
{
	char moznost = 0;
	int tovar = 0;
	bool z = 0;
	bool o = 0;
	bool och = 0;
	string meno = "";
	cout << "++++++++++ Sprava Biofarmarov ++++++++++" << endl;
	cout << "1. Pridaj Biofarmara\n" << "2. Vypis zoznam Biofarmarov\n" << "3. Najoblubenejsi Biofarmar\n"<< "4. Hlavne Menu" << endl;
	cin >> moznost;
	switch (moznost)
	{
	case '1':
		
		cout << "\nZadaj meno: " << endl;
		cin >> meno;
		cout << "Vyraba zemiaky? (1/0)" << endl;
		cin >> z;
		cout << "Vyraba olej? (1/0)" << endl;
		cin >> o;
		cout << "Vyraba ochucovadlo? (1/0)" << endl;
		cin >> och;
		firma->pridajBiofarmara(vytvorBiofarmara(meno, z, o, och));
		system("cls");
		menuBiofarmar();
		break;
	case '2':
		cout << "\nAky tovar hladas? (0 - zemiaky,1 - olej, 2 - ochucovadlo) " << endl;
		cin >> tovar;
		system("cls");
		if (tovar == 0) firma->vypisZoznamBiofarmarov(eTovary::ZEMIAKY);
		if (tovar == 1) firma->vypisZoznamBiofarmarov(eTovary::OLEJ);
		if (tovar == 2) firma->vypisZoznamBiofarmarov(eTovary::OCHUCOVALO);
		cout << "\n\n";
		menuBiofarmar();
		break;
	case '3':
		system("cls");
		cout << "\nAky tovar hladas? (0 - zemiaky,1 - olej, 2 - ochucovadlo) " << endl;
		cin >> tovar;
		if (tovar == 0) firma->najOblubenejsiFarmar(eTovary::ZEMIAKY);
		if (tovar == 1) firma->najOblubenejsiFarmar(eTovary::OLEJ);
		if (tovar == 2) firma->najOblubenejsiFarmar(eTovary::OCHUCOVALO);

		menuBiofarmar();
		break;
	case '4':
		system("cls");
		hlavneMenu();
		break;
	default:
		system("cls");
		menuBiofarmar();
		break;
	}
}

void Uicko::menuVozidla()
{
	char moznost = 0;
	int typVozu = 0;
	cout << "++++++++++ Sprava Vozidiel ++++++++++" << endl;
	cout << "1. Pridaj Vozidlo\n" << "2. Vypis zoznam Vozidiel\n" << "3. Hlavne Menu" << endl;
	cin >> moznost;
	switch (moznost)
	{
	case '1':
		system("cls");
		cout << "Zadaj typ vozidla: (0 - MRAZENE, 1 - NEMRAZENE)" << endl;
		cin >> typVozu;
		if (typVozu == 0) firma->pridajVozidlo(vytvorVozidlo(eVozidla::MRAZENE));
		if (typVozu == 1) firma->pridajVozidlo(vytvorVozidlo(eVozidla::NEMRAZENE));
		menuVozidla();
		break;
	case '2':
		system("cls");
		firma->vypisZoznamVozidiel();
		cout << "\n\n";
		menuVozidla();
		break;
	case '3':
		system("cls");
		hlavneMenu();
		break;
	default:
		system("cls");
		menuVozidla();
		break;
	}
}

void Uicko::menuZakaznici()
{
	char moznost = 0;
	string meno = "";
	int region = 0;
	string odkedy = "";
	string dokedy = "";
	cout << "++++++++++ Sprava Zakaznikov ++++++++++" << endl;
	cout << "1. Pridaj Zakaznika\n" << "2. Vypisanie Zakaznikov z daneho regionu\n" <<"3. Hlavne Menu" << endl;
	cin >> moznost;
	switch (moznost)
	{
	case '1':
		system("cls");
		cout << "Zadaj meno zakaznika: " << endl;
		cin >> meno;
		cout << "Zadaj adresu/region: (1-8)" << endl;
		cin >> region;
		firma->pridajZakaznika(vytvorZakaznika(meno,region));
		menuZakaznici();
		break;
	case '2':
		system("cls");
		cout << "Z akeho regionu: " << endl;
		cin >> region;
		cout << "Odkedy :(vo formate DD/MM/RRRR)" << endl;
		cin >> odkedy;
		cout << "Dokedy :(vo formate DD/MM/RRRR)" << endl;
		cin >> dokedy;

		firma->vypisanieZakaznikov(region,odkedy,dokedy);
		menuZakaznici();
		break;
	case '3':
		system("cls");
		hlavneMenu();
		break;
	default:
		system("cls");
		menuZakaznici();
		break;
	}
}

void Uicko::menuObjednavky()
{
	char moznost = 0;
	Zakaznik* pom;
	string meno = "";
	string datumDo = "";
	string datumOd = "";
	int typProduktu = 0;
	int mnozstvo = 0;
	cout << "++++++++++ Sprava Objednavok ++++++++++" << endl;
	cout << "1. Pridaj Objednavku\n" << "2. Vypisanie Objednavok na zajtra\n" << "3. Vypisanie Zrealizovanych objednavok\n" <<
		"4. Vypisanie Zrusenych/Zamietnutych objednavok\n" << "5. Hlavne Menu" << endl;
	cin >> moznost;
	switch (moznost)
	{
	case '1':
		system("cls");
		cout << "Zadaj meno zakaznika: " << endl;
		cin >> meno;
		pom = firma->najdiZakaznikaPodlaMena(meno);
		if (pom == nullptr) menuObjednavky();
		cout << "Zadaj typ produktu: (0 - LUPIENKY, 1 - HRANOLCEKY)" << endl;
		cin >> typProduktu;
		cout << "Zadaj mnozstvo: (predavame na cele kila)" << endl;
		cin >> mnozstvo;
		cout << "Zadaj datum dorucenia: (vo formate DD/MM/RRRR)" << endl;
		cin >> datumDo;
		if(typProduktu == 0) firma->pridajObjednavku(firma->vytvorObjednavku(pom,eTypProduktu::LUPIENKY,mnozstvo,datumDo));
		if(typProduktu == 1) firma->pridajObjednavku(firma->vytvorObjednavku(pom,eTypProduktu::HRANOLCEKY, mnozstvo, datumDo));
		//firma->vypisanieVsetkychObjednavok();
		menuObjednavky();
		break;
	case '2':
		system("cls");
		firma->vypisanieObjednavokNasledujuciDen();
		cout << "\n\n";
		menuObjednavky();
		break;
	case '3':
		system("cls");
		cout << "Odkedy: (vo formate DD/MM/RRRR)" << endl;
		cin >> datumOd;
		cout << "Dokedy: (vo formate DD/MM/RRRR)" << endl;
		cin >> datumDo;
		firma->vypisanieZrealizovychObjednavok(datumOd,datumDo);
		cout << "\n\n";
		menuObjednavky();
		break;
	case '4':
		system("cls");
		cout << "Odkedy: (vo formate DD/MM/RRRR)" << endl;
		cin >> datumOd;
		cout << "Dokedy: (vo formate DD/MM/RRRR)" << endl;
		cin >> datumDo;
		firma->vypisanieZrusenychObjednavok(datumOd, datumDo);
		cout << "\n\n";
		menuObjednavky();
		break;
		break;
	case '5':
		system("cls");

		hlavneMenu();
		break;
	default:
		system("cls");
		menuObjednavky();
		break;
	}
}


