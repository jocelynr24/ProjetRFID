/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier AccesTimer1.cpp
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 08/06/2016

*/

#include "AccesTimer1.h"

void InitTimer1(void){
	TCCR1A = B00000000;
	TCCR1B = B00000100;
	TCCR1C = B00000000;
}


void Seconde(void) {
	TCNT1 = INIT_TCNT1;
	do{
	} while ((TIFR1 & MASQUE_TOV1) != MASQUE_TOV1);
	TIFR1 = TIFR1 | B00000001;
}
