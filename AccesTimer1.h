/*
 _____    ______   _____   _____
|  __ \  |  ____| |_   _| |  __ \
| |__) | | |__      | |   | |  | |
|  _  /  |  __|     | |   | |  | |
| | \ \  | |       _| |_  | |__| |
|_|  \_\ |_|      |_____| |_____/

Fichier AccessTimer1.h
ROUTIN Jocelyn, GERARDI Marcelin
Dernière modification : 08/06/2016

*/

#ifndef _AccesTimer1_
#define _AccesTimer1_

#include <Arduino.h>

#define INIT_TCNT1 3038
#define MASQUE_TOV1 B00000001

void InitTimer1(void);
void Seconde(void);

#endif