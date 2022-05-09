#include "lista.h"
#include "testing.h"
#include <stdlib.h>
#include "pila.h"

#define CANTIDAD_DE_DATOS_A_INSERTAR 500

void destruir_pila_envoltorio(void *puntero) {
	pila_destruir(puntero); 
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
    lista_destruir(lista, free);
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

void prueba_lista_volumen(size_t volumen) {
	lista_t *lista = lista_crear();
	print_test("Se creo una lista vacia para prueba de volumen", lista_esta_vacia(lista));
	int arreglo_int[volumen];
	for (int i = 0; i < volumen; i++) {
		arreglo_int[i] = i;
	}
	for (int i = 0; i < volumen; i++) {
		if (!lista_insertar_ultimo(lista, &arreglo_int[i])) {
			print_test("Falló el insertar en la prueba de volumen", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
	for (int i = 0; i < volumen; i++) {
		if (&arreglo_int[i] != lista_ver_primero(lista)) {
			print_test("El primer elemento no es el correspondiente en la prueba de volumen. La prueba se frena", false);
			lista_destruir(lista, NULL);
			return;
		}
		if (&arreglo_int[i] != lista_borrar_primero(lista)) {
			print_test("El elemento borrado no es el correspondiente en la prueba de volumen. La prueba se frena", false);
			lista_destruir(lista, NULL);
			return;
		}
	}
    print_test("Borrar lista vaciada devuelve NULL", lista_borrar_primero(lista) == NULL);
	print_test("La lista esta vacia: se pudo manejar exitosamente el volumen", lista_esta_vacia(lista));
	lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_y_no_datos(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    int* valor = malloc(sizeof(int));
    *valor = 5;
    lista_insertar_primero(lista, valor);
    int* valor_tope = lista_ver_primero(lista);
    print_test("El valor se inserto correctamente", *valor_tope == *valor);
    lista_destruir(lista, NULL);
    print_test("La lista se destruyo y su datos no", *valor == 5);
    free(valor);
}

static void prueba_destruir_lista_y_datos_con_free(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    int* valor = malloc(sizeof(int));
    *valor = 5;
    lista_insertar_primero(lista, valor);
    int* valor_tope = lista_ver_primero(lista);
    print_test("El valor se inserto correctamente", *valor_tope == *valor);
    lista_destruir(lista, free);
}

static void prueba_destruir_lista_y_datos_con_fucion(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
	pila_t *pila1 = pila_crear();
	pila_t *pila2 = pila_crear();
	pila_t *pila3 = pila_crear();
    lista_insertar_primero(lista, pila1);
    print_test("Una pila se inserto correctamente", lista_ver_primero(lista) == pila1);
    lista_insertar_primero(lista, pila2);
    print_test("Una pila se inserto correctamente", lista_ver_primero(lista) == pila2);
    lista_insertar_primero(lista, pila3);
    print_test("Una pila se inserto correctamente", lista_ver_primero(lista) == pila3);
    lista_destruir(lista, destruir_pila_envoltorio);
}

static void prueba_destruir_lista_vacia_y_no_datos(void){
    lista_t* lista = lista_crear();
    lista_destruir(lista, NULL);
}

static void prueba_destruir_lista_vacia_y_datos_con_free(void){
    lista_t* lista = lista_crear();
    lista_destruir(lista, free);
}

static void prueba_destruir_lista_vacia_y_datos_con_fucion(void){
    lista_t* lista = lista_crear();
    lista_destruir(lista, destruir_pila_envoltorio);
}

static void prueba_general_lista(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    char** valor2 = malloc(4*sizeof(char*));
    *valor2 = "dos";
    float* valor3 = malloc(sizeof(float));
    *valor3 = 3.00;
    int* valor4 = malloc(sizeof(int));
    *valor4 = 4;
    int* valor5 = malloc(sizeof(int));
    *valor5 = 5;
    print_test("Se inserto un elemento al principio", lista_insertar_primero(lista, valor2));
    print_test("Se inserto un elemento al final", lista_insertar_ultimo(lista, valor3));
    print_test("Se inserto un elemento al principio", lista_insertar_primero(lista, valor1));
    print_test("Se inserto un elemento al final", lista_insertar_ultimo(lista, valor4));
    print_test("Se borro el primer elemento", lista_borrar_primero(lista) == valor1);
    print_test("Se inserto un elemento al final", lista_insertar_ultimo(lista, valor5));
    print_test("La lista no esta vacia", !lista_esta_vacia(lista));
    print_test("El primer elemento es el correspondiente", lista_ver_primero(lista) == valor2);
    print_test("El ultimo elemento es el correspondiente", lista_ver_ultimo(lista) == valor5);
    free(valor1);
    lista_destruir(lista, free);
}

static void prueba_iter_crear(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_iter_avanzar_y_ver_actual(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 5;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 10;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    print_test("El iterador apunta al primer dato", lista_iter_ver_actual(iter) == valor1);
    print_test("El iterador avanzo correctamente", lista_iter_avanzar(iter));
    print_test("El iterador apunta al proximo dato", lista_iter_ver_actual(iter) == valor2);
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
}

static void prueba_iter_al_final(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 5;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 10;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    print_test("El iterador no apunta al ultimo dato", !lista_iter_al_final(iter));
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("El iterador apunta al ultimo dato", lista_iter_al_final(iter));
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
}

static void prueba_iter_destruir(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

static void prueba_iter_insertar_al_principio(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    print_test("El iterador apunta al primer dato", lista_iter_ver_actual(iter) == valor1);
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    print_test("El iter inserto correctamente", lista_iter_insertar(iter, valor3));
    print_test("El valor se inserto al principio", lista_ver_primero(lista) == valor3);
    print_test("El iter apunta al primer dato", lista_ver_primero(lista) == lista_iter_ver_actual(iter));
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
}

static void prueba_iter_insertar_al_final(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("El iter esta al final", lista_iter_al_final(iter));
    print_test("El iter apunta a NULL", lista_iter_ver_actual(iter) == NULL);
    print_test("El iter inserto correctamente", lista_iter_insertar(iter, valor3));
    print_test("El valor se inserto al final", lista_ver_ultimo(lista) == valor3);
    print_test("El iter apunta al ultimo dato", lista_iter_ver_actual(iter) == valor3);
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
}

static void prueba_iter_insertar_en_el_medio(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    lista_iter_avanzar(iter);
    print_test("El iter esta en la segunda posicion", lista_iter_ver_actual(iter) == valor2);
    print_test("El iter inserto correctamente", lista_iter_insertar(iter, valor3));
    print_test("El valor se inserto en el medio", lista_ver_ultimo(lista) == valor2 && lista_ver_primero(lista) == valor1);
    print_test("El iter apunta al dato del medio", lista_iter_ver_actual(iter) == valor3);
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
}

static void prueba_iter_borrar_al_principio(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_insertar_ultimo(lista, valor3);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    print_test("El iterador apunta al primer dato", lista_iter_ver_actual(iter) == valor1);
    print_test("El iter borro correctamente", lista_iter_borrar(iter) == valor1);
    print_test("Lista ver primero apunta al proximo dato", lista_ver_primero(lista) == valor2);
    print_test("El iter apunta al primer dato", lista_ver_primero(lista) == lista_iter_ver_actual(iter));
    free(valor1);
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
}

static void prueba_iter_borrar_al_final(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_insertar_ultimo(lista, valor3);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("El iter esta en el ultimo elemento", lista_iter_ver_actual(iter) == lista_ver_ultimo(lista));
    print_test("El iter borro el ultimo elemento", lista_iter_borrar(iter) == valor3);
    print_test("Lista ver ultimo devuelve el penultimo dato insertado", lista_ver_ultimo(lista) == valor2);
    print_test("El iter apunta al NULL", lista_iter_ver_actual(iter) == NULL);
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
    free(valor3);
}

static void prueba_iter_borrar_en_el_medio(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_insertar_ultimo(lista, valor3);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    lista_iter_avanzar(iter);
    print_test("El iter esta en el medio posicion", lista_iter_ver_actual(iter) == valor2);
    print_test("El iter borro el elemento correspondiente", lista_iter_borrar(iter) == valor2);
    print_test("El iter apunta al siguiente elemento", lista_iter_ver_actual(iter) == valor3);
    lista_iter_destruir(iter);
    lista_destruir(lista, free);
    free(valor2);
}

static void prueba_iter_borrar_todo(void){
    lista_t* lista = lista_crear();
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    lista_insertar_primero(lista, valor1);
    lista_iter_t *iter = lista_iter_crear(lista);
    print_test("El iterador fue creado", iter != NULL);
    print_test("El iter esta en la primer posicion", lista_iter_ver_actual(iter) == valor1);
    print_test("El iter borro el elemento correspondiente", lista_iter_borrar(iter) == valor1);
    print_test("El iter apunta al NULL", lista_iter_ver_actual(iter) == NULL);
    print_test("El iter borrar devuelve NULL", lista_iter_borrar(iter) == NULL);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    free(valor1);
}

bool buscar_pos_valor3(void *elemento, void *extra) {
    int *contador = extra;
    int *valor = elemento;
    if (*valor == 3){
        return false;
    }
    ++(*contador);
    return true;
}

bool sumar_uno_a_todos(void *elemento, void *extra) {
    ++*((int*)elemento);
    return true;
}

static void prueba_iter_interno_con_corte(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    int* valor4 = malloc(sizeof(int));
    *valor4 = 4;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_insertar_ultimo(lista, valor3);
    lista_insertar_ultimo(lista, valor4);
    print_test("El primer elemento es el correspondiente", lista_ver_primero(lista) == valor1);
    print_test("El ultimo elemento es el correspondiente", lista_ver_ultimo(lista) == valor4);
    int pos_valor3 = 0;
    lista_iterar(lista, buscar_pos_valor3, &pos_valor3);
    print_test("La posicion del valor3 es correcta", pos_valor3 == 2);
    lista_iterar(lista, sumar_uno_a_todos, NULL);
    print_test("Se le sumo uno a todos", *(int*)lista_ver_primero(lista) == 2 && *(int*)lista_ver_ultimo(lista) == 5);
    lista_iterar(lista, sumar_uno_a_todos, NULL);
    print_test("Se le sumo una a todos", *(int*)lista_ver_primero(lista) == 3 && *(int*)lista_ver_ultimo(lista) == 6);
    lista_destruir(lista, free);
}

bool contar_cant_datos(void *elemento, void *extra) {
    int *contador = extra;
    ++(*contador);
    return true;
} 

static void prueba_iter_interno_sin_corte(void){
    lista_t* lista = lista_crear();
    print_test("La lista fue creada", lista != NULL);
    int* valor1 = malloc(sizeof(int));
    *valor1 = 1;
    int* valor2 = malloc(sizeof(int));
    *valor2 = 2;
    int* valor3 = malloc(sizeof(int));
    *valor3 = 3;
    int* valor4 = malloc(sizeof(int));
    *valor4 = 4;
    lista_insertar_primero(lista, valor1);
    lista_insertar_ultimo(lista, valor2);
    lista_insertar_ultimo(lista, valor3);
    lista_insertar_ultimo(lista, valor4);
    print_test("El primer elemento es el correspondiente", lista_ver_primero(lista) == valor1);
    print_test("El ultimo elemento es el correspondiente", lista_ver_ultimo(lista) == valor4);
    int cant_datos = 0;
    lista_iterar(lista, contar_cant_datos, &cant_datos);
    print_test("La cantidad de datos es correcta", cant_datos == 4);
    lista_destruir(lista, free);
}

void prueba_iterador_volumen(size_t volumen) {
	lista_t *lista = lista_crear();
	print_test("Se creo una lista vacia para prueba de volumen", lista_esta_vacia(lista));
	int arreglo_int[volumen];
	for (int i = 0; i < volumen; i++) {
		arreglo_int[i] = i;
	}
    lista_iter_t* iter = lista_iter_crear(lista);
	for (int i = 0; i < volumen; i++) {
		if (!lista_iter_insertar(iter, &arreglo_int[i])) {
			print_test("Falló el insertar en la prueba de volumen", false);
			lista_iter_destruir(iter);
            lista_destruir(lista, NULL);
			return;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    iter = lista_iter_crear(lista);
    for (int i = 0; i < volumen; i++) {
		if (&arreglo_int[i] != lista_iter_ver_actual(iter)) {
			print_test("El primer elemento no es el correspondiente en la prueba de volumen de iterador externo. La prueba se frena", false);
			lista_iter_destruir(iter);
            lista_destruir(lista, NULL);
			return;
		}
		if (&arreglo_int[i] != lista_iter_borrar(iter)) {
			print_test("El elemento borrado no es el correspondiente en la prueba de volumen de iterador externo. La prueba se frena", false);
			lista_iter_destruir(iter);
            lista_destruir(lista, NULL);
			return;
		}
	}
    lista_iter_destruir(iter);
    print_test("Borrar lista vaciada devuelve NULL", lista_borrar_primero(lista) == NULL);
	print_test("La lista esta vacia: se pudo manejar exitosamente el volumen", lista_esta_vacia(lista));
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
    prueba_lista_volumen(CANTIDAD_DE_DATOS_A_INSERTAR);
    prueba_destruir_lista_y_no_datos();
    prueba_destruir_lista_y_datos_con_free();
    prueba_destruir_lista_y_datos_con_fucion();
    prueba_destruir_lista_vacia_y_no_datos();
    prueba_destruir_lista_vacia_y_datos_con_free();
    prueba_destruir_lista_vacia_y_datos_con_fucion();
    prueba_general_lista();
    prueba_iter_crear();
    prueba_iter_avanzar_y_ver_actual();
    prueba_iter_al_final();
    prueba_iter_destruir();
    prueba_iter_insertar_al_principio();
    prueba_iter_insertar_al_final();
    prueba_iter_insertar_en_el_medio();
    prueba_iter_borrar_al_principio();
    prueba_iter_borrar_al_final();
    prueba_iter_borrar_en_el_medio();
    prueba_iter_interno_con_corte();
    prueba_iter_interno_sin_corte();
    prueba_iter_borrar_todo();
    prueba_iterador_volumen(CANTIDAD_DE_DATOS_A_INSERTAR);
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