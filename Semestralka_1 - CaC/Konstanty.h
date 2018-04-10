#pragma once

enum eNosnostAuta {
	NAKLADY_MRAZ = 100,
	NAKLADY_LUP = 70,
	NOSTNOST_MRAZ = 5000,
	NOSTNOST_LUP = 2000
};

enum class eTypProduktu
{
	LUPIENKY,
	HRANOLCEKY
};

enum class eStavObjednavky
{
	PRIJATA = 1,
	ZAMIETNUTA = 2,
	ZRUSENA = 3,
	ZREALIZOVANA = 4,
	PRIPRAVENA = 5
};

enum class eVozidla
{
	MRAZENE,
	NEMRAZENE
};

enum class eTovary
{
	ZEMIAKY,
	OLEJ,
	OCHUCOVALO
};


class Recept
{
public:
	const float HRANOLKY_OLEJ = 0.2;
	const float HRANOLKY_ZEMIAKY = 1.5;
	const float LUPIENKY_OLEJ = 0.4;
	const float LUPIENKY_ZEMIAKY = 2;
	const float LUPIENKY_OCHU = 0.02;
	inline float kolkoOlejaHranolky(int mnozstvo) {
		return HRANOLKY_OLEJ * mnozstvo;
	}
	inline float kolkoZemHranolky(int mnozstvo) {
		return HRANOLKY_ZEMIAKY * mnozstvo;
	}
	inline float kolkoOlejaLup(int mnozstvo) {
		return LUPIENKY_OLEJ * mnozstvo;
	}
	inline float kolkoZemLup(int mnozstvo) {
		return LUPIENKY_ZEMIAKY * mnozstvo;
	}
	inline float kolkoOchuLup(int mnozstvo) {
		return LUPIENKY_OCHU * mnozstvo;
	}
};