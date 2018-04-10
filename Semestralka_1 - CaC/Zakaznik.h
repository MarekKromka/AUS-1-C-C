#pragma once
#include <iostream>
#include <string>

#include "Datum.h"

class Objednavka;

using namespace std;
class Zakaznik
{
public:
	Zakaznik(string nazov, int adresa);
	~Zakaznik();
	string getMeno() {
		return nazov_;
	}
	int getAdresa() {
		return adresa_;
	}
	void vypis();

private:
	string nazov_;
	int adresa_;

};

