/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier Fonctions.h
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 09/06/2016

*/

#include <Arduino.h>

// Nombre de cartes maximum
#define NB_CARTES 20

// Prototype des fonctions
void CarteValide(void);
void SerialClear(void);
bool BP1(void);
bool BP2(void);
void InitCartes(void);