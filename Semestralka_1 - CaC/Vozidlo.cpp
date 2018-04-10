#include "Vozidlo.h"



Vozidlo::Vozidlo(const eVozidla typVozidla) 
{
	if (typVozidla == eVozidla::MRAZENE) {
		nosnost_ = (int)NOSTNOST_MRAZ;
		prevadzkoveNaklady_ = NAKLADY_MRAZ;
		typ_ = typVozidla;
	}
	else if (typVozidla == eVozidla::NEMRAZENE) {
		nosnost_ = (int)NOSTNOST_LUP;
		prevadzkoveNaklady_ = NAKLADY_LUP;
		typ_ = typVozidla;
	}
	setSPZ();
}

int Vozidlo::unikatnostSPZ_ = 000;

Vozidlo::~Vozidlo()
{
}

void Vozidlo::toString()
{
	std::cout << "SPZ - " << SPZ_ <<"\t TYP : " << ((typ_ == eVozidla::MRAZENE) ? "na rozvoz mrazenych hranolcekov" : "na rozvoz zemiakovych lupienkov") 
		<< "\t NAKLADY: " << naklady_ << " " ;
}

void Vozidlo::setSPZ()
{
	int pom = ++unikatnostSPZ_;
	std::string formated = std::string(3 - std::to_string(pom).length(), '0') + std::to_string(pom);
	SPZ_ += formated + "JJ";
}
