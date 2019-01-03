/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier Fonctions.cpp
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 09/06/2016

*/

// On inclut les fichiers essentiels au projet
#include "Fonctions.h"
#include "AccesPort.h"

// On inclut les variables provenant du .ino
extern int iBcl;
extern unsigned int iTabMemoire[NB_CARTES][11];
extern unsigned long lTemps5sValid[NB_CARTES];

// Fonction qui met la sortie à un NL1 quand une carte est valide
void CarteValide(void){
	PORTB = (PORTB | B00100000);
	delay(3000);
	PORTB = (PORTB &~ B00100000);
}

// Fonction qui vide la mémoire tampon (buffer) de la carte Arduino
void SerialClear(void){
	while (Serial.available() > 0) {
		Serial.read();
	}
}

// Fonction qui permet de tester BP1
bool BP1(void){
	bool bAppui = false;
	PORTC = PORTC | B00100000;
	if ((PINC & B00100000) == 0){
		delay(2);
		PORTC = PORTC | B00100000;
		if ((PINC & B00100000) == 0){
			bAppui = true;
		}
	}
	return bAppui;
}

// Fonction qui permet de tester BP2
bool BP2(void){
	bool bAppui = false;
	PORTC = PORTC | B00000001;
	if ((PINC & B00000001) == 0){
		delay(2);
		PORTC = PORTC | B00000001;
		if ((PINC & B00000001) == 0){
			bAppui = true;
		}
	}
	return bAppui;
}

// Fonction qui permet d'initialiser les cartes Arduino
void InitCartes(void) {
	for (iBcl = 0; iBcl < NB_CARTES; iBcl++) {
		iTabMemoire[iBcl][11] = 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0;
		lTemps5sValid[iBcl] = 0;
	}
}