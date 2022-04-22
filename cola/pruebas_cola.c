#include "cola.h"
#include "testing.h"
#include <stdlib.h>

#define CANTIDAD_DE_DATOS_A_ENCOLAR 500

void destruir_datos(void* dato){
    free(dato);
}

static void prueba_cola_crear(void) {
    cola_t* cola = cola_crear();
    print_test("La cola fue creada", cola != NULL);
    cola_destruir(cola, NULL);
}

static void prueba_cola_vacia(void) {
    cola_t* cola = cola_crear();
    print_test("La cola fue creada", cola != NULL);
    int* valor = malloc(sizeof(int));
    *valor = 5;
    cola_encolar(cola, valor);
    int* tope = cola_ver_primero(cola);
    print_test("El tope es correcto", *tope == *valor);
    int* resultado = cola_desencolar(cola);
    print_test("La cola se desencolo correctamente", resultado == valor);
    print_test("La cola está vacía", cola_esta_vacia(cola));
    cola_destruir(cola, NULL);
    free(valor);
}

static void prueba_cola_apilar_desapilar(void) {
    cola_t* cola = cola_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    print_test("La cola y el valor fueron creados", cola != NULL && valor != NULL);
    cola_encolar(cola, valor);
    int* tope = cola_ver_primero(cola);
    print_test("El tope es correcto", *tope == *valor);
    int* resultado = cola_desencolar(cola);
    print_test("La cola se encola y desencola correctamente", resultado == valor);
    free(valor);
    cola_destruir(cola, NULL);
}

static void prueba_cola_ver_tope(void) {
    cola_t* cola = cola_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 10;
    char* valor2 = malloc(sizeof(char));
    *valor2 = 'a';
    cola_encolar(cola, valor1);
    cola_encolar(cola, valor2);
    int* valor_tope = cola_ver_primero(cola);
    print_test("Se puede ver el tope", *valor_tope == 10);
    cola_destruir(cola, destruir_datos);
}

static void prueba_cola_apilar_null(void){
    cola_t* cola = cola_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 10;
    void* null = NULL;
    cola_encolar(cola, null);
    cola_encolar(cola, valor1);
    void* desapilar_null = cola_desencolar(cola);
    print_test("Se puede encolar NULL", desapilar_null == NULL);
    cola_destruir(cola, destruir_datos);
}

static void prueba_desapilar_y_ver_primero_en_cola_nueva(void){
    cola_t* cola = cola_crear();
    print_test("La cola fue creada correctamente", cola != NULL);
    print_test("No se puede ver primero en cola recien creada", cola_ver_primero(cola) == NULL);
    print_test("No se puede descolar cola recien creada", cola_desencolar(cola) == NULL);
    cola_destruir(cola, NULL);
}

static void prueba_desapilar_y_ver_tope_en_pila_que_se_desapilo(void){
    cola_t* cola = cola_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    cola_encolar(cola, valor);
    int* valor_tope = cola_ver_primero(cola);
    print_test("La cola se encolo correctamente", *valor_tope == *valor);
    int* resultado = cola_desencolar(cola);
    print_test("La cola se descolo correctamente", *resultado == *valor);
    free(valor);
    print_test("Cola esta vacia despues de desapilar", cola_esta_vacia(cola));
    print_test("Cola desencolada completamente no se puede desencolar", cola_desencolar(cola) == NULL);
    print_test("Cola desencolada completamente no se puede ver el primero", cola_ver_primero(cola) == NULL);
    cola_destruir(cola, NULL);
}

static void prueba_cola_volumen(void){
    cola_t* cola = cola_crear();
    int datos[CANTIDAD_DE_DATOS_A_ENCOLAR];
    bool resultados_correctos = true;

    for (int i = 0; i < CANTIDAD_DE_DATOS_A_ENCOLAR; ++i){
        datos[i] = i+1;
        cola_encolar(cola, &datos[i]);
        int* dato = cola_desencolar(cola);
        if (*dato != i+1){
            resultados_correctos = false;
        }
    }

    print_test("Todos los datos se encolaron correctamente", resultados_correctos);

    print_test("La cola se desencolo completamente y se comporta como nueva", cola_desencolar(cola) == NULL && cola_ver_primero(cola) == NULL && cola_esta_vacia(cola));

    cola_destruir(cola, NULL);
}

static void prueba_destruir_cola_y_no_datos(void){
    cola_t* cola = cola_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    cola_encolar(cola, valor);
    int* valor_tope = cola_ver_primero(cola);
    print_test("La cola se encolo correctamente", *valor_tope == *valor);
    cola_destruir(cola, NULL);
    print_test("La cola se destruyo y su datos no", *valor == 5);
    free(valor);
}


void pruebas_cola_estudiante() {
    prueba_cola_crear();
    prueba_cola_vacia();
    prueba_cola_apilar_desapilar();
    prueba_cola_ver_tope();
    prueba_cola_apilar_null();
    prueba_desapilar_y_ver_primero_en_cola_nueva();
    prueba_desapilar_y_ver_tope_en_pila_que_se_desapilo();
    prueba_cola_volumen();
    prueba_destruir_cola_y_no_datos();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif