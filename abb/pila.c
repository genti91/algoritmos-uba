#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INICIAL 50
#define EXPANDIR_CAPACIDAD 2
#define DIVIDIR_CAPACIDAD 2
#define MIN_REDUCCION 4

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void){
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) return NULL;
    pila->cantidad = 0;
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->datos = malloc(pila->capacidad * sizeof(void*));
    if (pila->datos == NULL){
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila){
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return pila->cantidad == 0;
}

bool pila_redimensionar(pila_t *pila, size_t tam_nuevo) {
    void **capacidad_nueva = realloc(pila->datos, tam_nuevo * sizeof(void*));
    if (capacidad_nueva == NULL) return false;
    pila->datos = capacidad_nueva;
    pila->capacidad = tam_nuevo;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor){
    if (pila->cantidad == pila->capacidad) {
        if (!pila_redimensionar(pila, pila->capacidad * EXPANDIR_CAPACIDAD)){
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila){
    if (pila_esta_vacia(pila)) return NULL;
    return pila->datos[pila->cantidad - 1];
}

void *pila_desapilar(pila_t *pila){
    if (pila_esta_vacia(pila)) return NULL;
    pila->cantidad--;
    if (pila->capacidad/MIN_REDUCCION >= pila->cantidad && 
        pila->capacidad/DIVIDIR_CAPACIDAD >= CAPACIDAD_INICIAL){
        pila_redimensionar(pila, pila->capacidad/DIVIDIR_CAPACIDAD);
    }
    return pila->datos[pila->cantidad];
}