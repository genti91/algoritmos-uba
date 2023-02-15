#include "heap.h"
#include <stdlib.h>

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

void upheap(void **arreglo, size_t hijo, cmp_func_t cmp){
    if(hijo == 0) return;
    size_t padre = (hijo-1)/2;
    if ((cmp(arreglo[hijo], arreglo[padre]) > 0)){
        swap(arreglo, hijo, padre);
        upheap(arreglo, padre, cmp);
    }
    
}

size_t maximo(void **arreglo, cmp_func_t cmp, size_t padre, size_t h_izq, size_t h_der){
    if (cmp(arreglo[padre], arreglo[h_izq]) < 0 && cmp(arreglo[padre], arreglo[h_der]) > 0) return h_izq;
    if (cmp(arreglo[padre], arreglo[h_izq]) > 0 && cmp(arreglo[padre], arreglo[h_der]) < 0) return h_der;
    if (cmp(arreglo[padre], arreglo[h_izq]) < 0 && cmp(arreglo[padre], arreglo[h_der]) < 0){
        if (cmp(arreglo[h_der], arreglo[h_izq]) > 0) return h_der;
        else return h_izq;
    }
    return padre;
}

void downheap(void **arreglo, size_t tam, size_t padre, cmp_func_t cmp){
    if (padre == tam-1) return;
    size_t h_izq = (2*padre)+1;
    if (h_izq > tam-1) return;
    size_t h_der = (2*padre)+2;
    if (h_der > tam-1) h_der = h_izq;
    size_t max = maximo(arreglo, cmp, padre, h_izq, h_der);
    if (max != padre){
        swap(arreglo, padre, max);
        downheap(arreglo, tam, max, cmp);
    }
}

void heapify(void *elementos[], size_t cant, cmp_func_t cmp){
    for (size_t i = 0; i < cant; i++) {
        size_t indice = cant-i-1;
        downheap(elementos, cant, indice, cmp);
    }
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos, cant, cmp);
    for (size_t i = cant; i > 0; i--) {
        swap(elementos, 0, i-1);
        if (i <= 1) break;
        downheap(elementos, i-1, 0, cmp);
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
    heap->capacidad = n*2;
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
    upheap(heap->arreglo, heap->cantidad-1, heap->cmp);
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
        downheap(heap->arreglo, heap->cantidad, 0, heap->cmp);
    }
    if (heap->capacidad/MIN_REDUCCION >= heap->cantidad && 
        heap->capacidad/DIVIDIR_CAPACIDAD >= CAPACIDAD_INICIAL){
        arreglo_redimensionar(heap, heap->capacidad/DIVIDIR_CAPACIDAD);
    }
    return dato;
}