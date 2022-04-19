#include "pila.h"
#include "testing.h"
#include <stdlib.h>

#define CANTIDAD_DE_DATOS_A_APILAR 500

static void prueba_pila_crear(void) {
    pila_t *pila = pila_crear();
    print_test("La pila fue creada", pila != NULL);
    pila_destruir(pila);
}

static void prueba_pila_vacia(void) {
    pila_t *pila = pila_crear();
    print_test("La pila está vacía", pila_esta_vacia(pila));
    pila_destruir(pila);
}

static void prueba_pila_apilar_desapilar(void) {
    pila_t *pila = pila_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    pila_apilar(pila, valor);
    int* resultado = pila_desapilar(pila);
    pila_destruir(pila);
    print_test("La pila se apila y desapila", resultado == valor);
    free(valor);
}

static void prueba_pila_ver_tope(void) {
    pila_t *pila = pila_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 10;
    char* valor2 = malloc(sizeof(char));
    *valor2 = 'a';
    pila_apilar(pila, valor1);
    pila_apilar(pila, valor2);
    char* valor_tope = pila_ver_tope(pila);
    print_test("Se puede ver el tope", *valor_tope == 'a');
    pila_destruir(pila);
    free(valor1);
    free(valor2);
}

static void prueba_pila_apilar_null(void){
    pila_t *pila = pila_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 10;
    void* null = NULL;
    pila_apilar(pila, valor1);
    pila_apilar(pila, null);
    void* desapilar_null = pila_desapilar(pila);
    print_test("Se puede apilar NULL", desapilar_null == NULL);
    free(valor1);
    pila_destruir(pila);
}

static void prueba_desapilar_y_ver_tope_en_pila_nueva(void){
    pila_t *pila = pila_crear();
    print_test("La pila fue creada correctamente", pila != NULL);
    print_test("No se puede ver tope en pila recien creada", pila_ver_tope(pila) == NULL);
    print_test("No se puede desapilar pila recien creada", pila_desapilar(pila) == NULL);
    pila_destruir(pila);
}

static void prueba_desapilar_y_ver_tope_en_pila_que_se_desapilo(void){
    pila_t *pila = pila_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    pila_apilar(pila, valor);
    int* valor_tope = pila_ver_tope(pila);
    print_test("La pila se apilo correctamente", *valor_tope == *valor);
    int* resultado = pila_desapilar(pila);
    print_test("La pila se desapilo correctamente", *resultado == *valor);
    free(valor);
    print_test("Pila desapilada completamente no se puede desapilar", pila_desapilar(pila) == NULL);
    print_test("Pila desapilada completamente no se puede ver el tope", pila_ver_tope(pila) == NULL);
    pila_destruir(pila);
}

static void prueba_pila_volumen(void){
    pila_t *pila = pila_crear();
    int datos[CANTIDAD_DE_DATOS_A_APILAR];
    int **ptr_datos;
    bool resultados_correctos = true;

    ptr_datos = malloc(CANTIDAD_DE_DATOS_A_APILAR * sizeof(int*));

    for (int i = 0; i < CANTIDAD_DE_DATOS_A_APILAR; ++i){
        datos[i] = i+1;
        ptr_datos[i] = &datos[i];
        pila_apilar(pila, ptr_datos[i]);
        int* valor_tope = pila_ver_tope(pila);
        if (*valor_tope != i+1){
            resultados_correctos = false;
        }
    }

    print_test("Todos los datos se apilaron correctamente", resultados_correctos);

    int* tope = pila_ver_tope(pila);

    print_test("La pila se expandio y se apilaron todos los datos", *tope == CANTIDAD_DE_DATOS_A_APILAR);

    resultados_correctos = true;

    for (int i = CANTIDAD_DE_DATOS_A_APILAR; i > 0; --i){
        int* resultado = pila_desapilar(pila);
        if (*resultado != i){
            resultados_correctos = false;
        }
    }

    print_test("Todos los datos se desapilaron correctamente", resultados_correctos);

    print_test("La pila se desapilo completamente y se comporta como nueva", pila_desapilar(pila) == NULL && pila_ver_tope(pila) == NULL && pila_esta_vacia(pila));

    pila_destruir(pila);
    free(ptr_datos);
}

void pruebas_pila_estudiante() {
    prueba_pila_crear();
    prueba_pila_vacia();
    prueba_pila_apilar_desapilar();
    prueba_pila_ver_tope();
    prueba_pila_apilar_null();
    prueba_desapilar_y_ver_tope_en_pila_nueva();
    prueba_desapilar_y_ver_tope_en_pila_que_se_desapilo();
    prueba_pila_volumen();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
