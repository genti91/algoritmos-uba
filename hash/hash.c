#include "hash.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define TAM_INICIAL 50
#define EXPANDIR_CAPACIDAD 2

enum estado{VACIO, OCUPADO, BORRADO};

typedef struct campo {
    int estado;
    char* clave;
    void* dato;
} campo_t;

struct hash {
    campo_t** arreglo;
    size_t cantidad;
    size_t capacidad;
    hash_destruir_dato_t destruir_dato;
};

// https://en.wikipedia.org/wiki/Jenkins_hash_function
uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    hash->arreglo = malloc(TAM_INICIAL * sizeof(campo_t));
    hash->cantidad = 0;
    hash->capacidad = TAM_INICIAL;
    hash->destruir_dato = destruir_dato;
    for (int i = 0; i < TAM_INICIAL; i++){
        hash->arreglo[i]->estado = VACIO;
    }
    return hash;
}

void hash_insertar(hash_t *hash, const char *clave, void *dato, size_t hash_pos){
    hash->arreglo[hash_pos]->clave = (char*)clave;
    hash->arreglo[hash_pos]->dato = dato;
    hash->arreglo[hash_pos]->estado = OCUPADO;
    hash->cantidad++;
}

size_t buscar_clave(hash_t *hash, size_t pos, const char *clave){
    size_t pos_nueva = pos;
    while (pos != pos_nueva){
        if (hash->arreglo[pos_nueva]->clave == clave){
            return pos_nueva;
        }
        if (hash->arreglo[pos_nueva]->estado == VACIO) {
            return -1;
        }
        pos_nueva++;
        if (pos_nueva == hash->capacidad) {
            pos_nueva = 0;
        }
    }
    return pos_nueva;
}

bool redimensionar(hash_t *hash){
    hash_t* hash_nuevo = malloc(sizeof(hash_t));
    hash_nuevo->arreglo = malloc((hash->capacidad * EXPANDIR_CAPACIDAD) * sizeof(campo_t));
    if (hash_nuevo->arreglo == NULL) return false;

    hash->capacidad = hash->capacidad*2;
    return true;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    size_t hash_pos = jenkins_one_at_a_time_hash((uint8_t*)clave, strlen(clave)) % hash->capacidad;
    size_t hash_pos_nueva = buscar_clave(hash, hash_pos, clave);
    if (hash_pos_nueva != -1) {
        hash->destruir_dato(hash->arreglo[hash_pos_nueva]->dato);
        hash_insertar(hash, clave, dato, hash_pos);
    } else {
        hash_pos_nueva = hash_pos;
        while (hash_pos_nueva != hash_pos){
            if (hash->arreglo[hash_pos_nueva]->estado != OCUPADO) {
                hash_insertar(hash, clave, dato, hash_pos_nueva);
                break;
            }
            hash_pos_nueva++;
            if (hash_pos_nueva == hash->capacidad) {
                hash_pos_nueva = 0;
            }
        }
    }
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if (!hash_pertenece(hash, clave)) return NULL;
    size_t hash_pos = jenkins_one_at_a_time_hash((uint8_t*)clave, strlen(clave)) % hash->capacidad;
    hash_pos = buscar_clave(hash, hash_pos, clave);
    void* dato = hash->arreglo[hash_pos]->dato;
    hash->arreglo[hash_pos]->estado = BORRADO;
    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    if (!hash_pertenece(hash, clave)) return NULL;
    size_t hash_pos = jenkins_one_at_a_time_hash((uint8_t*)clave, strlen(clave)) % hash->capacidad;
    return hash->arreglo[buscar_clave(hash, hash_pos, clave)]->dato;
}
    
bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t hash_pos = jenkins_one_at_a_time_hash((uint8_t*)clave, strlen(clave)) % hash->capacidad;
    size_t pos_nueva = hash_pos;
    while (hash_pos != pos_nueva){
        if (hash->arreglo[pos_nueva]->clave == clave){
            return true;
        }
        if (hash->arreglo[pos_nueva]->estado == VACIO){
            return false;
        }
        pos_nueva++;
        if (pos_nueva == hash->capacidad) {
            pos_nueva = 0;
        }
    }
    return false;
}
    


size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}



void hash_destruir(hash_t *hash){
    while (hash->cantidad != 0){
        if (hash->destruir_dato != NULL){
            
        }
        
    }
    free(hash);
}


