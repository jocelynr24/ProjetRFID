/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier AccesPort.cpp
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 08/06/2016

*/

#include "AccesPort.h"

void GereLed1(bool bMode){
	if(bMode == 0){
		PORTD = (PORTD & ~ MASQUE_LED1);
	}
	else if(bMode == 1){
		PORTD = (PORTD | MASQUE_LED1);
	}
}

void GereLed2(bool bMode){
	if(bMode == 0){
		PORTC = (PORTC & ~ MASQUE_LED2);
	}
	else if(bMode == 1){
		PORTC = (PORTC | MASQUE_LED2);
	}
}

void GereLed3(bool bMode){
	if(bMode == 0){
		PORTC = (PORTC & ~ MASQUE_LED3);
	}
	else if(bMode == 1){
		PORTC = (PORTC | MASQUE_LED3);
	}
}

void GereLed4(bool bMode){
	if(bMode == 0){
		PORTD = (PORTD & ~ MASQUE_LED4);
	}
	else if(bMode == 1){
		PORTD = (PORTD | MASQUE_LED4);
	}
}

bool GereBpK1(void){
	return ((PINC & B00100000) == B00100000);
}

bool GereBpK2(void){
	return ((PIND & MASQUE_K2) == MASQUE_K2);
}

void InitPortCarteFille(void){
	DDRD = B01010010;
	DDRC = B00000000;
	DDRB = B00100000;
}

void GereLed1t(int8_t i8Mode){
	if (i8Mode == 0){
		PORTD = (PORTD & ~MASQUE_LED1);
	}
	else if (i8Mode == 1){
		PORTD = (PORTD | MASQUE_LED1);
	}
	else if (i8Mode == 2){
		PORTD = (PORTD ^ MASQUE_LED1);
	}
}

void GereLed4t(int8_t i8Mode){
	if (i8Mode == 0){
		PORTD = (PORTD & ~MASQUE_LED4);
	}
	else if (i8Mode == 1){
		PORTD = (PORTD | MASQUE_LED4);
	}
	else if (i8Mode == 2){
		PORTD = (PORTD ^ MASQUE_LED4);
	}
}

void GereLed2t(int8_t i8Mode){
	if (i8Mode == 0){
		PORTD = (PORTD & ~MASQUE_LED2);
	}
	else if (i8Mode == 1){
		PORTD = (PORTD | MASQUE_LED2);
	}
	else if (i8Mode == 2){
		PORTD = (PORTD ^ MASQUE_LED2);
	}
}

void GereLed3t(int8_t i8Mode){
	if (i8Mode == 0){
		PORTD = (PORTD & ~MASQUE_LED3);
	}
	else if (i8Mode == 1){
		PORTD = (PORTD | MASQUE_LED3);
	}
	else if (i8Mode == 2){
		PORTD = (PORTD ^ MASQUE_LED3);
	}
}