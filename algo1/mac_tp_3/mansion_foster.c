#include "carteles.h"
#include "archivos.h"

int main(int argc, char const *argv[]){

	char archivo[MAX_NOMBRE];

	bool commando_config_juego = false;
	if (argc == 2){
		if (strcmp(argv[1], "config_juego") == 0){
			commando_config_juego = true;
		}
	}

	juego_t juego;

	srand ((unsigned)time(NULL));

	inicializar_juego(&juego);


	if(commando_config_juego){
		configurar_juego(&juego);
	}


	if (argc == 1 || commando_config_juego){

		imprimir_intro();
		while((estado_juego(juego) == 0 || estado_nivel(juego) == 0) && estado_juego(juego) != -1){
			imprimir_stats(juego.personaje, juego.sombra, juego.nivel_actual);
			imprimir_terreno(juego);
			realizar_jugada(&juego);
		}
		if (estado_juego(juego) == 1 && estado_nivel(juego) == 1){
			imprimir_ganaste();
		}else{
			imprimir_perdiste();
			imprimir_bloo();
		}

	}else if (strcmp(argv[COMANDO_A_EJECUTAR], "agregar_partida") == 0){

		strcpy(archivo, argv[ARCHIVO_A_MODIFICAR]);
		if(agregar_partida_a_lista(archivo) == ARCHIVO_NO_ENCONTRADO){
			return ARCHIVO_NO_ENCONTRADO;
		}

	}else if (strcmp(argv[COMANDO_A_EJECUTAR], "eliminar_partida") == 0){

		char nombre[MAX_NOMBRE];
		strcpy(nombre, argv[NOMBRE_A_ELIMINAR]);
		if(argc == 5){
			strcat(nombre, " ");
			strcat(nombre, argv[APELLIDO_A_ELIMINAR]);
		}
		strcpy(archivo, argv[ARCHIVO_A_MODIFICAR]);
		if(eliminar_partida_de_lista(archivo, nombre) == ARCHIVO_NO_ENCONTRADO){
			return ARCHIVO_NO_ENCONTRADO;
		}

	}else if (strcmp(argv[COMANDO_A_EJECUTAR], "ordenar_partidas") == 0){

		strcpy(archivo, argv[ARCHIVO_A_MODIFICAR]);
		if (ordenar_partidas_en_lista(archivo) == ARCHIVO_NO_ENCONTRADO){
			return ARCHIVO_NO_ENCONTRADO;
		}	
	}

	return 0;
}
