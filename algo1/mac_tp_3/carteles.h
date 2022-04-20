#ifndef __CARTELES__
#define __CARTELES__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "la_sombra_de_mac.h"

// Procedimiento que imprime un mac y bloo por dos segundos antes de que epiece el juego.
void imprimir_intro();

// Procedimiento que imprime ganaste.
void imprimir_ganaste();

// Procedimiento que imprime perdiste.
void imprimir_perdiste();
 
// Procedimiento que imprime un cartel.
void imprimir_bloo();

// Procedimiento que imprime la informacion del personaje y sombra.
void imprimir_stats(personaje_t personaje,  sombra_t sombra, int nivel);

#endif