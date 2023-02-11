#include "postusuario.h"

void usuario_destruir_envoltorio(void* usuario) {
    usuario_destruir(usuario);
}

post_t* post_crear(usuario_t* usuario, char *texto, int id){
    post_t* post = malloc(sizeof(post_t));
    post->id = id;
    post->usuario = usuario;
    post->contenido = texto;
    post->usuarios_likes = abb_crear(strcmp, NULL);
    return post;
}

void post_destruir(post_t *post) {
    free(post->contenido);
    abb_destruir(post->usuarios_likes);
    free(post);
}

int post_id(post_t *post) {
    return post->id;
}

char *post_usuario(post_t *post) {
    return post->usuario->nombre;
}

char *post_contenido(post_t *post) {
    return post->contenido;
}

int post_cant_likes(post_t *post) {
    return (int)abb_cantidad(post->usuarios_likes);
}

void _post_destruir(void* post) {
    post_destruir(post);
}

void usuario_destruir(usuario_t* usuario){
    free(usuario->nombre);
    vectorc_destruir(usuario->ids_posts);
    heap_destruir(usuario->feed, NULL);
    free(usuario);
}

void post_likear(post_t *post, usuario_t* usuario){
    if (!abb_pertenece(post->usuarios_likes, usuario->nombre)) {
        abb_guardar(post->usuarios_likes, usuario->nombre, NULL);
    }
}

void post_likes(post_t *post){
    if (post == NULL || post_cant_likes(post) == 0){
        fputs("Error: Post inexistente o sin likes\n", stdout);
        return;
    }
    fprintf(stdout, "El post tiene %d likes:\n", post_cant_likes(post));
    abb_iter_t *iter = abb_iter_in_crear(post->usuarios_likes);
    while (!abb_iter_in_al_final(iter)) {
        fprintf(stdout, "	%s\n", abb_iter_in_ver_actual(iter));
        abb_iter_in_avanzar(iter);
    }
    abb_iter_in_destruir(iter);
    return;
}

// Devuelve 1 si el el primero es mayor, -1 si el segundo es mayor.
int prioridad_feed(const post_t *post1, const post_t *post2, int id3) {
	int distancia_id1 = abs((post1->usuario->id_usuario)-id3);
    int distancia_id2 = abs((post2->usuario->id_usuario)-id3);
    if (distancia_id1 == distancia_id2) {
        if (post1->id < post2->id) {
            return 1;
        } else {
            return -1;
        }
    }
    if (distancia_id1 < distancia_id2) {
        return 1;
    }
    return -1;
}

int prioridad_envoltorio(const void *post1, const void *post2, int id3) {
	return prioridad_feed(post1, post2, id3);
}

usuario_t *usuario_crear(char *nombre, int id){
    usuario_t* usuario = malloc(sizeof(usuario_t));
    if (usuario == NULL) return NULL;
    usuario->id_usuario = id;
	usuario->nombre = malloc(sizeof(char)*strlen(nombre)+1);
    if (usuario->nombre == NULL) return NULL;
	strcpy(usuario->nombre, nombre);
	usuario->feed = heap_crear(prioridad_envoltorio, id);
	if (usuario->feed == NULL){
		free(usuario);
		return NULL;
	}
    usuario->ids_posts = vectorc_crear();
	if (usuario->ids_posts == NULL){
		free(usuario->feed);
		free(usuario);
		return NULL;
	}
	return usuario;
}

char *usuario_nombre(usuario_t *usuario) {
    return usuario->nombre;
}

void usuario_sumar_feed(post_t *post, usuario_t* usuario) {
    heap_encolar(usuario->feed, post);
}

post_t *usuario_restar_feed(usuario_t* usuario) {
    return heap_desencolar(usuario->feed);
}
