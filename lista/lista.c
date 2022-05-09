#include "lista.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct nodo nodo_t;

struct nodo {
	void* dato;
	nodo_t* prox;
};

struct lista {
	nodo_t* primero;
	nodo_t* ultimo;
    size_t largo;
};

struct lista_iter {
	nodo_t* actual;
	nodo_t* anterior;
	lista_t* lista;
};

nodo_t* crear_nodo(void *valor){
	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
	if (nodo_nuevo == NULL){
        return NULL;
    }
	nodo_nuevo->dato = valor;
	nodo_nuevo->prox = NULL;
	return nodo_nuevo;
}

lista_t *lista_crear(void){
	lista_t *lista = malloc(sizeof(lista_t));
	if(lista == NULL){
		return NULL;
	}
	lista->primero = NULL;
	lista->ultimo =NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return (lista->largo == 0 && lista->primero == NULL && lista->ultimo == NULL);
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nuevo = crear_nodo(dato);
    if (nuevo == NULL) {
        return false;
    }
    if (lista_esta_vacia(lista)) {
        lista->ultimo = nuevo;
    } else {
        nuevo->prox = lista->primero;
    }
    lista->primero = nuevo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nodo_nuevo = crear_nodo(dato);
	if (nodo_nuevo == NULL){
		return false;
	}
	if (lista_esta_vacia(lista)){
		lista->primero = nodo_nuevo;
	}else{
		lista->ultimo->prox = nodo_nuevo;
	}
	lista->ultimo = nodo_nuevo;
	lista->largo++;
	return true;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
	while(!lista_esta_vacia(lista)){
		if (destruir_dato != NULL){
			destruir_dato(lista->primero->dato);
		}
		lista_borrar_primero(lista);
	}
	free(lista);
}

void *lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)) return NULL;
	void* primer_dato = lista->primero->dato;
	nodo_t* nodo_prox = lista->primero->prox;
	free(lista->primero);
	lista->largo--;
	if (nodo_prox == NULL){
		lista->primero = NULL;
		lista->ultimo = NULL;
		return primer_dato;
	}
	lista->primero = nodo_prox;
	return primer_dato;
}

void* lista_ver_primero(const lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;
    return lista->primero->dato;
}

void* lista_ver_ultimo(const lista_t *lista){
    if (lista_esta_vacia(lista)) return NULL;
    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t* nodo_aux = lista->primero;
	for (int i = 0; i < lista->largo; i++) {
		if (!visitar(nodo_aux->dato, extra)) return;
		nodo_aux = nodo_aux->prox;
	}
}

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (iter == NULL){
		return NULL;
	}
	iter->actual = lista->primero;
	iter->anterior = NULL;
	iter->lista = lista;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if (lista_iter_al_final(iter)) return false;	
	nodo_t* nodo_aux = iter->actual;
	iter->actual = nodo_aux->prox;
	iter->anterior = nodo_aux;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
	if (iter->actual == NULL) {
		return NULL;
	}
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return (iter->actual == NULL);
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	nodo_t* nodo_nuevo = crear_nodo(dato);
	if (nodo_nuevo == NULL) return false;
	if (iter->actual == iter->lista->primero) {
		iter->lista->primero = nodo_nuevo;
	} else {
		iter->anterior->prox = nodo_nuevo;
	}
	if (lista_iter_al_final(iter)) {
		iter->lista->ultimo = nodo_nuevo;
	}
	nodo_nuevo->prox = iter->actual;
	iter->actual = nodo_nuevo;
	iter->lista->largo++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
	nodo_t* nodo_aux = iter->actual;
	if (lista_iter_al_final(iter)) {
		return NULL;
	}
	if (iter->actual == iter->lista->primero) {
		iter->lista->primero = iter->actual->prox;
	}
	if (iter->actual == iter->lista->ultimo) {
		iter->lista->ultimo = iter->anterior;
	}
	iter->actual = nodo_aux->prox;
	if (iter->actual != iter->lista->primero){
		iter->anterior->prox = nodo_aux->prox;
	}
	void* dato = nodo_aux->dato;
	free(nodo_aux);
	iter->lista->largo--;
	return dato;
}