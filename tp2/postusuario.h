#ifndef ALGOGRAM_H
#define ALGOGRAM_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "abb.h"
#include "hash.h"
#include "vector_creciente.h"
#include "heap.h"

typedef struct usuario {
    int id_usuario;
    char *nombre;
    vectorc_t *ids_posts;
    heap_t *feed;
} usuario_t;

typedef struct estado {
    bool logueado;
    usuario_t *usuario;
} estado_t;

typedef struct post {
    int id;
    usuario_t *usuario;
    char* contenido;
    abb_t *usuarios_likes;
} post_t;

// Crea un post.
post_t *post_crear(usuario_t* usuario, char *texto, int id);

// Devuelve el ID del post.
int post_id(post_t *post);

// Devuelve el nombre del usuario publicador del post.
char *post_usuario(post_t *post);

// Devuelve el texto contenido del post.
char *post_contenido(post_t *post);

// Devuelve la cantidad de likes que tiene el post.
int post_cant_likes(post_t *post);

// Se agrega el usuario a la lista de usuarios que likearon el post con el ID indicado.
// Se suma uno a la cantidad de likes si no estaba.
void post_likear(post_t *post, usuario_t* usuario);

// Imprime por stdout la cantidad de likes que tiene el post con el ID indicado y
// los nombres de los usuarios que likearon el post.
void post_likes(post_t *post);

// Destruye un post.
void post_destruir(post_t *post);

// Crea un usuario.
usuario_t *usuario_crear(char *nombre, int id);

// Deuvelve el ID del usuario.
int usuario_id(usuario_t *usuario);

// Devuelve el nombre del usuario.
char *usuario_nombre(usuario_t *usuario);

// Suma un post al feed del usuario.
void usuario_sumar_feed(post_t *post, usuario_t* usuario);

// Resta un post del feed del usuario segun corresponde.
post_t* usuario_restar_feed(usuario_t* usuario);

// Destruye el usuario.
void usuario_destruir(usuario_t *usuario);

void usuario_destruir_envoltorio(void* usuario);

#endif  // ALGOGRAM_H