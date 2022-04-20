#include "la_sombra_de_mac.h"
#include <stdlib.h>
#include <stdio.h>
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
	
	do{
		scanf(" %c", ref_movimiento);
	}while(!(es_movimiento_valido(*ref_movimiento)));

}

bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){

	return ((posicion.fila <= max_ancho) && (posicion.col <= max_alto) && (posicion.fila >= 0) && (posicion.col >= 0));
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

	bool coordenadas_iguales = false;

	for (int j = 0; j < tope_paredes && !coordenadas_iguales; ++j){

		if (elemento.fila == paredes[j].fila && elemento.col == paredes[j].col){

			coordenadas_iguales = true;
		}
	}

	return coordenadas_iguales;
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

// Procedimiento que recibe juego_t e inicializa el nivel 1
void inicializar_nivel_1(juego_t* juego){

	(*juego).niveles[0].numero_nivel = 1;
	(*juego).niveles[0].tope_obstaculos = 21;
	(*juego).niveles[0].tope_herramientas = 21;

	(*juego).niveles[0].tope_paredes = 0;
	obtener_mapa((*juego).niveles[0].paredes, &((*juego).niveles[0].tope_paredes));

	for (int i = 0; i < (*juego).niveles[0].tope_obstaculos; ++i){
		do{
			(*juego).niveles[0].obstaculos[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));

			if(i < 5){
				(*juego).niveles[0].obstaculos[i].tipo = VELA;
			}else if(i < 20){
				(*juego).niveles[0].obstaculos[i].tipo = POZO;
			}else{
				(*juego).niveles[0].obstaculos[i].tipo = INTERRUPTOR;
			}
		}while(asignar_obstaculos(juego, i, 0));
	}
	for (int i = 0; i < (*juego).niveles[0].tope_herramientas; ++i){
		if (i < 11){
			do{
				(*juego).niveles[0].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));

				if (i < 1){
					(*juego).niveles[0].herramientas[i].tipo = PUERTA;
				}else{
					(*juego).niveles[0].herramientas[i].tipo = MONEDA;
				}
			}while(asignar_herramientas(juego, i, 0, (*juego).niveles[0].tope_obstaculos));
		}else{
			do{
				(*juego).niveles[0].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));
				(*juego).niveles[0].herramientas[i].tipo = ESCALERA;
			}while(!(pared_igual_a_elemento((*juego).niveles[0].herramientas[i].coordenada, (*juego).niveles[0].paredes, (*juego).niveles[0].tope_paredes)) || buscar_elemento((*juego).niveles[0].herramientas, i, (*juego).niveles[0].herramientas[i].coordenada) != -1 );
		}
	}
}

// Procedimiento que recibe juego_t e inicializa el nivel 2
void inicializar_nivel_2(juego_t* juego){

	(*juego).niveles[1].numero_nivel = 2;
	(*juego).niveles[1].tope_obstaculos = 34;
	(*juego).niveles[1].tope_herramientas = 32;

	(*juego).niveles[1].tope_paredes = 0;
	obtener_mapa((*juego).niveles[1].paredes, &((*juego).niveles[1].tope_paredes));

	for (int i = 0; i < (*juego).niveles[1].tope_obstaculos; ++i){
		do{
			(*juego).niveles[1].obstaculos[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));

			if(i < 10){
				(*juego).niveles[1].obstaculos[i].tipo = VELA;
			}else if(i < 30){
				(*juego).niveles[1].obstaculos[i].tipo = POZO;
			}else if (i < 32){
				(*juego).niveles[1].obstaculos[i].tipo = INTERRUPTOR;
			}else{
				(*juego).niveles[1].obstaculos[i].tipo = PORTAL;
			}
		}while(asignar_obstaculos(juego, i, 1));
	}
	for (int i = 0; i < (*juego).niveles[1].tope_herramientas; ++i){
		if (i < 17){
			do{
				(*juego).niveles[1].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));

				if (i < 15){
					(*juego).niveles[1].herramientas[i].tipo = MONEDA;
				}else if(i < 16){
					(*juego).niveles[1].herramientas[i].tipo = LLAVE;
				}else{
					(*juego).niveles[1].herramientas[i].tipo = PUERTA;
				}
			}while(asignar_herramientas(juego, i, 1, (*juego).niveles[1].tope_obstaculos));
		}else{
			do{
				(*juego).niveles[1].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));
				(*juego).niveles[1].herramientas[i].tipo = ESCALERA;
			}while(!(pared_igual_a_elemento((*juego).niveles[1].herramientas[i].coordenada, (*juego).niveles[1].paredes, (*juego).niveles[1].tope_paredes)) || buscar_elemento((*juego).niveles[1].herramientas, i, (*juego).niveles[1].herramientas[i].coordenada) != -1 );
		}
	}
}

// Procedimiento que recibe juego_t e inicializa el nivel 3
void inicializar_nivel_3(juego_t* juego){

	(*juego).niveles[2].numero_nivel = 3;
	(*juego).niveles[2].tope_obstaculos = 50;
	(*juego).niveles[2].tope_herramientas = 32;

	(*juego).niveles[2].tope_paredes = 0;
	obtener_mapa((*juego).niveles[2].paredes, &((*juego).niveles[2].tope_paredes));

	for (int i = 0; i < (*juego).niveles[2].tope_obstaculos; ++i){
		do{
			(*juego).niveles[2].obstaculos[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));

			if(i < 12){
				(*juego).niveles[2].obstaculos[i].tipo = VELA;
			}else if(i < 42){
				(*juego).niveles[2].obstaculos[i].tipo = POZO;
			}else if (i < 46){
				(*juego).niveles[2].obstaculos[i].tipo = INTERRUPTOR;
			}else{
				(*juego).niveles[2].obstaculos[i].tipo = PORTAL;
			}
		}while(asignar_obstaculos(juego, i, 2));
	}
	for (int i = 0; i < (*juego).niveles[2].tope_herramientas; ++i){
		if (i < 17){
			do{
				(*juego).niveles[2].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));

				if (i < 15){
					(*juego).niveles[2].herramientas[i].tipo = MONEDA;
				}else if(i < 16){
					(*juego).niveles[2].herramientas[i].tipo = LLAVE;
				}else{
					(*juego).niveles[2].herramientas[i].tipo = PUERTA;
				}
			}while(asignar_herramientas(juego, i, 2, (*juego).niveles[2].tope_obstaculos));
		}else{
			do{
				(*juego).niveles[2].herramientas[i].coordenada = coordenada_aleatoria((MAX_COLUMNAS-1), (MAX_FILAS-1));
				(*juego).niveles[2].herramientas[i].tipo = ESCALERA;
			}while(!pared_igual_a_elemento((*juego).niveles[2].herramientas[i].coordenada, (*juego).niveles[2].paredes, (*juego).niveles[2].tope_paredes) || buscar_elemento((*juego).niveles[2].herramientas, i, (*juego).niveles[2].herramientas[i].coordenada) != -1 );
		}
	}
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

// Procedimiento que recibe juego_t, movimiento y se fija si el personaje esta en una escalera y si esta, lo mueve hasta que este en 
// un espacio libre. Si el personaje se encuentra con una pared lo mueve con el movimiento contrario hasta que este en un espacio libre.
void personaje_en_pared(juego_t* juego, char movimiento){

	for (int i = 0; i < (*juego).niveles[(*juego).nivel_actual-1].tope_paredes; ++i){

		if (coordenadas_iguales((*juego).niveles[(*juego).nivel_actual-1].paredes[i], (*juego).personaje.posicion)){

			int posicion_escalera = buscar_elemento((*juego).niveles[(*juego).nivel_actual-1].herramientas, (*juego).niveles[(*juego).nivel_actual-1].tope_herramientas, (*juego).personaje.posicion);

			if (posicion_escalera != -1){

				mover_solo_personaje((&(*juego).personaje), movimiento);
				personaje_en_pared(juego, movimiento);

				if(coordenadas_iguales((*juego).niveles[(*juego).nivel_actual-1].paredes[i], (*juego).personaje.posicion)){

					mover_solo_personaje((&(*juego).personaje), movimiento_contrario(movimiento));
					personaje_en_pared(juego, movimiento);

				}else{
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

	personaje_en_pared(juego, movimiento);

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