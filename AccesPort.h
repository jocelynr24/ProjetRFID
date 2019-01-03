/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier AccesPort.h
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 08/06/2016

*/

#include <Arduino.h>

#define MASQUE_LED1 B01000000
#define MASQUE_LED2 B00100000
#define MASQUE_LED3 B00010000
#define MASQUE_LED4 B00010000
#define MASQUE_K1 B00001000
#define MASQUE_K2 B00000100

void InitPortCarteFille(void);
void GereLed1(bool bMode);
void GereLed2(bool bMode);
void GereLed3(bool bMode);
void GereLed4(bool bMode);
bool GereBpK1(void);
bool GereBpK2(void);
void GereLed1t(int8_t i8Mode);
void GereLed2t(int8_t i8Mode);
void GereLed3t(int8_t i8Mode);
void GereLed4t(int8_t i8Mode);

