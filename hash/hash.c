#include "hash.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

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

hash_t *hash_crear_con_tam(hash_destruir_dato_t destruir_dato, size_t tam) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL) return NULL;
    hash->arreglo = malloc(tam*sizeof(campo_t));
    hash->cantidad = 0;
    hash->capacidad = tam;
    hash->cant_borrados = 0;
    hash->destruir_dato = destruir_dato;
    for (int i = 0; i < tam; i++){
        hash->arreglo[i].clave = NULL;
        hash->arreglo[i].dato = NULL;
        hash->arreglo[i].estado = VACIO;
    }
    return hash;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    return hash_crear_con_tam(destruir_dato, TAM_INICIAL);;
}

bool hash_insertar(hash_t *hash, const char *clave, void *dato, size_t hash_pos){
    char* clave_aux = malloc(sizeof(char)*strlen(clave)+1);
    if (clave_aux == NULL) return false;
    if (hash->arreglo[hash_pos].clave != NULL) {
        free(hash->arreglo[hash_pos].clave);
    }
    hash->arreglo[hash_pos].clave = clave_aux;
    strcpy(hash->arreglo[hash_pos].clave, clave);
    if (hash->arreglo[hash_pos].estado != OCUPADO) {
        hash->cantidad++;
    }
    hash->arreglo[hash_pos].dato = dato;
    hash->arreglo[hash_pos].estado = OCUPADO;
    return true;
}

size_t buscar_clave(const hash_t *hash, size_t pos, const char *clave){
    size_t pos_nueva = pos;
    if (hash->cantidad == 0) {
        return -1;
    }
    do {
        if (hash->arreglo[pos_nueva].clave != NULL && !strcmp(hash->arreglo[pos_nueva].clave, clave)){
            return pos_nueva;
        }
        if (hash->arreglo[pos_nueva].estado == VACIO) {
            return -1;
        }
        pos_nueva++;
        if (pos_nueva == hash->capacidad) {
            pos_nueva = 0;
        }
    }
    while (pos != pos_nueva);

    return pos_nueva;
}

void hash_guardar_en_posicion(hash_t *hash, const char *clave, void *dato) {
    size_t hash_pos = func_hashing((uint8_t*)clave, strlen(clave), hash->capacidad);
    size_t hash_pos_nueva = buscar_clave(hash, hash_pos, clave);
    if (hash_pos_nueva == -1) {
        for (int i = 0; i < hash->capacidad; i++) {
            if (hash->arreglo[hash_pos].estado != OCUPADO) {
                hash_insertar(hash, clave, dato, hash_pos);
                break;
            }
            hash_pos++;
            if (hash_pos == hash->capacidad) {
                hash_pos = 0;
            }
        }
    } else {
        if (hash->destruir_dato != NULL){
            hash->destruir_dato(hash->arreglo[hash_pos_nueva].dato);
        }
        hash_insertar(hash, clave, dato, hash_pos_nueva);
    }
}

hash_t *redimensionar(hash_t *hash, size_t tam){
    hash_t* hash_nuevo = hash_crear_con_tam(hash->destruir_dato, tam);
    if (hash_nuevo == NULL) return NULL;
    printf("no falla crear\n");
    //hash->capacidad /= 2;
    printf("capacidad: %ld\n", (size_t)hash->capacidad);
    printf("cantidad: %ld\n", (size_t)hash->cantidad);
    for (size_t i = 0; i < hash->capacidad; i++){
        printf("for: %li\n", i);
        if (hash->arreglo[i].estado == OCUPADO){
            printf("entro al if\n");
            hash_guardar_en_posicion(hash_nuevo, hash->arreglo[i].clave, hash->arreglo[i].dato);
            printf("no falla guardar en %ld posicion\n", i);
        }
    }
    return hash_nuevo;
}

void hash_destruir_aux(hash_t *hash, int red){
    for (size_t i = 0; i < hash->capacidad; i++){
        if (hash->destruir_dato != NULL && hash->arreglo[i].estado == OCUPADO && red == 0){
            hash->destruir_dato(hash->arreglo[i].dato);
        }
        free(hash->arreglo[i].clave);
    }
    free(hash->arreglo);
    free(hash);
}

void hash_destruir(hash_t *hash){
    hash_destruir_aux(hash, 0);
}

bool factor_de_carga(hash_t *hash, double factor_de_carga){
    if ((hash->cantidad + hash->cant_borrados) / hash->capacidad >= factor_de_carga){
        return true;
    }
    return false;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if (factor_de_carga(hash, FACTOR_CARGA_LIM)) {
        hash_t *hash_nuevo = redimensionar(hash, hash->cantidad*EXPANDIR_CAPACIDAD);
        if (hash_nuevo == NULL) {
            return false;
        }
        printf("no falla nada antes de destruir");
        hash_destruir_aux(hash, 1);
        printf("no falla destruir dps de redimension");
        *hash = *hash_nuevo;
    }

    hash_guardar_en_posicion(hash, clave, dato);
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if (!hash_pertenece(hash, clave)) return NULL;
    if (factor_de_carga(hash, FACTOR_CARGA_MIN)) {
        hash_t *hash_nuevo = redimensionar(hash, hash->cantidad/REDUCIR_CAPACIDAD);
        if (hash_nuevo == NULL) {
            return false;
        }
        hash_destruir_aux(hash, 1);
        *hash = *hash_nuevo;
    }
    size_t hash_pos = func_hashing((uint8_t*)clave, strlen(clave), hash->capacidad);
    hash_pos = buscar_clave(hash, hash_pos, clave);
    void* dato = hash->arreglo[hash_pos].dato;
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
    return hash->arreglo[buscar_clave(hash, hash_pos, clave)].dato;
}
    
bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t hash_pos = func_hashing((uint8_t*)clave, strlen(clave), hash->capacidad);
    if (hash->cantidad == 0) {
        return false;
    }
    if (buscar_clave(hash, hash_pos, clave) != -1) {
        return true;
    } else {
        return false;
    }
}
    
size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
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
