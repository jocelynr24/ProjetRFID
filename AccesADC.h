/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier AccesADC.h
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 08/06/2016

*/

#include <Arduino.h>

#define MASQUE_ADSC B01000000

void InitCan(void);
int16_t ConvAn(void);
