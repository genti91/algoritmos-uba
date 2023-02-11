#include "tp2.h"
#define TAM_VECTOR 10


bool login(hash_t *usuarios, estado_t *estado){
    if (estado->logueado){
        fputs("Error: Ya habia un usuario loggeado\n", stdout);
        return false;
    }
    char* usuario = NULL;
    size_t tam = 0;
    size_t largo = getline(&usuario, &tam, stdin);
    usuario[largo-1] = '\0';
    bool pertenece = hash_pertenece(usuarios, usuario);
    if(pertenece && !estado->logueado){
        fprintf(stdout, "Hola %s\n", usuario);
        estado->logueado = true;
        estado->usuario = hash_obtener(usuarios, usuario);
        free(usuario);
        return true;
    }
    free(usuario);
    if (!pertenece){
        fputs("Error: usuario no existente\n", stdout);
    }
    return false;
}

bool logout(estado_t *estado){
    if (estado->logueado){
        fputs("Adios\n", stdout);
        estado->logueado = false;
        estado->usuario = NULL;
        return true;
    }else{
        fputs("Error: no habia usuario loggeado\n", stdout);
    }
    return false;
}

bool publicar(hash_t *usuarios, estado_t estado, vectorc_t *posts){
    if (!estado.logueado){
        fputs("Error: no habia usuario loggeado\n", stdout);
        return false;
    }
    char* texto = NULL;
    size_t tam = 0;
    size_t largo = getline(&texto, &tam, stdin);
    char* publicacion = malloc(sizeof(char)*(largo)+1);
    strcpy(publicacion, texto);
    publicacion[largo-1] = '\0';
    free(texto);
    post_t* post = post_crear(estado.usuario, publicacion, (int)vectorc_cantidad(posts));
    vectorc_guardar_al_final(posts, post);
    hash_iter_t *iter = hash_iter_crear(usuarios);
    while (!hash_iter_al_final(iter)){
        usuario_t *actual = hash_obtener(usuarios, hash_iter_ver_actual(iter));
        if(strcmp(usuario_nombre(actual), usuario_nombre(estado.usuario))){
            usuario_sumar_feed(post, actual);
        }
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    fputs("Post publicado\n", stdout);
    return true;
}

// mandamos post como NULL desde main si no existe
bool likear_post(vectorc_t *posts, estado_t estado) {
    int id_post;
    if(scanf("%i", &id_post)){}
    post_t *post = vectorc_obtener(posts, id_post);
    if (!estado.logueado || post == NULL) {
        fputs("Error: Usuario no loggeado o Post inexistente\n", stdout);
        return false;
    }
    post_likear(post, estado.usuario);
    fputs("Post likeado\n", stdout);
    return true;
}

post_t *ver_siguiente_feed(estado_t estado){
    if (!estado.logueado){
        fputs("Usuario no loggeado o no hay mas posts para ver\n", stdout);
        return NULL;
    }
    post_t *post = usuario_restar_feed(estado.usuario);
    if (post == NULL) {
        fputs("Usuario no loggeado o no hay mas posts para ver\n", stdout);
        return NULL;
    }
    fprintf(stdout, "Post ID %d\n", post_id(post));
    fprintf(stdout, "%s dijo: %s\n", post_usuario(post), post_contenido(post));
    fprintf(stdout, "Likes: %d\n", post_cant_likes(post));
    return post;
}

void mostrar_likes(vectorc_t *posts){
    int id_post;
    if(scanf("%i", &id_post)){}
    post_t *post = vectorc_obtener(posts, id_post);
    post_likes(post);
}