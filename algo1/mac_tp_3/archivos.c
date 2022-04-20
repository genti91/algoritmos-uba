#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "archivos.h"

#define FORMATO_LECTURA_PARTIDAS "%[^;];%i;%i;%i;%[^\n]\n"
#define FORMATO_ESCRITURA_PARTIDAS "%s;%i;%i;%i;%s\n"

typedef struct partida {
	char jugador[MAX_NOMBRE];
	int nivel_llegado;
	int puntos;
	int vidas_restantes;
	bool gano;
} partida_t;

// Procedimiento que recibe el vector de partidas con su tope y lo ordena con el metodo de seleccion
void ordenar_partidas(partida_t partida[MAX_PARTIDAS], int tope){
	
	partida_t aux;
	int minimo;

	for (int i = 0; i < tope-1; ++i){
		minimo = i;
		for (int j = i+1; j < tope; ++j){
			if (strcmp(partida[j].jugador, partida[minimo].jugador) < 0){
				minimo = j;
			}
			if (strcmp(partida[j].jugador, partida[minimo].jugador) == 0){
				if (partida[j].nivel_llegado > partida[minimo].nivel_llegado){
					minimo = j;
				}
				if (partida[j].nivel_llegado == partida[minimo].nivel_llegado){
					if (partida[j].puntos > partida[minimo].puntos){
						minimo = j;
					}
				}
			}
		}
		aux = partida[i];
		partida[i] = partida[minimo];
		partida[minimo] = aux;
	}
}

// Funcion que le pide al usuario que ingrese una nueva partida y la retorna
partida_t pedir_nueva_partida(){

	partida_t partida_a_agregar;

	system("clear");
	printf("Ingrese su nombre\n");
	scanf("%[^\n]", partida_a_agregar.jugador);
	system("clear");
	printf("Hasta que nivel llegaste?\n");
	scanf("%i", &partida_a_agregar.nivel_llegado);
	system("clear");
	printf("Cuantos puntos ganaste?\n");
	scanf("%i", &partida_a_agregar.puntos);
	system("clear");
	printf("Cuantas vidas te quedaron?\n");
	scanf("%i", &partida_a_agregar.vidas_restantes);
	system("clear");
	partida_a_agregar.gano = true;
	if(partida_a_agregar.vidas_restantes == 0 || partida_a_agregar.nivel_llegado < 3){
		partida_a_agregar.gano = false;
	}

	return partida_a_agregar;

}


// Procedimiento que recibe el vector de partida, su tope, una partida para agregar y la agrega de forma ordenada
void agregar_partida(partida_t partida[MAX_PARTIDAS], int* tope, partida_t partida_a_agregar){
	partida_t aux;
	for (int i = 0; i < *tope; ++i){
		if (strcmp(partida_a_agregar.jugador, partida[i].jugador) < 0){
			aux = partida[i];
			partida[i] = partida_a_agregar;
			partida_a_agregar = aux;
		}
		if (strcmp(partida_a_agregar.jugador, partida[i].jugador) == 0){
			if (partida_a_agregar.nivel_llegado > partida[i].nivel_llegado){
				aux = partida[i];
				partida[i] = partida_a_agregar;
				partida_a_agregar = aux;
			}
			if (partida_a_agregar.nivel_llegado == partida[i].nivel_llegado){
				if (partida_a_agregar.puntos > partida[i].puntos){
					aux = partida[i];
					partida[i] = partida_a_agregar;
					partida_a_agregar = aux;
				}
			}
		}
	}
	partida[*tope] = partida_a_agregar;
	(*tope)++;
}

// Funcion que recibe el vector de partida, su tope, un jugador y busca la pocicion de el jugador que se queiere 
// eliminar y la retorna
int buscar_jugador(partida_t partida[MAX_PARTIDAS], int tope, char jugador[MAX_NOMBRE]){

	int posicion = -1;

	for (int i = 0; i < tope; ++i)
	{
		if (strcmp(partida[i].jugador, jugador) == 0){

			posicion = i;
			i = tope;
			
		}
	}
	return posicion;
}

// Procedimiento que recibe vector de partidas con su tope y el jugador que se quiere eliminar y se elimna
// del vector de forma ordenada 
void eliminar_partida(partida_t partida[MAX_PARTIDAS], int* tope, char jugador[MAX_NOMBRE]){

	int posicion = buscar_jugador(partida, *tope, jugador);

	if (posicion == -1){
		return;
	}
	for (int i = posicion; i < *tope; ++i){
		partida[i] = partida[i+1];
	}
	(*tope)--;
}

// Procedimiento que recibe el nombre de un archivo, un vector de partida_t con su tope y lee el archivo para copiar todo 
// lo del archivo adentro de el vector de partidas.
int leer_archivo_partidas(char nombre_archivo[MAX_NOMBRE], partida_t partida[MAX_PARTIDAS], int* tope){

	FILE* partidas = fopen(nombre_archivo, "r");
	if (!partidas){
		perror("No se pudo abrir el archivo!\n");
		return ARCHIVO_NO_ENCONTRADO;
	}

	char gano[3];

	while(!feof(partidas)){
		fscanf(partidas, FORMATO_LECTURA_PARTIDAS, partida[*tope].jugador, &partida[*tope].nivel_llegado, &partida[*tope].puntos, &partida[*tope].vidas_restantes, gano);
		partida[*tope].gano = strcmp(gano, "No");
		(*tope)++;
	}

	fclose(partidas);

	return 0;

}


// Procedimiento que recibe el nombre de un archivo y agrega una partida ingresada por el usuario.
int agregar_partida_a_lista(char nombre_archivo[MAX_NOMBRE]){
	partida_t partida[MAX_PARTIDAS];
	int tope_partidas = 0;
	int estado_archivo = leer_archivo_partidas(nombre_archivo, partida, &tope_partidas);
	if (estado_archivo == ARCHIVO_NO_ENCONTRADO){
		return ARCHIVO_NO_ENCONTRADO;
	}
	FILE* partidas_aux = fopen("archivo_aux.csv", "w");
	if (!partidas_aux){
		perror("No se pudo crear el archivo auxiliar!\n");
		return ARCHIVO_NO_ENCONTRADO;
	}

	partida_t partida_a_agregar = pedir_nueva_partida();
	agregar_partida(partida, &tope_partidas, partida_a_agregar);

	char gano[3];

	for (int i = 0; i < tope_partidas; ++i){
		
		strcpy(gano, "No");
		if (partida[i].gano){
			strcpy(gano, "Si");
		}

		fprintf(partidas_aux, FORMATO_ESCRITURA_PARTIDAS, partida[i].jugador, partida[i].nivel_llegado, partida[i].puntos, partida[i].vidas_restantes, gano);
	}

	fclose(partidas_aux);
	rename("archivo_aux.csv", nombre_archivo);

	return 0;
}


// Procedimiento que recibe el nombre de un archivo con un nombre y elimina la partida que tenga el nombre ingresado.
int eliminar_partida_de_lista(char nombre_archivo[MAX_NOMBRE], char nombre[MAX_NOMBRE]){
	partida_t partida[MAX_PARTIDAS];
	int tope_partidas = 0;
	int estado_archivo = leer_archivo_partidas(nombre_archivo, partida, &tope_partidas);
	if (estado_archivo == ARCHIVO_NO_ENCONTRADO){
		return ARCHIVO_NO_ENCONTRADO;
	}
	FILE* partidas_aux = fopen("archivo_aux.csv", "w");
	if (!partidas_aux){
		perror("No se pudo crear el archivo auxiliar!\n");
		return ARCHIVO_NO_ENCONTRADO;
	}

	eliminar_partida(partida, &tope_partidas, nombre);

	char gano[3];
	
	for (int i = 0; i < tope_partidas; ++i){
		
		strcpy(gano, "No");
		if (partida[i].gano){
			strcpy(gano, "Si");
		}

		fprintf(partidas_aux, FORMATO_ESCRITURA_PARTIDAS, partida[i].jugador, partida[i].nivel_llegado, partida[i].puntos, partida[i].vidas_restantes, gano);
	}

	fclose(partidas_aux);
	rename("archivo_aux.csv", nombre_archivo);

	return 0;
}

// Procedimiento que recibe el nombre de un archivo y ordena todas las partidas que se encuentran en el archivo.
int ordenar_partidas_en_lista(char nombre_archivo[MAX_NOMBRE]){
	
	partida_t partida[MAX_PARTIDAS];
	int tope_partidas = 0;
	int estado_archivo = leer_archivo_partidas(nombre_archivo, partida, &tope_partidas);
	if (estado_archivo == ARCHIVO_NO_ENCONTRADO){
		return ARCHIVO_NO_ENCONTRADO;
	}
	FILE* partidas_aux = fopen("archivo_aux.csv", "w");
	if (!partidas_aux){
		perror("No se pudo crear el archivo auxiliar!\n");
		return ARCHIVO_NO_ENCONTRADO;
	}

	ordenar_partidas(partida, tope_partidas);

	char gano[3];

	for (int i = 0; i < tope_partidas; ++i){
		
		strcpy(gano, "No");
		if (partida[i].gano){
			strcpy(gano, "Si");
		}

		fprintf(partidas_aux, FORMATO_ESCRITURA_PARTIDAS, partida[i].jugador, partida[i].nivel_llegado, partida[i].puntos, partida[i].vidas_restantes, gano);
	}

	fclose(partidas_aux);
	rename("archivo_aux.csv", nombre_archivo);

	return 0;
}