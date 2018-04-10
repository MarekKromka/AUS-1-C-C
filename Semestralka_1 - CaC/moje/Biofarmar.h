#pragma once
#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <String>

#include "..\structures\list\array_list.h"
#include "..\structures\list\linked_list.h"
#include "..\Konstanty.h"

using namespace std;


class Biofarmar
{
public:
	Biofarmar(string meno, bool zemiaky, bool olej, bool ochucovadlo);
	~Biofarmar();
	void toString();
	bool coVyrabas(const eTovary tovar);
	string getMeno();
	
	float kolkoMasTovaru(eTovary tovar);
	void setSklad(float mnozstvo, eTovary tovar);

	float prepocitajPriemer(eTovary tovar);
	void zaznamenajNakup(eTovary tovar);
	void dalsiDen();
	bool bolNakup(eTovary tovar,int index);

	friend ostream& operator<< (ostream& os, Biofarmar& bio);
	friend istream& operator>> (istream& is, Biofarmar& bio);
private:
	string meno_;
	bool zemiaky_;
	bool olej_;
	bool ochucovadlo_;

	//kvoli priemeru uklada 30 dni
	structures::LinkedList<float>* nakupZem;
	bool dnesnyNakupZ = false;
	structures::LinkedList<float>* nakupOlj;
	bool dnesnyNakupO = false;
	structures::LinkedList<float>* nakupOchu;
	bool dnesnyNakupOchu = false;

	float cenaZ = 0;
	float cenaO = 0;
	float cenaOch = 0;
	float skladZemiakov_ = 0;
	float skladOleja_ = 0;
	float skladOchu_ = 0;
	void setCena(float cena, const eTovary tovar);
};

