#include "Biofarmar.h"
#include <iostream>



Biofarmar::Biofarmar(string meno, bool zemiaky, bool olej, bool ochucovadlo)
	: meno_(meno), zemiaky_(zemiaky), olej_(olej), ochucovadlo_(ochucovadlo)
{

	if (olej_) {
		cenaO = 1 + ((float)rand() / ((float)RAND_MAX / (4 - 1)));
		skladOleja_ = (1000 - 0) * ((((float)rand()) / (float)RAND_MAX)) + 0;
		//cout << skladOleja_ << "Sklad oleja " << endl;
		nakupOlj = new structures::LinkedList<float>();
		for (int i = 0; i < 30; i++)
		{
			nakupOlj->add(0);
		}
	}
	if (zemiaky_) {
		
		cenaZ = 1 + ((float)rand() / ((float)RAND_MAX / (3 - 1)));
		skladZemiakov_ = (5000 - 0) * ((((float)rand()) / (float)RAND_MAX)) + 0;
		//cout << skladZemiakov_ << "Sklad zem " << endl;
		nakupZem = new structures::LinkedList<float>();
		for (int i = 0; i < 30; i++)
		{
			nakupZem->add(0);
		}
	}
	if (ochucovadlo_) {
		cenaOch = 1 + ((float)rand() / ((float)RAND_MAX / (2 - 1)));
		skladOchu_ = (50 - 0) * ((((float)rand()) / (float)RAND_MAX)) + 0;
		//cout << skladOchu_ << "Sklad ochu " << endl;
		nakupOchu = new structures::LinkedList<float>();
		for (int i = 0; i < 30; i++)
		{
			nakupOchu->add(0);
		}
	}
	
}

Biofarmar::~Biofarmar()
{
	if (!nakupOchu->isEmpty()) {
		delete nakupOchu;
	}
	if (!nakupZem->isEmpty()) {
		delete nakupZem;
	}
	if (!nakupOlj->isEmpty()) {
		delete nakupOlj;
	}
	
	
}

void Biofarmar::toString()
{
	cout << "Biofarmar: " << meno_ << " Dodava: " << setprecision(2) << fixed << (zemiaky_ ? "ZEMIAKY za Pcenu: " + to_string(prepocitajPriemer(eTovary::ZEMIAKY)) : "")
		<< (olej_ ? " OLEJ za Pcenu: " + to_string(prepocitajPriemer(eTovary::OLEJ)) : "") 
		<< (ochucovadlo_ ? " OCHUCOVADLA za Pcenu:" + to_string(prepocitajPriemer(eTovary::OCHUCOVALO)) : "") 
		<< endl;
}

bool Biofarmar::coVyrabas(const eTovary tovar)
{
	switch (tovar)
	{
	case eTovary::ZEMIAKY:
		if (zemiaky_)
		return true;
		break;
	case eTovary::OLEJ:
		if (olej_)
		return true;
		break;
	case eTovary::OCHUCOVALO:
		if (ochucovadlo_)
		return true;
		break;
	}
	return false;
}

string Biofarmar::getMeno()
{
	return meno_;
}

float Biofarmar::kolkoMasTovaru(eTovary tovar)
{
	switch (tovar)
	{
	case eTovary::ZEMIAKY:
		return skladZemiakov_;
		break;
	case eTovary::OLEJ:
		return skladOleja_;
		break;
	case eTovary::OCHUCOVALO:
		return skladOchu_;
		break;
	}
	return 0;
}

void Biofarmar::setSklad(float mnozstvo, eTovary tovar)
{
	switch (tovar) {
	case eTovary::ZEMIAKY:
		skladZemiakov_ += mnozstvo;
		break;
	case eTovary::OLEJ:
		skladOleja_ += mnozstvo;
		break;
	case eTovary::OCHUCOVALO:
		skladOchu_ += mnozstvo;
		break;
	}
}


float Biofarmar::prepocitajPriemer(eTovary tovar)
{
	structures::LinkedList<float>* temp;
	switch (tovar)
	{
	case eTovary::ZEMIAKY:
		temp = this->nakupZem;
		break;
	case eTovary::OLEJ:
		temp = this->nakupOlj;
		break;
	case eTovary::OCHUCOVALO:
		temp = this->nakupOchu;
		break;
	default:
		break;
	}

	float priemer = 0;
	int index = 0;
	for (float var : *temp) {
		if (var != 0.00) {
			priemer += var;
			index++;
		}
	}
	if (index != 0) {
		priemer /= index;
	}
	return priemer;
}

void Biofarmar::zaznamenajNakup(eTovary tovar)
{
	structures::LinkedList<float>* temp;
	float tempCena = 0;
	switch (tovar)
	{
	case eTovary::ZEMIAKY:
		temp = this->nakupZem;
		tempCena = cenaZ;
		dnesnyNakupZ = true;
		break;
	case eTovary::OLEJ:
		temp = this->nakupOlj;
		tempCena = cenaO;
		dnesnyNakupO = true;
		break;
	case eTovary::OCHUCOVALO:
		temp = this->nakupOchu;
		tempCena = cenaOch;
		dnesnyNakupOchu = true;
		break;
	default:
		break;
	}
	temp->removeAt(0);
	temp->add(tempCena);
	
}

void Biofarmar::dalsiDen()
{
	if (zemiaky_) {
		if (!dnesnyNakupZ) {
			nakupZem->removeAt(0);
			nakupZem->add(0);
		}
	}

	if (olej_) {
		if (!dnesnyNakupO) {
			nakupOlj->removeAt(0);
			nakupOlj->add(0);
		}
	}

	if (ochucovadlo_) {
		if (!dnesnyNakupOchu) {
			nakupOchu->removeAt(0);
			nakupOchu->add(0);
		}
	}
	dnesnyNakupO = false;
	dnesnyNakupZ = false;
	dnesnyNakupOchu = false;
}

bool Biofarmar::bolNakup(eTovary tovar, int index)
{
	switch (tovar)
	{
	case eTovary::ZEMIAKY:
		if ((*nakupZem)[index] != 0) {
			return true;
		}
		break;
	case eTovary::OLEJ:
		if ((*nakupOlj)[index] != 0) {
			return true;
		}
		break;
	case eTovary::OCHUCOVALO:
		if ((*nakupOchu)[index] != 0) {
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void Biofarmar::setCena(float cena,const eTovary tovar)
{

	switch (tovar) {
	case eTovary::ZEMIAKY:
		cenaZ = cenaZ + cena;
		break;
	case eTovary::OLEJ:
		cenaO = cenaO + cena;
		break;
	case eTovary::OCHUCOVALO:
		cenaOch = cenaOch + cena;
		break;
	}

	

}

istream& operator>> (istream& is, Biofarmar& bio) {
	
	is >> bio.meno_ >> bio.cenaZ >> bio.cenaO >> bio.cenaOch >> bio.zemiaky_ >> bio.olej_ >> bio.ochucovadlo_;
	is >> bio.skladZemiakov_ >> bio.skladOleja_ >> bio.skladOchu_;
	if (bio.zemiaky_) {
		for (size_t i = 0; i < 30; i++)
		{
			float var = 0;
			is >> var;
			bio.nakupZem->removeAt(0);
			bio.nakupZem->add(var);
		}
	}
	if (bio.olej_) {
		for (size_t i = 0; i < 30; i++)
		{
			float var = 0;
			is >> var;
			bio.nakupOlj->removeAt(0);
			bio.nakupOlj->add(var);
		}
	}
	if (bio.ochucovadlo_) {
		for (size_t i = 0; i < 30; i++)
		{
			float var = 0;
			is >> var;
			bio.nakupOchu->removeAt(0);
			bio.nakupOchu->add(var);
		}
	}

	return is;
}

ostream& operator<< (ostream& os, Biofarmar& bio) {
	os << bio.meno_ << " " << bio.cenaZ << " " << bio.cenaO << " " << bio.cenaOch << "\n" << bio.zemiaky_ << " " << bio.olej_ << " " << bio.ochucovadlo_ << "\n";
	os << bio.skladZemiakov_ << " " << bio.skladOleja_ << " " << bio.skladOchu_ << "\n";
	if (bio.zemiaky_) {
		for (float var : *bio.nakupZem)
		{
			os << var;
			os << " ";
		}
	}
	if (bio.olej_) {
		for (float var : *bio.nakupOlj)
		{
			os << var;
			os << " ";
		}
	}
	if (bio.ochucovadlo_) {
		for (float var : *bio.nakupOchu)
		{
			os << var;
			os << " ";
		}
	}
	os << "\n";
	return os;
}
