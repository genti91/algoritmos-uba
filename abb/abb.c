#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct nodo nodo_t;

struct nodo {
    void* clave;
	void* dato;
	nodo_t* izq;
    nodo_t* der;
};

struct abb {
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

struct abb_iter {
    pila_t *pila;
};


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;
    abb->cantidad = 0;
    abb->raiz = NULL;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;    
    return abb;
}

// nunca recibe un NULL como nodo. devuelve el puntero al hijo que contiene el padre del elemento a buscar.
// si no existe el elemento, devuelve el puntero del padre del NULL donde debería ubicarse el elemento.
nodo_t **buscar_clave(nodo_t **nodo, const char *clave, abb_comparar_clave_t cmp) {
    int mayor = cmp(clave, (*nodo)->clave);
    if (mayor == 0) return nodo;
    if (mayor > 0) {
        if ((*nodo)->der == NULL) {
            return &((*nodo)->der);
        }
        return buscar_clave(&((*nodo)->der), clave, cmp);
    }
    if (mayor < 0) {
        if ((*nodo)->izq == NULL) {
            return &((*nodo)->izq);
        }
        return buscar_clave(&((*nodo)->izq), clave, cmp);
    }
    return NULL;
}

nodo_t* crear_nodo(void *valor, char *clave){
	nodo_t* nodo_nuevo = malloc(sizeof(nodo_t));
	if (nodo_nuevo == NULL){
        return NULL;
    }
    nodo_nuevo->clave = malloc(sizeof(char)*strlen(clave)+1);
    strcpy(nodo_nuevo->clave, clave);
	nodo_nuevo->dato = valor;
	nodo_nuevo->izq = NULL;
    nodo_nuevo->der = NULL;
	return nodo_nuevo;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
    nodo_t* nodo_nuevo = crear_nodo(dato, (char*)clave);
    if (nodo_nuevo == NULL) return false;
    if (!arbol->raiz) {
        arbol->raiz = nodo_nuevo;
        arbol->cantidad++;
        return true;
    }
    nodo_t **posicion = buscar_clave(&(arbol->raiz), clave, arbol->cmp);
    
    if ((*posicion) == NULL){
        arbol->cantidad++;
        *posicion = nodo_nuevo;
    } else {
        if (arbol->destruir_dato != NULL) {
            arbol->destruir_dato((*posicion)->dato);
        }
        (*posicion)->dato = dato;
        free(nodo_nuevo->clave);
        free(nodo_nuevo);
    }
    return true;
}

nodo_t **buscar_reemplazante_izq(nodo_t **nodo) {
    if ((*nodo)->der == NULL) return nodo;
    return buscar_reemplazante_izq(&((*nodo)->der));
}

void *abb_borrar(abb_t *arbol, const char *clave){
    if (arbol->raiz == NULL) return NULL;
    nodo_t **borrar = buscar_clave(&(arbol->raiz), clave, arbol->cmp);
    if (*borrar == NULL) return NULL;
    void *dato = (*borrar)->dato;
    nodo_t *nodo_a_liberar = *borrar;
    if ((*borrar)->izq == NULL && (*borrar)->der == NULL){
        free((*borrar)->clave);
        *borrar = NULL;
    } else if ((*borrar)->izq == NULL || (*borrar)->der == NULL){
        free((*borrar)->clave);
        if ((*borrar)->izq == NULL){
            (*borrar) = (*borrar)->der;
        } else {
            (*borrar) = (*borrar)->izq;
        }
    } else if ((*borrar)->izq != NULL && (*borrar)->der != NULL) {
        nodo_t **reemplazante = buscar_reemplazante_izq(&((*borrar)->izq));
        free((*borrar)->clave);
        (*borrar)->clave = (*reemplazante)->clave;
        (*borrar)->dato = (*reemplazante)->dato;
        nodo_a_liberar = (*reemplazante);
        (*reemplazante) = (*reemplazante)->izq;
    }
    free(nodo_a_liberar);
    arbol->cantidad--;
    return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
    if (arbol->raiz == NULL) return NULL;
    nodo_t **obtener = buscar_clave(&(((abb_t*)arbol)->raiz), (char*)clave, arbol->cmp);
    if (*obtener == NULL) return NULL;
    return (*obtener)->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
    if (arbol->raiz == NULL) return false;
    return (*(buscar_clave(&(((abb_t*)arbol)->raiz), clave, arbol->cmp)) != NULL);
}

size_t abb_cantidad(const abb_t *arbol){
    return arbol->cantidad;
}

void destruir_nodo_hijos(nodo_t* nodo, abb_destruir_dato_t destruir_dato) {
    if (!nodo) return;
    destruir_nodo_hijos(nodo->izq, destruir_dato);
    destruir_nodo_hijos(nodo->der, destruir_dato);
    if (destruir_dato != NULL){
        destruir_dato(nodo->dato);
    }
    free(nodo->clave);    
    free(nodo);
}

void abb_destruir(abb_t *arbol){
    destruir_nodo_hijos(arbol->raiz, arbol->destruir_dato);
    free(arbol); 
}

bool nodos_in_order(nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra) {
    if (!nodo) return true;
    if (nodos_in_order(nodo->izq, visitar, extra)) {
        if (!visitar(nodo->clave, nodo->dato, extra)) {
            return false;
        } else {
            return nodos_in_order(nodo->der, visitar, extra);
        }
    }
    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    nodos_in_order(arbol->raiz, visitar, extra);
}

void apilar_todo_izquierda(nodo_t *nodo, pila_t *pila){
    if (!nodo) return;
    pila_apilar(pila, nodo);
    apilar_todo_izquierda(nodo->izq, pila);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    pila_t* pila = pila_crear();
    iter->pila = pila;
    apilar_todo_izquierda(arbol->raiz, iter->pila);
    return iter;
} 

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if (abb_iter_in_al_final(iter)) return false;
    apilar_todo_izquierda((((nodo_t*)pila_desapilar(iter->pila))->der), iter->pila);
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    if (pila_esta_vacia(iter->pila)) return NULL;
    nodo_t *nodo = pila_ver_tope(iter->pila);
    return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}
