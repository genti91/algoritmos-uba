#include "hash.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TAM_INICIAL 37
#define EXPANDIR_CAPACIDAD 2
#define REDUCIR_CAPACIDAD 2
#define FACTOR_CARGA_LIM 0.7
#define FACTOR_CARGA_MIN 0.2

enum estado{VACIO, OCUPADO, BORRADO};

typedef struct campo {
    int estado;
    char* clave;
    void* dato;
} campo_t;

struct hash {
    campo_t* arreglo;
    size_t cantidad;
    size_t capacidad;
    size_t cant_borrados;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter {
    size_t pos;
    hash_t* hash;
};

// https://en.wikipedia.org/wiki/Jenkins_hash_function
uint32_t func_hashing(const uint8_t* key, size_t largo_clave, size_t capacidad) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != largo_clave) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash % (uint32_t)capacidad;
}

campo_t *arreglo_crear(size_t tam){
    campo_t* arreglo = malloc(tam*sizeof(campo_t));
    if (arreglo == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < tam; i++){
        arreglo[i].clave = NULL;
        arreglo[i].dato = NULL;
        arreglo[i].estado = VACIO;
    }
    return arreglo;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL) return NULL;
    hash->arreglo = arreglo_crear(TAM_INICIAL);
    if (hash->arreglo == NULL) {
        free(hash);
        return NULL;
    }
    hash->cantidad = 0;
    hash->cant_borrados = 0;
    hash->capacidad = TAM_INICIAL;
    hash->destruir_dato = destruir_dato;
    return hash;
}

size_t buscar_clave_hash(const campo_t *arreglo, size_t pos, const char *clave, size_t capacidad){
    size_t pos_nueva = pos;
    do {
        if (arreglo[pos_nueva].clave != NULL && !strcmp(arreglo[pos_nueva].clave, clave)){
            return pos_nueva;
        }
        if (arreglo[pos_nueva].estado == VACIO) {
            return -1;
        }
        pos_nueva++;
        if (pos_nueva == capacidad) {
            pos_nueva = 0;
        }
    }
    while (pos != pos_nueva);
    return pos_nueva;
}

bool hash_insertar(campo_t *arreglo, const char *clave, void *dato, size_t capacidad, size_t *cantidad, hash_destruir_dato_t destruir_dato) {
    size_t hash_pos = func_hashing((uint8_t*)clave, strlen(clave), capacidad);
    size_t hash_pos_nueva = buscar_clave_hash(arreglo, hash_pos, clave, capacidad);
    if (hash_pos_nueva == -1) {
        while (true) {
            if (arreglo[hash_pos].estado != OCUPADO) {
                char* clave_aux = malloc(sizeof(char)*strlen(clave)+1);
                if (clave_aux == NULL) return false;
                arreglo[hash_pos].clave = clave_aux;
                strcpy(arreglo[hash_pos].clave, clave);
                arreglo[hash_pos].dato = dato;
                arreglo[hash_pos].estado = OCUPADO;
                (*cantidad)++;
                break;
            }
            hash_pos++;
            if (hash_pos == capacidad) {
                hash_pos = 0;
            }
        }
    } else {
        if (destruir_dato != NULL){
            destruir_dato(arreglo[hash_pos_nueva].dato);
        }
        arreglo[hash_pos_nueva].dato = dato;
        arreglo[hash_pos_nueva].estado = OCUPADO;
    }
    return true;
}

campo_t *hash_redimensionar_arreglo(hash_t *hash, size_t tam){
    campo_t* arreglo_nuevo = arreglo_crear(tam);
    if (arreglo_nuevo == NULL) return NULL;
    hash->cantidad = 0;
    hash->cant_borrados = 0;
    for (size_t i = 0; i < hash->capacidad; i++){
        if (hash->arreglo[i].estado == OCUPADO){
            hash_insertar(arreglo_nuevo, hash->arreglo[i].clave, hash->arreglo[i].dato, hash->capacidad * EXPANDIR_CAPACIDAD, &(hash->cantidad), hash->destruir_dato);      
        }
    }
    return arreglo_nuevo;
}

void destruir_arreglo(hash_t *hash){
    for (size_t i = 0; i < hash->capacidad; i++){
        free(hash->arreglo[i].clave);
    }
    free(hash->arreglo);
}

bool factor_de_carga(hash_t *hash, double factor_de_carga){
    if ((hash->cantidad + hash->cant_borrados) / hash->capacidad >= factor_de_carga){
        return true;
    }
    return false;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if (factor_de_carga(hash, FACTOR_CARGA_LIM)) {
        campo_t* arreglo_nuevo = hash_redimensionar_arreglo(hash, hash->capacidad*EXPANDIR_CAPACIDAD);
        if (arreglo_nuevo == NULL) {
            return false;
        }
        destruir_arreglo(hash);
        hash->capacidad = hash->capacidad*EXPANDIR_CAPACIDAD;
        hash->arreglo = arreglo_nuevo;
    }
    hash_insertar(hash->arreglo, clave, dato, hash->capacidad, &(hash->cantidad), hash->destruir_dato);
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if (!hash_pertenece(hash, clave)) return NULL;
    if (factor_de_carga(hash, FACTOR_CARGA_MIN) && hash->capacidad >= TAM_INICIAL) {
        campo_t* arreglo_nuevo = hash_redimensionar_arreglo(hash, hash->cantidad/EXPANDIR_CAPACIDAD);
        if (arreglo_nuevo != NULL) {
            destruir_arreglo(hash);
            hash->capacidad = hash->capacidad/EXPANDIR_CAPACIDAD;
            hash->arreglo = arreglo_nuevo;
        }
    }
    size_t hash_pos = func_hashing((uint8_t*)clave, strlen(clave), hash->capacidad);
    hash_pos = buscar_clave_hash(hash->arreglo, hash_pos, clave, hash->capacidad);
    char* clave_aux = realloc(hash->arreglo[hash_pos].clave, sizeof(char)*2);
    if (clave_aux == NULL) return false;
    void* dato = hash->arreglo[hash_pos].dato;
    hash->arreglo[hash_pos].clave = clave_aux;
    free(hash->arreglo[hash_pos].clave);
    hash->arreglo[hash_pos].clave = NULL;
    hash->arreglo[hash_pos].dato = NULL;
    hash->arreglo[hash_pos].estado = BORRADO;
    hash->cantidad--;
    hash->cant_borrados++;
    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    if (!hash_pertenece(hash, clave)) return NULL;
    size_t hash_pos = func_hashing((uint8_t*)clave, strlen(clave), hash->capacidad);
    return hash->arreglo[buscar_clave_hash(hash->arreglo, hash_pos, clave, hash->capacidad)].dato;
}
    
bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t hash_pos = func_hashing((uint8_t*)clave, strlen(clave), hash->capacidad);
    if (hash->cantidad == 0) {
        return false;
    }
    if (buscar_clave_hash(hash->arreglo, hash_pos, clave, hash->capacidad) != -1) {
        return true;
    }
    return false;
}
    
size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){
    for (size_t i = 0; i < hash->capacidad; i++){
        if (hash->destruir_dato != NULL && hash->arreglo[i].estado == OCUPADO){
            hash->destruir_dato(hash->arreglo[i].dato);
        }
    }
    destruir_arreglo(hash);
    free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    iter->hash = (hash_t*)hash;
    for (size_t i = 0; i < hash->capacidad; i++){
        if (hash->arreglo[i].estado == OCUPADO){
            iter->pos = i;
            return iter;
        }
    }
    iter->pos = -1;
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if (hash_iter_al_final(iter)) {
        return false;
    }
    for (size_t i = iter->pos+1; i < iter->hash->capacidad; i++) {
        if (iter->hash->arreglo[i].estado == OCUPADO){
            iter->pos = i;
            return true;
        }
    }
    iter->pos = -1;
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    return iter->hash->arreglo[iter->pos].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return iter->pos == -1;
}

void hash_iter_destruir(hash_iter_t *iter){
    free(iter);
}