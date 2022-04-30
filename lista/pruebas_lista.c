#include "lista.h"
#include "testing.h"
#include <stdlib.h>

#define CANTIDAD_DE_DATOS_A_ENCOLAR 500

void destruir_datos(void* dato){
    free(dato);
}

static void prueba_lista_crear(void) {
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    lista_destruir(lista, NULL);
}

static void prueba_lista_insertar_primero(void) {
    lista_t* lista = lista_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    print_test("La lista y el valor fueron creados", lista != NULL && valor != NULL);
    lista_insertar_primero(lista, valor);
    int* tope = lista_ver_primero(lista);
    print_test("El tope es correcto", *tope == *valor);
    int* resultado = lista_borrar_primero(lista);
    print_test("La lista se inserta y borra correctamente", resultado == valor);
    lista_destruir(lista, NULL);
    free(valor);
}

static void prueba_lista_vacia(void) {
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    int* valor = malloc(sizeof(int));
    *valor = 5;
    lista_insertar_primero(lista, valor);
    int* tope = lista_ver_primero(lista);
    print_test("El tope es correcto", *tope == *valor);
    int* resultado = lista_borrar_primero(lista);
    print_test("La lista se desencolo correctamente", resultado == valor);
    print_test("La lista está vacía", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    free(valor);
}

static void prueba_lista_ver_primero(void) {
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 10;
    char* valor2 = malloc(sizeof(char));
    *valor2 = 'a';
    lista_insertar_primero(lista, valor2);
    lista_insertar_primero(lista, valor1);
    int* valor_tope = lista_ver_primero(lista);
    print_test("Se puede ver el primero", *valor_tope == 10);
    lista_destruir(lista, destruir_datos);
}

static void prueba_lista_insertar_ultimo(void) {
    lista_t* lista = lista_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 10;
    print_test("La lista y el valor fueron creados", lista != NULL && valor != NULL);
    lista_insertar_primero(lista, valor);
    int* tope = lista_ver_primero(lista);
    print_test("El tope es correcto", *tope == *valor);
    lista_insertar_ultimo(lista, valor2);
    int* ultimo = lista_ver_ultimo(lista);
    print_test("La lista se inserta por el ultimo correctamente", ultimo == valor2);
    lista_destruir(lista, free);
}

static void prueba_lista_insertar_null(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 10;
    void* null = NULL;
    lista_insertar_primero(lista, valor1);
    lista_insertar_primero(lista, null);
    void* desapilar_null = lista_borrar_primero(lista);
    print_test("Se puede insertar NULL", desapilar_null == NULL);
    lista_destruir(lista, free);
}

static void prueba_borrar_y_ver_primero_y_ultimo_en_lista_nueva(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada correctamente", lista != NULL);
    print_test("No se puede ver primero en lista recien creada", lista_ver_primero(lista) == NULL);
    print_test("No se puede ver ultimo en lista recien creada", lista_ver_ultimo(lista) == NULL);
    print_test("No se puede borrar en lista recien creada", lista_borrar_primero(lista) == NULL);
    lista_destruir(lista, NULL);
}

static void prueba_borrar_y_ver_primero_y_ultimo_en_lista_que_se_borro(void){
    lista_t* lista = lista_crear();
    int* valor = malloc(sizeof(int));
    *valor = 5;
    lista_insertar_primero(lista, valor);
    int* valor_tope = lista_ver_primero(lista);
    print_test("La lista se inserto correctamente", *valor_tope == *valor);
    int* resultado = lista_borrar_primero(lista);
    print_test("La lista se borro correctamente", *resultado == *valor);
    free(valor);
    print_test("lista esta vacia despues de borrar", lista_esta_vacia(lista));
    print_test("lista borrada completamente no se puede borrar", lista_borrar_primero(lista) == NULL);
    print_test("lista borrada completamente no se puede ver el primero", lista_ver_primero(lista) == NULL);
    print_test("lista borrada completamente no se puede ver el primero ultimo", lista_ver_ultimo(lista) == NULL);
    lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante() {
    prueba_lista_crear();
    prueba_lista_insertar_primero();
    prueba_lista_vacia();
    prueba_lista_ver_primero();
    prueba_lista_insertar_ultimo();
    prueba_lista_insertar_null();
    prueba_borrar_y_ver_primero_y_ultimo_en_lista_nueva();
    prueba_borrar_y_ver_primero_y_ultimo_en_lista_que_se_borro();
    //prueba_lista_volumen();
    //prueba_destruir_lista_y_no_datos();
    //prueba_destruir_lista_y_datos_con_free();
    //prueba_destruir_lista_y_datos_con_fucion();
    //prueba_destruir_lista_vacia_y_no_datos();
    //prueba_destruir_lista_vacia_y_datos_con_free();
    //prueba_destruir_lista_vacia_y_datos_con_fucion();
    //prueba_general_lista();
    //prueba_iter_crear();
    //prueba_iter_avanzar();
    //prueba_iter_ver_actual();
    //prueba_iter_al_final();
    //prueba_iter_destruir();
    //prueba_iter_insertar_al_principio();
    //prueba_iter_insertar_al_final();
    //prueba_iter_insertar_en_el_medio();
    //prueba_iter_borrar_al_principio();
    //prueba_iter_borrar_al_final();
    //prueba_iter_borrar_en_el_medio();
    //prueba_iter_borrar();
    //prueba_iter_interno_con_corte();
    //prueba_iter_interno_sin_corte();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif