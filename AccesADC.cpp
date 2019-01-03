/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier AccesADC.cpp
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 08/06/2016

*/

#include "AccesADC.h"

void InitCan(void){
	ADCSRA = B10000100;
	ADMUX = B01000011;
}

int16_t ConvAn(void){
	uint8_t ui8ResCanH  = 0;
	uint8_t ui8ResCanL  = 0;
	int16_t i16RetResultat;
	ADCSRA = ADCSRA | MASQUE_ADSC; 
	do{}while((ADCSRA & MASQUE_ADSC) == MASQUE_ADSC);
	ui8ResCanL   = ADCL; 
	ui8ResCanH   = ADCH;
	i16RetResultat  = ((int16_t)ui8ResCanH<<8) + (int16_t)ui8ResCanL ; 
	return i16RetResultat;
}
