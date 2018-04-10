#include "Biofarmar.h"
#include "..\Firma.h"
#include "..\Vozidlo.h"
#include "..\Datum.h"

#include "..\Uicko.h"

int main () {
	srand(time(NULL));
	initHeapMonitor();

	Uicko *ui = new Uicko();
	ui->hlavneMenu();

	
	delete ui;
	system("pause");
	return 0;
}