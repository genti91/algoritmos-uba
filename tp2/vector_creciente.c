#include "vector_creciente.h"
#include <stdio.h>
#define TAM_INICIAL 13
#define CONST_REDIM 2

vectorc_t *vectorc_crear(vectorc_destruir_dato_t destruir_dato) {
    vectorc_t *vector = malloc(sizeof(vectorc_t));
    if (vector == NULL) {
        return NULL;
    }
    vector->datos = malloc(TAM_INICIAL * sizeof(void*));
    if (vector->datos == NULL) {
        free(vector);
        return NULL;
    }
    vector->tam = TAM_INICIAL;
    vector->cant = 0;
    vector->destruir_dato = destruir_dato;
    return vector;
}

void vectorc_destruir(vectorc_t *vector) {
    if (vector->destruir_dato != NULL) {
        for (size_t i = 0; i < vector->cant; i++){   
            vector->destruir_dato(vector->datos[i]);
        }
    }
    free(vector->datos);
    free(vector);
}

void *vectorc_obtener(vectorc_t *vector, size_t pos) { 
    if (pos >= vector->cant || pos < 0) return NULL;
    return vector->datos[pos];
}

bool vectorc_guardar_al_final(vectorc_t *vector, void *valor) {
    size_t pos = vector->cant;
    if (pos >= vector->tam) {
        void **datos_nuevo = realloc(vector->datos, CONST_REDIM*vector->tam*sizeof(void*));
        if (datos_nuevo == NULL) return false;
/*        for (size_t i = 0; vector->cant; i++){
            datos_nuevo[i] = vector->datos[i];
        }
*/
        vector->datos = datos_nuevo;
        vector->tam = CONST_REDIM*vector->tam;
    }
    vector->datos[pos] = valor;
    vector->cant++;
    return true;
}

size_t vectorc_cantidad(vectorc_t *vector) {
    return vector->cant;
}
