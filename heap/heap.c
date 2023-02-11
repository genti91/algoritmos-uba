#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL 20
#define EXPANDIR_CAPACIDAD 2
#define DIVIDIR_CAPACIDAD 2
#define MIN_REDUCCION 4

struct heap {
    void ** arreglo;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};

void swap(void **arreglo, size_t x, size_t y) {
    void *aux = arreglo[x];
    arreglo[x] = arreglo[y];
    arreglo[y] = aux;
}

// devuelve true si se intercambiaron posiciones, falso si no paso nada.
// se actualiza el indice segun el lugar segun corresponde.
bool upheap(void **arreglo, size_t *indice, cmp_func_t cmp) {
    if (*indice == 0) return false;
    size_t indice_padre = (*indice-1)/2;
    if ((cmp(arreglo[*indice], arreglo[indice_padre]) > 0)) {
        swap(arreglo, *indice, indice_padre);
        *indice = indice_padre;
        return true;
    }
    return false;
}

// devuelve true si se intercambiaron posiciones, falso si no paso nada.
// se actualiza el valor de la variable indice segun corresponde.
bool downheap(void **arreglo, size_t tam, size_t* indice, cmp_func_t cmp) {
    size_t indice_izq = (*indice)*2+1;
    if (indice_izq > tam-1) return false;
    bool menor_que_izq = (cmp(arreglo[*indice], arreglo[indice_izq]) < 0);
    //if (menor_que_izq) printf("Es menor que el izq\n");
    if (indice_izq == tam-1) {
        if (menor_que_izq) {
            swap(arreglo, *indice, indice_izq);
            *indice = indice_izq;
            return true;
        }
        return false;
    }
    size_t indice_der = (*indice)*2+2;
    bool menor_que_der = (cmp(arreglo[*indice], arreglo[indice_der]) < 0);
    if (menor_que_izq && menor_que_der) {
        if (cmp(arreglo[indice_izq], arreglo[indice_der]) < 0) {
            swap(arreglo, *indice, indice_der);
            *indice = indice_der;
        } else {
            swap(arreglo, *indice, indice_izq);
            *indice = indice_izq;
        }
        return true;
    }
    if (menor_que_izq) {
        swap(arreglo, *indice, indice_izq);
        *indice = indice_izq;
        return true;
    }
    if (menor_que_der) {
        swap(arreglo, *indice, indice_der);
        *indice = indice_der;
        return true;
    }
    return false;
}

void heapify(void *elementos[], size_t cant, cmp_func_t cmp){
    for (size_t i = 0; i < cant; i++) {
        size_t indice = cant-i-1;
        //printf("%lda iteracion, indice: %ld\n", i+1, indice);
        while (downheap(elementos, cant, &indice, cmp));
    }
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cant, cmp);
    for (size_t i = cant; i > 0; i--) {
        /*
        printf("preswap: ");
        for (size_t j = 0; j < cant; j++) {
            printf("%d ", *(int*)elementos[j]);
        }
        printf("\n");
        */
        swap(elementos, 0, i-1);
        /*
        printf("postswap: ");
        for (size_t j = 0; j < cant; j++) {
            printf("%d ", *(int*)elementos[j]);
        }
        printf("\n");
        */
        size_t indice = 0;
        if (i <= 1) break;
        while (downheap(elementos, i-1, &indice, cmp));
    }
}

heap_t *heap_crear(cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (heap == NULL) return NULL;
    heap->arreglo = malloc(CAPACIDAD_INICIAL * sizeof(void*));
    if (!heap->arreglo){
        free(heap);
        return NULL;
    }
    heap->cantidad = 0;
    heap->capacidad = CAPACIDAD_INICIAL;
    heap->cmp = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
    heap_t* heap = malloc(sizeof(heap_t));
    if (heap == NULL) return NULL;
    heap->cmp = cmp;
    heap->cantidad = n;
    heap->capacidad = n;
    heap->arreglo = malloc(heap->capacidad * sizeof(void*));
    if (!heap->arreglo) {
        free(heap);
        return NULL;
    }
    for (size_t i = 0; i < n; i++) {
        heap->arreglo[i] = arreglo[i];
    }
    heapify(heap->arreglo, n, cmp);
    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    if (destruir_elemento) {
        for (size_t i = 0; i < heap->cantidad; i++){
        destruir_elemento(heap->arreglo[i]);
        }
    }
    free(heap->arreglo);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap){
    return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
    return !heap->cantidad;
}

bool arreglo_redimensionar(heap_t *heap, size_t tam_nuevo) {
    void **arreglo_nuevo = realloc(heap->arreglo, tam_nuevo * sizeof(void*));
    if (!arreglo_nuevo) return false;
    heap->arreglo = arreglo_nuevo;
    heap->capacidad = tam_nuevo;
    return true;
}

bool heap_encolar(heap_t *heap, void *elem){
    if (heap->cantidad == heap->capacidad) {
        if (!arreglo_redimensionar(heap, heap->capacidad * EXPANDIR_CAPACIDAD)){
            return false;
        }
    }
    heap->arreglo[heap->cantidad] = elem;
    heap->cantidad++;
    size_t indice = heap->cantidad-1;
    while(upheap(heap->arreglo, &indice, heap->cmp));
    return true;
}

void *heap_ver_max(const heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    return heap->arreglo[0];
}

void *heap_desencolar(heap_t *heap){
    if (heap_esta_vacio(heap)) return NULL;
    void* dato = heap->arreglo[0];
    heap->arreglo[0] = heap->arreglo[heap->cantidad-1];
    heap->cantidad--;
    if (heap->cantidad > 1) {
        size_t indice = 0;
        while(downheap(heap->arreglo, heap->cantidad, &indice, heap->cmp));
    }
    if (heap->capacidad/MIN_REDUCCION >= heap->cantidad && 
        heap->capacidad/DIVIDIR_CAPACIDAD >= CAPACIDAD_INICIAL){
        arreglo_redimensionar(heap, heap->capacidad/DIVIDIR_CAPACIDAD);
    }
    return dato;
}
