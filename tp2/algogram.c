//scanf en publicar solo toma hasta espacio, no se puede publicar una oracion con espacio.
//los usuarios contienen \n.
//no se ven los contenidos de los posts en el ver_siguiente_feed.
//falta liberar memoria.

#define _POSIX_C_SOURCE 200809L
#include "tp2.h"
#include "postusuario.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PARAM_USUARIOS 1
#define VALOR_STDIN 2


hash_t *crear_hash_de_usuarios(FILE *mi_archivo) {
    hash_t* usuarios = hash_crear(usuario_destruir_envoltorio);
	int id_usuario = 0;
	char *nombre = NULL;
	size_t tam = 0;
	while (!feof(mi_archivo)){
		size_t largo = getline(&nombre, &tam, mi_archivo);
		if (strcmp(nombre, "")){
			nombre[largo-1] = '\0';
		} else {
			continue;
		}
        usuario_t* usuario = usuario_crear(nombre, id_usuario);
        if (usuario == NULL){
            hash_destruir(usuarios);
            return NULL;
        }
		hash_guardar(usuarios, nombre, usuario);
		id_usuario++;
		strcpy(nombre, "");
	}
	free(nombre);
    return usuarios;
}


int main(int argc, char const *argv[]){  
    if ((argc != VALOR_STDIN)) {
		fprintf(stdout, "Error: Cantidad erronea de parametros\n");
		return -1;
	}
	FILE *mi_archivo = fopen(argv[PARAM_USUARIOS], "r");
	if (mi_archivo == NULL) {
		fprintf(stdout, "Error: Archivo usuarios inexistente o inaccesible\n");
        return -1;
    }
	hash_t *usuarios = crear_hash_de_usuarios(mi_archivo);
	fclose(mi_archivo);	
	estado_t *estado = malloc(sizeof(estado_t));
	estado->logueado = false;
	estado->usuario = NULL;
	vectorc_t *posts = vectorc_crear(post_destruir);
	char comando[50] = "";
	while (scanf("%s", comando) != EOF) {
		getchar();
		if (strcmp(comando, "login") == 0) {
			login(usuarios, estado);
		} else if (strcmp(comando, "logout") == 0) {
			logout(estado);
		} else if (strcmp(comando, "publicar") == 0) {
			publicar(usuarios, *estado, posts);
		} else if (strcmp(comando, "ver_siguiente_feed") == 0) {
			ver_siguiente_feed(*estado);
		} else if (strcmp(comando, "likear_post") == 0) {
			likear_post(posts, *estado);
		} else if (strcmp(comando, "mostrar_likes") == 0) {
			mostrar_likes(posts);
		}
	}
	free(estado);
	hash_destruir(usuarios);
	vectorc_destruir(posts);
    return 0;
}