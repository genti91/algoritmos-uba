#ifndef __ARCHIVOS__
#define __ARCHIVOS__

#define MAX_NOMBRE 100
#define MAX_PARTIDAS 1000

#define ARCHIVO_A_MODIFICAR 2
#define COMANDO_A_EJECUTAR 1
#define NOMBRE_A_ELIMINAR 3
#define APELLIDO_A_ELIMINAR 4

#define ARCHIVO_NO_ENCONTRADO 1

// Procedimiento que recibe el nombre de un archivo y agrega una partida ingresada por el usuario.
int agregar_partida_a_lista(char nombre_archivo[MAX_NOMBRE]);

// Procedimiento que recibe el nombre de un archivo con un nombre y elimina la partida que tenga el nombre ingresado.
int eliminar_partida_de_lista(char nombre_archivo[MAX_NOMBRE], char nombre[MAX_NOMBRE]);

// Procedimiento que recibe el nombre de un archivo y ordena todas las partidas que se encuentran en el archivo.
int ordenar_partidas_en_lista(char nombre_archivo[MAX_NOMBRE]);

#endif