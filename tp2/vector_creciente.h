#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef void (*vectorc_destruir_dato_t)(void *);

typedef struct vectorc {
    size_t tam;
    size_t cant;
    void **datos;
    vectorc_destruir_dato_t destruir_dato;
} vectorc_t;

/*******************************************************************
 *                    PRIMITIVAS DEL VECTOR
 ******************************************************************/

// Crea un vector
// Post: Se creo un vector
vectorc_t *vectorc_crear();

// Destruye el vector
// Pre: el vector fue creado
// Post: se eliminaron todos los elementos del vector
void vectorc_destruir(vectorc_t *vector);

// Almacena en valor el dato guardado en la posición pos del vector
// Pre: el vector fue creado
// Post: se almacenó en valor el dato en la posición pos. Devuelve false si la
// posición es inválida (fuera del rango del vector, que va de 0 a tamaño-1)
void *vectorc_obtener(vectorc_t *vector, size_t pos);

// Almacena el valor al final del vector
// Pre: el vector fue creado
// Post: se almacenó el valor en el final. Devuelve false si no se pudo guardar
// y true si se guardó el valor con éxito.
bool vectorc_guardar_al_final(vectorc_t *vector, void *valor);

// Devuelve el tamaño del vector
// Pre: el vector fue creado
size_t vectorc_cantidad(vectorc_t *vector);

#endif  // _VECTOR_H