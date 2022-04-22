#include "cola.h"
#include <stdlib.h>

typedef struct nodo nodo_t;

struct nodo {
	void* dato;
	nodo_t* prox;
};

struct cola {
	nodo_t* primero;
	nodo_t* ultimo;
};

cola_t *cola_crear(void){
	cola_t *cola = malloc(sizeof(cola_t));
	if (cola == NULL){
        return NULL;
    }
	cola->primero = NULL;
	cola->ultimo = NULL;
	return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){
	if (cola_esta_vacia(cola)){
		free(cola);
		return;
	}
	while(cola->primero->prox != NULL){
		nodo_t* nodo_prox = cola->primero->prox;
		if (destruir_dato != NULL){
			destruir_dato(cola->primero->dato);
		}
		free(cola->primero);
		cola->primero = nodo_prox;
	}
	if (destruir_dato != NULL){
		destruir_dato(cola->primero->dato);
	}
	free(cola->primero);
	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return cola->primero == NULL;
}

nodo_t* crear_nodo(void *valor){
	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
	if (nodo_nuevo == NULL){
        return NULL;
    }
	nodo_nuevo->dato = valor;
	nodo_nuevo->prox = NULL;
	return nodo_nuevo;
}

bool cola_encolar(cola_t *cola, void *valor){
	nodo_t* nodo_nuevo = crear_nodo(valor);
	if (nodo_nuevo == NULL) {
		return false;
	}
	
	nodo_t* nodo_ultimo_anterior = cola->ultimo;
	cola->ultimo = nodo_nuevo;

	if (cola_esta_vacia(cola)){
		cola->primero = cola->ultimo;
	}else{
		nodo_ultimo_anterior->prox = nodo_nuevo;
	}

	return true;
}

void *cola_ver_primero(const cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	return cola->primero->dato;
}

void *cola_desencolar(cola_t *cola){
	if (cola_esta_vacia(cola)){
		return NULL;
	}
	void* primer_dato = cola->primero->dato;
	nodo_t* nodo_prox = cola->primero->prox;
	if (cola->primero->prox == NULL){
		free(cola->primero);
		cola->primero = NULL;
		cola->ultimo = NULL;
		return primer_dato;
	}
	free(cola->primero);
	cola->primero = nodo_prox;
	return primer_dato;
}