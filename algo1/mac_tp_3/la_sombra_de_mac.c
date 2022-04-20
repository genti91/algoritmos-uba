#include "la_sombra_de_mac.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utiles.h"

#define MAC_EJ "\U0001f466"
#define BLOO_EJ "\U0001f47b"
#define ROJO "\e[31m"
#define VERDE "\e[32m"
#define RESET "\e[0m"
#define VELA_EJ "\U0001f56f"
#define POZO_EJ "\U0001f573"
#define INTERRUPTOR_EJ "\U0001f579"
#define PORTAL_EJ "🌀"
#define ESCALERA_EJ "\U0001fa9c"
#define MONEDA_EJ "\U0001fa99"
#define LLAVE_EJ "\U0001f5dd"
#define PUERTA_EJ "\U0001f6aa"

const char ARRIBA = 'W';
const char ABAJO = 'S';
const char DERECHA = 'D';
const char IZQUIERDA = 'A';
const char EXIT = 'X';
const char VIDA_EXTRA = 'V';

const char BLOO = 'B';
const char MAC = 'M';

const char VELA = 'V';
const char POZO = 'W';
const char INTERRUPTOR = 'O'; 
const char PORTAL = 'P';
const char ESCALERA = 'E';
const char MONEDA = 'C';
const char LLAVE = 'L';
const char PUERTA = 'D';
const char PARED = '#';

const int CANT_VELAS_L1 = 5;
const int CANT_POZOS_L1 = 15;
const int CANT_INTERRUPTORES_L1 = 1;
const int CANT_MONEDAS_L1 = 10;
const int CANT_LLAVES_L1 = 0;
const int TOPE_OBS_L1 = 21;
const int TOPE_HERR_L1 = 21;

const int CANT_VELAS_L2 = 10;
const int CANT_POZOS_L2 = 20;
const int CANT_INTERRUPTORES_L2 = 2;
const int CANT_MONEDAS_L2 = 15;
const int CANT_LLAVES_L2 = 1;
const int TOPE_OBS_L2 = 34;
const int TOPE_HERR_L2 = 32;

const int CANT_VELAS_L3 = 12;
const int CANT_POZOS_L3 = 30;
const int CANT_INTERRUPTORES_L3 = 4;
const int CANT_MONEDAS_L3 = 15;
const int CANT_LLAVES_L3 = 1;
const int TOPE_OBS_L3 = 50;
const int TOPE_HERR_L3 = 32;

const char CANT_PUERTAS = 1;

bool coordenadas_iguales(coordenada_t a, coordenada_t b){

	return ((a.fila == b.fila) && (a.col == b.col));
}

void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){

	(*ref_personaje).posicion = arranque_personaje;
	(*ref_personaje).vida = 3;
	(*ref_personaje).puntos = 0;
	(*ref_personaje).tiene_llave = false;
	(*ref_personaje).interruptor_apretado = false;
}

void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){

	(*ref_sombra).posicion = arranque_sombra;
	(*ref_sombra).esta_viva = true;
}

bool es_movimiento_valido(char movimiento){

	return (movimiento == ARRIBA || movimiento == ABAJO || movimiento == DERECHA || movimiento == IZQUIERDA || movimiento == EXIT 
			|| movimiento == VIDA_EXTRA);
}

void pedir_movimiento(char* ref_movimiento){

	printf(VERDE "Ingrese un movimiento: W A S D.\nIngrese V para comprar una vida pro 200 puntos\nIngrese X para terminar el juego.\n" RESET);
	scanf(" %c", ref_movimiento);
	
	while(!(es_movimiento_valido(*ref_movimiento))){
		printf("Movimiento incorrecto\n");
		scanf(" %c", ref_movimiento);
	}

}

bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){

	return (posicion.col >= max_ancho || posicion.fila < 0 || posicion.col < 0 || posicion.fila >= max_alto);
}

// Procedimiento que recibe sombra_t, el movimiento y mueve a sombra
void mover_sombra(personaje_t personaje, sombra_t* ref_sombra, char movimiento){

	int col_som = (*ref_sombra).posicion.col;
	int fil_som = (*ref_sombra).posicion.fila;

	if ((*ref_sombra).esta_viva){

		if (movimiento == ABAJO && fil_som < (MAX_FILAS-1)){
			(*ref_sombra).posicion.fila++;

		}else if (movimiento == ARRIBA && fil_som > 0){
				(*ref_sombra).posicion.fila--;

		}else if (movimiento == IZQUIERDA){

			if (personaje.interruptor_apretado && col_som > 0){
					(*ref_sombra).posicion.col--;

			}else if (col_som < (MAX_COLUMNAS-1)){
				(*ref_sombra).posicion.col++;
			}

		}else if (movimiento == DERECHA){

			if (personaje.interruptor_apretado && col_som < (MAX_COLUMNAS-1)){
				(*ref_sombra).posicion.col++;

			}else if (col_som > 0){
				(*ref_sombra).posicion.col--;
			}
		}
	}
}

// Procedimiento que recibe personaje_t, el movimiento y mueve el personaje
void mover_solo_personaje(personaje_t* ref_personaje, char movimiento){

	int col_per = (*ref_personaje).posicion.col;
	int fil_per = (*ref_personaje).posicion.fila;

	if (movimiento == ABAJO && fil_per < (MAX_FILAS-1)){
		(*ref_personaje).posicion.fila++;

	}else if (movimiento == ARRIBA && fil_per > 0){
		(*ref_personaje).posicion.fila--;

	}else if (movimiento == IZQUIERDA && col_per > 0){
		(*ref_personaje).posicion.col--;

	}else if (movimiento == DERECHA && col_per < (MAX_COLUMNAS-1)){
		(*ref_personaje).posicion.col++;
	}
}

coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){

	coordenada_t coordenada_aleatoria;

	coordenada_aleatoria.fila = rand() % (max_ancho);
	coordenada_aleatoria.col = rand() % (max_alto);

	return coordenada_aleatoria;

}

int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){

	int posicionf = -1;

	for (int i = 0; i < tope; ++i)
	{
		if (elementos[i].coordenada.fila == posicion.fila && elementos[i].coordenada.col == posicion.col){

			posicionf = i;
			i = tope;
			
		}
	}

	return posicionf;
}

// Procedimiento que recibe matriz y la carga con un fondo 
void cargar_fondo(char fondo[MAX_FILAS][MAX_COLUMNAS]){

	for (int i = 0; i < MAX_FILAS; ++i){

		for (int j = 0; j < MAX_COLUMNAS; ++j){

			fondo[i][j] = ' ';
		}
	}

	for (int i = 0; i < MAX_FILAS; ++i){
		fondo[i][0] = '|';
		fondo[i][((MAX_COLUMNAS-1))] = '|';
	}
	for (int i = 0; i < MAX_COLUMNAS; ++i){
		fondo[(MAX_FILAS-1)][i] = '~';
		fondo[0][i] = '~';
		fondo[(MAX_FILAS-1)][0] = '~';
	}
}

// Procedimiento que recbe juego_t, una matriz y pone los obstaculos, herramientas y paredes en la matriz.
void cargar_mapa(juego_t juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){

	for (int i = 0; i < juego.niveles[((juego.nivel_actual)-1)].tope_obstaculos; ++i){
		terreno[juego.niveles[((juego.nivel_actual)-1)].obstaculos[i].coordenada.fila][juego.niveles[((juego.nivel_actual)-1)].obstaculos[i].coordenada.col] = juego.niveles[((juego.nivel_actual)-1)].obstaculos[i].tipo;
	}

	for (int i = 0; i < juego.niveles[((juego.nivel_actual)-1)].tope_paredes; ++i){
		terreno[juego.niveles[((juego.nivel_actual)-1)].paredes[i].fila][juego.niveles[((juego.nivel_actual)-1)].paredes[i].col] = PARED;
	}

	for (int i = 0; i < juego.niveles[((juego.nivel_actual)-1)].tope_herramientas; ++i){
		terreno[juego.niveles[((juego.nivel_actual)-1)].herramientas[i].coordenada.fila][juego.niveles[((juego.nivel_actual)-1)].herramientas[i].coordenada.col] = juego.niveles[((juego.nivel_actual)-1)].herramientas[i].tipo;
	}
}

// Funcion que recibe una matriz y la imprime
void imprimir_matriz(char terreno[MAX_FILAS][MAX_COLUMNAS]){

	for (int i = 0; i < MAX_FILAS; ++i){

		for (int j = 0; j < MAX_COLUMNAS; ++j){

			if (terreno[i][j] == BLOO){
				printf("%s",BLOO_EJ);
			}else if (terreno[i][j] == MAC){
				printf("%s",MAC_EJ);
			}else if(terreno[i][j] == VELA){
				printf("%s ",VELA_EJ);
			}else if(terreno[i][j] == POZO){
				printf("%s ",POZO_EJ);
			}else if(terreno[i][j] == INTERRUPTOR){
				printf("%s ",INTERRUPTOR_EJ);
			}else if(terreno[i][j] == PORTAL){
				printf("%s",PORTAL_EJ);
			}else if(terreno[i][j] == ESCALERA){
				printf("%s",ESCALERA_EJ);
			}else if(terreno[i][j] == MONEDA){
				printf("%s",MONEDA_EJ);
			}else if(terreno[i][j] == PUERTA){
				printf("%s", PUERTA_EJ);
			}else if(terreno[i][j] == LLAVE){
				printf("%s ",LLAVE_EJ);
			}else{

				printf(ROJO " %c" RESET, terreno[i][j]);

			}
		}
		printf("\n");
	}
}

void imprimir_terreno(juego_t juego){

	char terreno[MAX_FILAS][MAX_COLUMNAS];

	cargar_fondo(terreno);
	cargar_mapa(juego, terreno);

	terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col] = BLOO;
	terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col] = MAC;

	imprimir_matriz(terreno);
}

// Funcion que recibe una coordenada_t y un vector de coordenada_t con su tope y se fija si la coordenada_t esta en
// el vector de coordenada_t. En el caso de que la coordenada este en el vector retorna true.
bool pared_igual_a_elemento(coordenada_t elemento, coordenada_t paredes[MAX_PAREDES], int tope_paredes){

	bool iguales_coordenadas = false;

	for (int j = 0; j < tope_paredes && !iguales_coordenadas; ++j){

		if (coordenadas_iguales(elemento, paredes[j])){

			iguales_coordenadas = true;
		}
	}

	return iguales_coordenadas;
}

// Funcion que recibe juego_t y se fija si personaje esta en algun obstaculo, herramienta o pared.
// En el caso que el personaje se encuentren en una coordenada ocupada retorna false.
bool personaje_en_espacio_libre(juego_t juego){

	return  buscar_elemento(juego.niveles[juego.nivel_actual - 1].obstaculos, juego.niveles[juego.nivel_actual - 1].tope_obstaculos, juego.personaje.posicion) != -1 || 
			buscar_elemento(juego.niveles[juego.nivel_actual - 1].herramientas, juego.niveles[juego.nivel_actual - 1].tope_herramientas, juego.personaje.posicion) != -1 ||
			pared_igual_a_elemento(juego.personaje.posicion, juego.niveles[juego.nivel_actual - 1].paredes, juego.niveles[juego.nivel_actual - 1].tope_paredes);
}

// Funcion que recibe juego_t y se fija si sombra esta en algun obstaculo, herramienta o pared.
// En el caso que el sombra se encuentren en una coordenada ocupada retorna false.
bool sombra_en_espacio_libre(juego_t juego){
	return  buscar_elemento(juego.niveles[juego.nivel_actual - 1].obstaculos, juego.niveles[juego.nivel_actual - 1].tope_obstaculos, juego.sombra.posicion) != -1 || 
			buscar_elemento(juego.niveles[juego.nivel_actual - 1].herramientas, juego.niveles[juego.nivel_actual - 1].tope_herramientas, juego.sombra.posicion) != -1 || 
			pared_igual_a_elemento(juego.sombra.posicion, juego.niveles[juego.nivel_actual - 1].paredes, juego.niveles[juego.nivel_actual - 1].tope_paredes);
}

// Procedimiento que recibe juego_t e inicializa a sombra y personaje en coordenadas que no eseten ocupadas
void inicializar_personaje_y_sombra(juego_t* juego){
	do{
		coordenada_t cooraleatoria = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));
		inicializar_personaje(&(*juego).personaje, cooraleatoria);
		coordenada_t posicion_sombra;
		posicion_sombra.fila = (*juego).personaje.posicion.fila;
		posicion_sombra.col = (MAX_COLUMNAS-1) - (*juego).personaje.posicion.col;
		inicializar_sombra(&(*juego).sombra, posicion_sombra);
	}while(personaje_en_espacio_libre((*juego)) || sombra_en_espacio_libre((*juego)));
}

// Procedimiento que recibe juego_t y asigna coordenadas libres a el personaje y sombra.
void reubicar_personajes(juego_t* juego){
	do{
		(*juego).personaje.posicion = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));
		(*juego).sombra.posicion.fila = (*juego).personaje.posicion.fila;
		(*juego).sombra.posicion.col = (MAX_COLUMNAS-1) - (*juego).personaje.posicion.col;
	}while(personaje_en_espacio_libre((*juego)) || sombra_en_espacio_libre((*juego)));
}

// Funcion que recibe juego_t un nivel, un tope y asigna a los obstaculos en coordenadas libres.
bool asignar_obstaculos(juego_t* juego, int i, int nivel){
	return ((buscar_elemento((*juego).niveles[nivel].obstaculos, i, (*juego).niveles[nivel].obstaculos[i].coordenada) != -1 || 
			pared_igual_a_elemento((*juego).niveles[nivel].obstaculos[i].coordenada, (*juego).niveles[nivel].paredes, (*juego).niveles[nivel].tope_paredes)));
}

// Funcion que recibe juego_t un tope y un nivel y se fija que las herramientas no se superpongan con los obstaculos y las paredes.
bool asignar_herramientas(juego_t* juego, int i, int nivel, int tope_obs){
	return ((buscar_elemento((*juego).niveles[nivel].herramientas, i, (*juego).niveles[nivel].herramientas[i].coordenada) != -1 || 
			buscar_elemento((*juego).niveles[nivel].obstaculos, tope_obs, (*juego).niveles[nivel].herramientas[i].coordenada) != -1 || 
			pared_igual_a_elemento((*juego).niveles[nivel].herramientas[i].coordenada, (*juego).niveles[nivel].paredes, (*juego).niveles[nivel].tope_paredes)));
}

// Procedimento que recibe una referencia de juego_t con la cantidad de cada obstaculos que se quiere agregar
// y las agrega al juego.
void cargar_obstaculos(juego_t* juego, int nivel, int velas, int pozos, int interruptores){

	for (int i = 0; i < (*juego).niveles[nivel].tope_obstaculos; ++i){

		do{
			(*juego).niveles[nivel].obstaculos[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS), (MAX_FILAS));

			if(i < velas){
				(*juego).niveles[nivel].obstaculos[i].tipo = VELA;
			}else if(i < velas + pozos){
				(*juego).niveles[nivel].obstaculos[i].tipo = POZO;
			}else if (i < velas + pozos + interruptores){
				(*juego).niveles[nivel].obstaculos[i].tipo = INTERRUPTOR;
			}else{
				(*juego).niveles[nivel].obstaculos[i].tipo = PORTAL;
			}
		}while(asignar_obstaculos(juego, i, nivel));
	}
}

// Procedimento que recibe una referencia de juego_t con la cantidad de cada herramienta que se quiere agregar
// y las agrega al juego.
void cargasr_herramientas(juego_t* juego, int nivel, int monedas, int llaves){

	for (int i = 0; i < (*juego).niveles[nivel].tope_herramientas; ++i){
		if (i < monedas + llaves + CANT_PUERTAS){
			do{
				(*juego).niveles[nivel].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS), (MAX_FILAS));

				if (i < monedas){
					(*juego).niveles[nivel].herramientas[i].tipo = MONEDA;
				}else if(i < monedas + llaves){
					(*juego).niveles[nivel].herramientas[i].tipo = LLAVE;
				}else{
					(*juego).niveles[nivel].herramientas[i].tipo = PUERTA;
				}
			}while(asignar_herramientas(juego, i, nivel, (*juego).niveles[nivel].tope_obstaculos));
		}else{
			do{
				(*juego).niveles[nivel].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS), (MAX_FILAS));
				(*juego).niveles[nivel].herramientas[i].tipo = ESCALERA;
			}while(!pared_igual_a_elemento((*juego).niveles[nivel].herramientas[i].coordenada, (*juego).niveles[nivel].paredes, (*juego).niveles[nivel].tope_paredes) || buscar_elemento((*juego).niveles[nivel].herramientas, i, (*juego).niveles[nivel].herramientas[i].coordenada) != -1 );
		}
	}
}

// Procedimiento que recibe juego_t e inicializa el nivel 1
void inicializar_nivel_1(juego_t* juego){

	(*juego).niveles[0].numero_nivel = 1;
	(*juego).niveles[0].tope_obstaculos = TOPE_OBS_L1;
	(*juego).niveles[0].tope_herramientas = TOPE_HERR_L1;

	(*juego).niveles[0].tope_paredes = 0;
	obtener_mapa((*juego).niveles[0].paredes, &((*juego).niveles[0].tope_paredes));

	cargar_obstaculos(juego, 0, CANT_VELAS_L1, CANT_POZOS_L1, CANT_INTERRUPTORES_L1);
	cargasr_herramientas(juego, 0, CANT_MONEDAS_L1, CANT_LLAVES_L1);
}

// Procedimiento que recibe juego_t e inicializa el nivel 2
void inicializar_nivel_2(juego_t* juego){

	(*juego).niveles[1].numero_nivel = 2;
	(*juego).niveles[1].tope_obstaculos = TOPE_OBS_L2;
	(*juego).niveles[1].tope_herramientas = TOPE_HERR_L2;

	(*juego).niveles[1].tope_paredes = 0;
	obtener_mapa((*juego).niveles[1].paredes, &((*juego).niveles[1].tope_paredes));

	cargar_obstaculos(juego, 1, CANT_VELAS_L2, CANT_POZOS_L2, CANT_INTERRUPTORES_L2);
	cargasr_herramientas(juego, 1, CANT_MONEDAS_L2, CANT_LLAVES_L2);

}

// Procedimiento que recibe juego_t e inicializa el nivel 3
void inicializar_nivel_3(juego_t* juego){

	(*juego).niveles[2].numero_nivel = 3;
	(*juego).niveles[2].tope_obstaculos = TOPE_OBS_L3;
	(*juego).niveles[2].tope_herramientas = TOPE_HERR_L3;

	(*juego).niveles[2].tope_paredes = 0;
	obtener_mapa((*juego).niveles[2].paredes, &((*juego).niveles[2].tope_paredes));

	cargar_obstaculos(juego, 2, CANT_VELAS_L3, CANT_POZOS_L3, CANT_INTERRUPTORES_L3);
	cargasr_herramientas(juego, 2, CANT_MONEDAS_L3, CANT_LLAVES_L2);

}


void inicializar_juego(juego_t* juego){

	cargar_mapas();
	(*juego).nivel_actual = 1;
	inicializar_nivel_1(juego);
	inicializar_nivel_2(juego);
	inicializar_nivel_3(juego);
	inicializar_personaje_y_sombra(juego);
}

int estado_juego(juego_t juego){

	int estado = 0;

	if (juego.nivel_actual == 3 && juego.sombra.esta_viva){
		estado = 1;
	} 
	if(juego.personaje.vida < 1){
		estado = -1;
	}
	return estado;
}

// Funcion que recibe dos coordenadas y retorna la distancia manhattan
int distancia_manhattan(coordenada_t personaje, coordenada_t puerta){

	int distancia_manhattan;

	int fila = personaje.fila - puerta.fila;
	int columna = personaje.col - puerta.col;

	if (fila < 0){
		fila *= -1;
	}
	if (columna < 0){
		columna *= -1;
	}

	distancia_manhattan = fila + columna;

	return distancia_manhattan;
}

int estado_nivel(juego_t juego){

	int estado = 0;

	for (int i = 0; i <= juego.niveles[(juego.nivel_actual)-1].tope_herramientas; ++i){

		if (juego.niveles[(juego.nivel_actual)-1].herramientas[i].tipo == PUERTA){

			if (distancia_manhattan(juego.personaje.posicion, juego.niveles[(juego.nivel_actual)-1].herramientas[i].coordenada) <= 1 && 
				distancia_manhattan(juego.sombra.posicion, juego.niveles[(juego.nivel_actual)-1].herramientas[i].coordenada) <= 1 
				&& juego.sombra.esta_viva){

				if (juego.nivel_actual == 1){
					estado = 1;
				}
				if (juego.nivel_actual > 1 && juego.personaje.tiene_llave){
					estado = 1;
				}
			}
		}
	}

	return estado;
}

// Funcion que recibe juego_t y se fija si sombra esta en el rango de la vela y retorna true si la sombra 
// esta en el rango de la vela.
bool sombra_en_vela(elemento_t obstaculos[MAX_ELEMENTOS], int tope_obs, coordenada_t sombra){

	bool sombra_en_vela = false;

	for (int i = 0; i < tope_obs && !sombra_en_vela; ++i){

		if(obstaculos[i].tipo == VELA){
			
			int elemento_fila = obstaculos[i].coordenada.fila;
			int elemento_col = obstaculos[i].coordenada.col;

			if (abs(sombra.fila - elemento_fila) <= 1 && abs(sombra.col - elemento_col) <= 1){
				sombra_en_vela = true;
			}
		}
	}

	return sombra_en_vela;
}

void mover_personaje(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){
	mover_solo_personaje(ref_personaje, movimiento);
	mover_sombra((*ref_personaje), ref_sombra, movimiento);
}

// Procedimiento que recibe juego_t, una posicion y elimina a un elemento de el vector herramientas.
void eliminar_herramienta(juego_t* juego, int i){
	(*juego).niveles[(((*juego).nivel_actual)-1)].herramientas[i] = (*juego).niveles[(((*juego).nivel_actual)-1)].herramientas[(((*juego).niveles[(((*juego).nivel_actual)-1)].tope_herramientas)-1)];
	(*juego).niveles[(((*juego).nivel_actual)-1)].tope_herramientas--;
}

// Procedimiento que recibe juego_t, una posicion y elimina a un elemento de el vector obstaculos.
void eliminar_obstaculo(juego_t* juego, int i){
	(*juego).niveles[(((*juego).nivel_actual)-1)].obstaculos[i] = (*juego).niveles[(((*juego).nivel_actual)-1)].obstaculos[(((*juego).niveles[(((*juego).nivel_actual)-1)].tope_obstaculos)-1)];
	(*juego).niveles[(((*juego).nivel_actual)-1)].tope_obstaculos--;
}
// Funcion que recibe un movimiento y retorna el movimiento contrario
char movimiento_contrario(char movimiento){
	if (movimiento == ABAJO){
		movimiento = ARRIBA;
	}else if (movimiento == ARRIBA){
		movimiento = ABAJO;
	}else if (movimiento == IZQUIERDA){
		movimiento = DERECHA;
	}else if (movimiento == DERECHA){
		movimiento = IZQUIERDA;
	}
	return movimiento;
}

// Funcion que recive una coordenada_t y se fija que no este afuera de el mapa
bool rango_personaje(coordenada_t personaje, char movimiento){

	bool rango_personaje_ = true;
	coordenada_t rango_personaje;
	rango_personaje = personaje;

	if(movimiento == ARRIBA){
		rango_personaje.fila -= 1;
	} 
	if(movimiento == IZQUIERDA){
		rango_personaje.col -= 1;
	} 
	if(movimiento == ABAJO){
		rango_personaje.fila += 1;
	} 
	if(movimiento == DERECHA){
		rango_personaje.col += 1;
	} 

	if (esta_dentro_rango(rango_personaje, MAX_FILAS, MAX_COLUMNAS)){
		rango_personaje_ = false;
	}

	return rango_personaje_;
}

// Procedimiento que recibe juego_t, movimiento y se fija si el personaje esta en una escalera y si esta, lo mueve hasta que este en 
// un espacio libre. Si el personaje se encuentra con una pared lo mueve con el movimiento contrario hasta que este en un espacio libre.
void personaje_en_pared(juego_t* juego, char movimiento, char movimienot_original){

	for (int i = 0; i < (*juego).niveles[(*juego).nivel_actual-1].tope_paredes; ++i){

		if (coordenadas_iguales((*juego).niveles[(*juego).nivel_actual-1].paredes[i], (*juego).personaje.posicion)){

			int posicion_escalera = buscar_elemento((*juego).niveles[(*juego).nivel_actual-1].herramientas, (*juego).niveles[(*juego).nivel_actual-1].tope_herramientas, (*juego).personaje.posicion);

			if (posicion_escalera != -1 && rango_personaje((*juego).personaje.posicion, movimiento)){

				mover_solo_personaje((&(*juego).personaje), movimiento);
				personaje_en_pared(juego, movimiento, movimienot_original);

				if(coordenadas_iguales((*juego).niveles[(*juego).nivel_actual-1].paredes[i], (*juego).personaje.posicion)){

					mover_solo_personaje((&(*juego).personaje), movimiento_contrario(movimiento));
					personaje_en_pared(juego, movimiento_contrario(movimiento), movimienot_original);

				}else if(movimienot_original == movimiento){
					eliminar_herramienta(juego, posicion_escalera);
				}

				return;
			}

			mover_solo_personaje((&(*juego).personaje), movimiento_contrario(movimiento));
			return;
		}
	}
}

// Procedimiento que recibe juego_t y movimiento y se fija si sombra esta en una pared. En el caso de
// que sombra este sobre una pared, mueve a la sombra con el movimiento contrario.
void sombra_en_pared(juego_t* juego, char movimiento){

	for (int i = 0; i < (*juego).niveles[(*juego).nivel_actual-1].tope_paredes; ++i){
		if (coordenadas_iguales((*juego).niveles[(*juego).nivel_actual-1].paredes[i], (*juego).sombra.posicion)){

			mover_sombra((*juego).personaje, (&(*juego).sombra), movimiento_contrario(movimiento));

			return;
		}
	}
}

// Procedimiento que recibe el juego y se fija si el personaje esta en una moneda o llave y si sombra esta en alguna moneda.
// En el caso de que esten sobre alguna de estas herramientas, realica la accion correspondiente.
void personaje_en_herramienta(juego_t* juego){

	for (int i = 0; i < ((*juego).niveles[(*juego).nivel_actual-1].tope_herramientas); ++i){

		if (coordenadas_iguales((*juego).sombra.posicion, (*juego).niveles[(*juego).nivel_actual-1].herramientas[i].coordenada) && 
			(*juego).niveles[(*juego).nivel_actual-1].herramientas[i].tipo == MONEDA){
			(*juego).personaje.puntos += (rand() % 11 + 10);
			eliminar_herramienta(juego, i);
		}

		if (coordenadas_iguales((*juego).personaje.posicion, (*juego).niveles[(*juego).nivel_actual-1].herramientas[i].coordenada)){

			if ((*juego).niveles[(*juego).nivel_actual-1].herramientas[i].tipo == MONEDA){
				(*juego).personaje.puntos += (rand() % 11 + 10);
				eliminar_herramienta(juego, i);
				return;
			}

			if ((*juego).niveles[(*juego).nivel_actual-1].herramientas[i].tipo == LLAVE){
				(*juego).personaje.tiene_llave = true;
				eliminar_herramienta(juego, i);
				return;
			}
		}
	}
}

// Procedimiento que recibe el juego y se fija si el personaje o sombra esta en algun obstaculo.
// En el caso de que esten sobre algun obstaculo, realica la accion correspondiente.
void personaje_en_obstaculo(juego_t* juego){
	for (int i = 0; i < ((*juego).niveles[(*juego).nivel_actual-1].tope_obstaculos); ++i){

		if (coordenadas_iguales((*juego).sombra.posicion, (*juego).niveles[(*juego).nivel_actual-1].obstaculos[i].coordenada)){

			if ((*juego).niveles[(*juego).nivel_actual-1].obstaculos[i].tipo == PORTAL){

				coordenada_t coordenada_aux;
				coordenada_aux = (*juego).personaje.posicion;
				(*juego).personaje.posicion = (*juego).sombra.posicion;
				(*juego).sombra.posicion = coordenada_aux;
			}
		}

		if (coordenadas_iguales((*juego).personaje.posicion, (*juego).niveles[(*juego).nivel_actual-1].obstaculos[i].coordenada)){

			if ((*juego).niveles[((*juego).nivel_actual)-1].obstaculos[i].tipo == POZO){

				(*juego).personaje.vida--;
				reubicar_personajes(juego);
				return;
			}

			if ((*juego).niveles[(*juego).nivel_actual-1].obstaculos[i].tipo == VELA){

				eliminar_obstaculo(juego, i);
				return;
			}

			if ((*juego).niveles[(*juego).nivel_actual-1].obstaculos[i].tipo == INTERRUPTOR){

				if ((*juego).personaje.interruptor_apretado){
					(*juego).personaje.interruptor_apretado = false;
				}else{
					(*juego).personaje.interruptor_apretado = true;
				}
				return;
			}
		}
	}
}

// Procedimiento que recibe juego_t y pasa de nivel
void pasar_nivel(juego_t* juego){

	if (estado_nivel(*juego) == 1){
		if ((*juego).nivel_actual < 3){
			(*juego).nivel_actual++;
			(*juego).personaje.interruptor_apretado = false;
			(*juego).personaje.tiene_llave = false;
			reubicar_personajes(juego);
		}
	}
}

void realizar_jugada(juego_t* juego){

	char movimiento;

	pedir_movimiento(&movimiento);

	mover_personaje((&(*juego).personaje), (&(*juego).sombra), movimiento);

	personaje_en_pared(juego, movimiento, movimiento);

	sombra_en_pared(juego, movimiento);

	if (movimiento == EXIT){
		(*juego).personaje.vida = 0;
		return;
	}
	if ((movimiento == VIDA_EXTRA) && ((*juego).personaje.puntos >= 200) && ((*juego).personaje.vida < 3)){
		(*juego).personaje.vida++;
		(*juego).personaje.puntos -= 200;
		return;
	}

	personaje_en_herramienta(juego);

	personaje_en_obstaculo(juego);

	if (sombra_en_vela((*juego).niveles[(*juego).nivel_actual-1].obstaculos, (*juego).niveles[(*juego).nivel_actual-1].tope_obstaculos, (*juego).sombra.posicion)){
		(*juego).sombra.esta_viva = false;
	}


	if (coordenadas_iguales((*juego).sombra.posicion, (*juego).personaje.posicion)){
		if ((*juego).sombra.esta_viva == false){
			(*juego).personaje.puntos -= 50;
		}
		if (!sombra_en_vela((*juego).niveles[(*juego).nivel_actual-1].obstaculos, (*juego).niveles[(*juego).nivel_actual-1].tope_obstaculos, (*juego).sombra.posicion)){
			(*juego).sombra.esta_viva = true;
		}
	}

	pasar_nivel(juego);	
}

// Procedimiento que recibe un vector de configuraciones_t y guarda las configuraciones de el archivo config.txt en el
// vector de configuracionse
void leer_configuraciones(configuraciones_t configuraciones[MAX_NIVELES]){

	FILE* configs = fopen("config.txt", "r");
	if (!configs){
		perror("No se pudo abrir el archivo!\n");
		return;
	}

	while(!feof(configs)){

		int nivel;
		char linea[100];
		fscanf(configs, "%s\n", linea);

		//strtok(linea, "N");
		char* nivel_actual = strtok(linea, "_");
		char* etiqueta = strtok(NULL, "=");
		char* valor = strtok(NULL, "\0");

		if (strcmp(nivel_actual, "N1") == 0){
			nivel = 0;
		}else if(strcmp(nivel_actual, "N2") == 0){
			nivel = 1;
		}else if(strcmp(nivel_actual, "N3") == 0){
			nivel = 2;
		}

		if (strcmp(etiqueta, "POZOS") == 0){
			configuraciones[nivel].pozos = atoi(valor);
		}else if(strcmp(etiqueta, "VELAS") == 0){
			configuraciones[nivel].velas = atoi(valor);
		}else if(strcmp(etiqueta, "INTERRUPTORES") == 0){
			configuraciones[nivel].interruptores = atoi(valor);
		}else if(strcmp(etiqueta, "PORTALES") == 0){
			configuraciones[nivel].portales = atoi(valor);
		}else if(strcmp(etiqueta, "MONEDAS") == 0){
			configuraciones[nivel].monedas = atoi(valor);
		}else if(strcmp(etiqueta, "ESCALERAS") == 0){
			configuraciones[nivel].escaleras = atoi(valor);
		}else if(strcmp(etiqueta, "LLAVES") == 0){
			configuraciones[nivel].llaves = atoi(valor);
		}
	}

	fclose(configs);

}

// Procedimiento que recive un vector de niveles_t y configuraciones_t y calcula los topes de
// los obstaculos y las herramientas para agregarlos en el vector de niveles.
void cargar_topes(nivel_t niveles[MAX_NIVELES], configuraciones_t configuraciones[MAX_NIVELES]){
	int tope_obs;
	int tope_herr;
	for (int i = 0; i < MAX_NIVELES; ++i){
		tope_obs = configuraciones[i].pozos + configuraciones[i].velas + configuraciones[i].interruptores + configuraciones[i].portales;
		tope_herr = configuraciones[i].monedas + configuraciones[i].escaleras + configuraciones[i].llaves + CANT_PUERTAS;
		niveles[i].tope_obstaculos = tope_obs;
		niveles[i].tope_herramientas = tope_herr;
	}
}

// Pocedimento que recibe juego_t con configuraciones_t y carga los obstaculos y las herramintas de el vector de 
// configuraciones en el juego
void cargar_configuracion(juego_t* juego, configuraciones_t configuraciones[MAX_NIVELES]){

	cargar_topes((*juego).niveles, configuraciones);

	for (int i = 0; i < MAX_NIVELES; ++i){
		cargar_obstaculos(juego, i, configuraciones[i].velas, configuraciones[i].pozos, configuraciones[i].interruptores);
		cargasr_herramientas(juego, i, configuraciones[i].monedas, configuraciones[i].llaves);
	}
}

// Procedimiento que recibe una referencia de juego_t y cambia las configuraciones del juego.
void configurar_juego(juego_t* juego){

	configuraciones_t configuraciones[4];

	leer_configuraciones(configuraciones);

	cargar_configuracion(juego, configuraciones);

}