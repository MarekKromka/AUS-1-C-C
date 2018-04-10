#pragma once
#include <string>
#include <iostream>
#include <stdio.h>

#include "Konstanty.h"


class Vozidlo
{
public:
	Vozidlo(const eVozidla typVozidla);
	~Vozidlo();
	void toString();
	inline eVozidla getTyp() {
		return typ_;
	}
	inline int getNosnost() {
		return nosnost_;
	}
	inline int getPrevazdkoveNaklady() {
		return prevadzkoveNaklady_;
	}
	inline std::string getSPZ() {
		return SPZ_;
	}
	inline int getNaklady() {
		return naklady_;
	}
	void setPrevadzkoveNaklady(int prevNaklady) {
		prevadzkoveNaklady_ = prevNaklady;
	}
	void setSPZ(std::string spz) {
		SPZ_ = "";
		SPZ_ = spz;
	}
	void pridajNaklady(int naklady) {
		naklady_ = naklady_ + naklady;
	}
private:
	void setSPZ();
	eVozidla typ_;
	int prevadzkoveNaklady_;
	int nosnost_;  //pis to v kilach
	std::string SPZ_ = "ZA_";
	int naklady_ = 0;
protected:
	static int unikatnostSPZ_;
};

