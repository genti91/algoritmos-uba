#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

// Primitivas de la lista

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, falso en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al principio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, valor se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve la cantidad de elementos que tiene la lista.
// Pre: la lista fue creada.
// Post: se devolvió la cantidad de elementos de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Primitiva del iterador interno

// Recibe la función visitar y una condición inicial extra por parámetro,
// para cada uno de los elementos de la lista llama a visitar hasta que
// visitar devuelva falso.
// Pre: la lista fue creada. La función visitar recibe por parámetro cada
// elemento iterado, y una condición compartida extra entre las
// iteraciones; si extra es NULL, se recorre la lista entera.
// Pre: la lista fue creada y no está vacía.
// Post: se recorrió la lista llamando a la función visitar hasta que ésta
// devolvió falso.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

// Primitivas del iterador externo

typedef struct lista_iter lista_iter_t;

// Crea un iterador que inicialmente apunta al primer elemento
// de la lista recibida.
// Pre: la lista fue creada, no est vacia.
// Post: devuelve un iterador de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

// El iterador apunta al siguiente elemento, si éste no existe, devuelve
// falso y sino, verdadero.
// Pre: el iterador fue creado.
// Post: devuelve verdadero si el iterador avanzó, falso en el caso contrario.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del elemento al que apunta el iterador actualmente.
// Pre: el iterador fue creado.
// Post: se devolvió el elemento al que apunta el iterador en el momento.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Determina si el iterador está apuntando al último elemento de la lista.
// Pre: el iterador fue creado.
// Post: devuelve verdadero si el elemento al que apunta el iterador es el
// último de la lista, falso en el caso contrario.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: El iterador fue destruido.
void lista_iter_destruir(lista_iter_t *iter);

// Agrega un nuevo elemento en el índice donde apunta el iterador y el
// iterador apunta a este nuevo elemento.
// Pre: el iterador fue creado.
// Post: se agregó un nuevo elemento en el índice donde apunta el iterador.
// El iterador sigue en el mismo índice, apuntando al nuevo elemento.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Saca el elemento del índice donde apunta el iterador y el iterador apunta
// al siguiente elemento.
// Pre: el iterador fue creado.
// Post: se eliminó un nuevo elemento en el índice donde apunta el iterador.
// El iterador sigue en el mismo índice, apuntando al siguiente elemento.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

// Realiza pruebas sobre la implementación realizada.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_lista.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_estudiante(void);

#endif