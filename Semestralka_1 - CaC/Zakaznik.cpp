#include "Zakaznik.h"
#include "Objednavka.h"


Zakaznik::Zakaznik(std::string nazov, int adresa)
	: nazov_(nazov), adresa_(adresa)
{
}


Zakaznik::~Zakaznik()
{

}

void Zakaznik::vypis()
{
	cout << nazov_ << " R" << adresa_ << " ";
}


