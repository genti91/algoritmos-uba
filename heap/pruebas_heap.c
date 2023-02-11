#include "heap.h"
#include "testing.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void prueba_crear_destruir_vacia() {
	heap_t *heap = heap_crear(NULL);
	print_test("Se creo correctamente el heap para ser destruida vacia recienbiendo NULL de parametro", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
}

int strcmp_envoltorio(const void* uno, const void* dos) {
    return strcmp(uno, dos);
}

void prueba_crear_destruir() {
	char *a = "test";
	char **puntero1 = &a;
	heap_t *heap = heap_crear(strcmp_envoltorio);
	print_test("Se creo correctamente la heap para ser destruida recienbiendo NULL de parametro", heap_esta_vacio(heap));
	print_test("Se encolo un elemento", heap_encolar(heap, puntero1));
	print_test("Se encolo un elemento", heap_encolar(heap, puntero1));
	print_test("Se encolo un elemento", heap_encolar(heap, puntero1));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
}

void prueba_invariante() {
	char *a = "aa";
	char **puntero1 = &a;
	char *b = "bb";
	char **puntero2 = &b;
	char *c = "cc";
	char **puntero3 = &c;
	char *d = "dd";
	char **puntero4 = &d;
	heap_t *heap = heap_crear(strcmp_envoltorio);
	print_test("Se creo el heap vacio para la prueba general", heap_esta_vacio(heap));
	print_test("Se encolo el primer elemento", heap_encolar(heap, puntero1));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("La cantidad de elementos del heap es 1", heap_cantidad(heap) == 1);
	print_test("El primero es el maximo",  heap_ver_max(heap) == puntero1);
	print_test("Se encolo el segundo elemento", heap_encolar(heap, puntero2));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el segundo elemento",  heap_ver_max(heap) == puntero2);
	print_test("Se encolo el tercer elemento", heap_encolar(heap, puntero3));
	print_test("La cantidad de elementos del heap es 3", heap_cantidad(heap) == 3);
    print_test("El maximo es el tercer elemento",  heap_ver_max(heap) == puntero3);
	print_test("Se desencolo el maximo y es el tercer elemento", (heap_desencolar(heap) == puntero3));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el segundo elemento",  heap_ver_max(heap) == puntero2);
	print_test("Se desencolo el maximo y es el segundo elemento", (heap_desencolar(heap) == puntero2));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("La cantidad de elementos del heap es 1", heap_cantidad(heap) == 1);
	print_test("El maximo es el primer elemento",  heap_ver_max(heap) == puntero1);
	print_test("Se encolo el cuarto elemento", heap_encolar(heap, puntero4));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el cuarto elemento",  heap_ver_max(heap) == puntero4);
    print_test("Se encolo nuevamente el tercer elemento", heap_encolar(heap, puntero3));
    print_test("El maximo es el cuarto elemento",  heap_ver_max(heap) == puntero4);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Se desencolo el maximo y es el cuarto elemento", (heap_desencolar(heap) == puntero4));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el tercer elemento",  heap_ver_max(heap) == puntero3);
	print_test("Se desencolo el maximo y es el tercer elemento", (heap_desencolar(heap) == puntero3));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el primer elemento",  heap_ver_max(heap) == puntero1);
	print_test("Se desencolo el maximo y es el primer elemento", (heap_desencolar(heap) == puntero1));
	print_test("El heap esta vacio", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
}

int comparar_numeros(const void* a, const void* b){
    if (*(int*)a < *(int*)b){
        return -1;
    }
    if (*(int*)a > *(int*)b){
        return 1;
    }
    return 0;
}

size_t rango_aleatorio(size_t minimo, size_t maximo) {
    return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

void prueba_volumen(size_t volumen) {
	srand(getpid());

	heap_t *heap = heap_crear(comparar_numeros);
	print_test("Se creo un heap vacio para prueba de volumen", heap_esta_vacio(heap));

	int arreglo_int[volumen];
	int arreglo_en_orden[volumen];
	int *arreglo_puntero[volumen];

	for (int i = 0; i < volumen; i++) {
		arreglo_int[i] = i;
		arreglo_en_orden[i] = i;
	}

	for (int i = 0; i < volumen; i++) {
    	int indiceAleatorio = (int)rango_aleatorio(0, volumen - 1);
    	int temporal = arreglo_int[i];
    	arreglo_int[i] = arreglo_int[indiceAleatorio];
    	arreglo_int[indiceAleatorio] = temporal;
	}

	for (int i = 0; i < volumen; i++) {
		arreglo_puntero[i] = &arreglo_int[i];
	}

	for (int i = 0; i < volumen; i++) {
		if (!heap_encolar(heap, arreglo_puntero[i])) {
			print_test("Falló el encolar en la prueba de volumen", false);
			heap_destruir(heap, NULL);
			return;
		}
	}
	for (int i = 1; i <= volumen; i++) {
		if (arreglo_en_orden[volumen-i] != *(int*)heap_ver_max(heap)) {
            //printf("%da iteracion \nmaximo: %d \nmaximo esperado: %ld \n", i, *(int*)heap_ver_max(heap), volumen-i);
            //printf("puntero maximo: %p\npuntero maximo esperado: %p\n", heap_ver_max(heap), arreglo_puntero[volumen-i]);
			print_test("El maximo no es el correspondiente en la prueba de volumen. La prueba se frena", false);
			heap_destruir(heap, NULL);
			return;
		}
		if (arreglo_en_orden[volumen-i] != *(int*)heap_desencolar(heap)) {
			print_test("El elemento desencolado no es el correspondiente en la prueba de volumen. La prueba se frena", false);
			heap_destruir(heap, NULL);
			return;
		}
	}
	print_test("El heap esta vacio: se pudo manejar exitosamente el volumen", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
}


void prueba_NULL() {
	void *puntero1 = NULL;

	heap_t *heap = heap_crear(comparar_numeros);
	print_test("Se creo la heap vacia para la prueba de encolar NULL", heap_esta_vacio(heap));
	print_test("Se encolo el primer elemento(NULL)", heap_encolar(heap, puntero1));
	print_test("El primero es el primer elemento(NULL)",  heap_ver_max(heap) == puntero1);
	print_test("Ver maximo me devuelve NULL",  heap_ver_max(heap) == NULL);
	print_test("Pero el heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Se desencolo el primer elemento(NULL)", (heap_desencolar(heap) == puntero1));
	print_test("Ver maximo me devuelve NULL",  heap_ver_max(heap) == NULL);
	print_test("El heap esta vacio", heap_esta_vacio(heap));
	print_test("Desencolar me devuelve NULL",  heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
}

void prueba_reciclaje() {
	char *a = "aa";
	char **puntero1 = &a;
	char *b = "bb";
	char **puntero2 = &b;
	char *c = "cc";
	char **puntero3 = &c;
	char *d = "dd";
	char **puntero4 = &d;
	heap_t *heap = heap_crear(strcmp_envoltorio);
	print_test("Se creo el heap vacio para la prueba general", heap_esta_vacio(heap));
	print_test("Se encolo el primer elemento", heap_encolar(heap, puntero1));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("La cantidad de elementos del heap es 1", heap_cantidad(heap) == 1);
	print_test("El primero es el maximo",  heap_ver_max(heap) == puntero1);
	print_test("Se encolo el segundo elemento", heap_encolar(heap, puntero2));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el segundo elemento",  heap_ver_max(heap) == puntero2);
	print_test("Se encolo el tercer elemento", heap_encolar(heap, puntero3));
	print_test("La cantidad de elementos del heap es 3", heap_cantidad(heap) == 3);
    print_test("El maximo es el tercer elemento",  heap_ver_max(heap) == puntero3);
	print_test("Se desencolo el maximo y es el tercer elemento", (heap_desencolar(heap) == puntero3));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el segundo elemento",  heap_ver_max(heap) == puntero2);
	print_test("Se desencolo el maximo y es el segundo elemento", (heap_desencolar(heap) == puntero2));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("La cantidad de elementos del heap es 1", heap_cantidad(heap) == 1);
	print_test("El maximo es el primer elemento",  heap_ver_max(heap) == puntero1);
	print_test("Se encolo el cuarto elemento", heap_encolar(heap, puntero4));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el cuarto elemento",  heap_ver_max(heap) == puntero4);
    print_test("Se encolo nuevamente el tercer elemento", heap_encolar(heap, puntero3));
    print_test("El maximo es el cuarto elemento",  heap_ver_max(heap) == puntero4);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Se desencolo el maximo y es el cuarto elemento", (heap_desencolar(heap) == puntero4));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el tercer elemento",  heap_ver_max(heap) == puntero3);
	print_test("Se desencolo el maximo y es el tercer elemento", (heap_desencolar(heap) == puntero3));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el primer elemento",  heap_ver_max(heap) == puntero1);
	print_test("Se desencolo el maximo y es el primer elemento", (heap_desencolar(heap) == puntero1));
	print_test("El heap esta vacio", heap_esta_vacio(heap));
	print_test("Se encolo el primer elemento", heap_encolar(heap, puntero1));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El primero es el maximo",  heap_ver_max(heap) == puntero1);
	print_test("Se encolo el segundo elemento", heap_encolar(heap, puntero2));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el segundo elemento",  heap_ver_max(heap) == puntero2);
	print_test("Se encolo el tercer elemento", heap_encolar(heap, puntero3));
    print_test("El maximo es el tercer elemento",  heap_ver_max(heap) == puntero3);
	print_test("Se desencolo el maximo y es el tercer elemento", (heap_desencolar(heap) == puntero3));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el segundo elemento",  heap_ver_max(heap) == puntero2);
	print_test("Se desencolo el maximo y es el segundo elemento", (heap_desencolar(heap) == puntero2));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el primer elemento",  heap_ver_max(heap) == puntero1);
	print_test("Se encolo el cuarto elemento", heap_encolar(heap, puntero4));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el cuarto elemento",  heap_ver_max(heap) == puntero4);
    print_test("Se encolo nuevamente el tercer elemento", heap_encolar(heap, puntero3));
    print_test("El maximo es el cuarto elemento",  heap_ver_max(heap) == puntero4);
    print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Se desencolo el maximo y es el cuarto elemento", (heap_desencolar(heap) == puntero4));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el tercer elemento",  heap_ver_max(heap) == puntero3);
	print_test("Se desencolo el maximo y es el tercer elemento", (heap_desencolar(heap) == puntero3));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("El maximo es el primer elemento",  heap_ver_max(heap) == puntero1);
	print_test("Se desencolo el maximo y es el primer elemento", (heap_desencolar(heap) == puntero1));
	print_test("El heap esta vacio", heap_esta_vacio(heap));
	print_test("Desencolar me devuelve NULL",  heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
}

void prueba_acciones_vacia() {
	heap_t *heap = heap_crear(strcmp_envoltorio);
	print_test("Se creo el heap vacia para la prueba de ver maximo y desencolar sobre heap vacio", heap_esta_vacio(heap));
	print_test("EL heap esta efectivamente vacio", heap_esta_vacio(heap));
	print_test("La cantidad de elementos del heap vacio reciclado es 0", heap_cantidad(heap) == 0);
	print_test("Ver primero en heap vacia devuelve NULL", heap_ver_max(heap) == NULL);
	print_test("Desencolar en heap vacia devuelve NULL", heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
}

void prueba_reciclaje_vacia() {
	int a = 1;
	int *puntero1 = &a;
	int b = 2;
	int *puntero2 = &b;
	int c = 3;
	int *puntero3 = &c;
	int d = 4;
	int *puntero4 = &d;
	heap_t *heap = heap_crear(comparar_numeros);
	print_test("Se creo la heap vacia para la prueba de reciclaje + acciones sobre pila vacia", heap_esta_vacio(heap));
	print_test("Se encolo el primer elemento", heap_encolar(heap, puntero1));
	print_test("Se encolo el segundo elemento", heap_encolar(heap, puntero2));
	print_test("Se encolo el tercer elemento", heap_encolar(heap, puntero3));
	print_test("Se desencolo el maximo", (heap_desencolar(heap) == puntero3));
	print_test("Se desencolo el maximo", (heap_desencolar(heap) == puntero2));
	print_test("Se encolo el cuarto elemento", heap_encolar(heap, puntero4));
	print_test("El heap no esta vacio", !heap_esta_vacio(heap));
	print_test("Se desencolo el maximo", (heap_desencolar(heap) == puntero4));
	print_test("Se desencolo el maximo", (heap_desencolar(heap) == puntero1));
	print_test("El heap esta vaciado", heap_esta_vacio(heap));
	print_test("La cantidad de elementos del heap vacio reciclado es 0", heap_cantidad(heap) == 0);
	print_test("Ver primero en heap vacio reciclado devuelve NULL", heap_ver_max(heap) == NULL);
	print_test("Desencolar en heap vacio reciclado devuelve NULL", heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
}

void prueba_destruir_free() {
	int *puntero1 = malloc(sizeof(int));
	*puntero1 = 1;
	int *puntero2 = malloc(sizeof(int));
	*puntero2 = 2;
	int *puntero3 = malloc(sizeof(int));
	*puntero3 = 3;
	int *puntero4 = malloc(sizeof(int));
	*puntero4 = 4;
	heap_t *heap = heap_crear(comparar_numeros);
	print_test("Se creo la heap vacia para ser destruida recibiendo free de segundo parametro", heap_esta_vacio(heap));
	heap_encolar(heap, puntero1);
	print_test("El primero es el primer elemento",  heap_ver_max(heap) == puntero1);
	heap_encolar(heap, puntero2);
	print_test("El primero es el primer elemento",  heap_ver_max(heap) == puntero2);
	heap_encolar(heap, puntero3);
	print_test("El primero es el primer elemento",  heap_ver_max(heap) == puntero3);
	heap_encolar(heap, puntero4);
	print_test("El primero es el primer elemento",  heap_ver_max(heap) == puntero4);
	free(heap_desencolar(heap));
	print_test("El primero es el segundo elemento",  heap_ver_max(heap) == puntero3);
	heap_destruir(heap, free);
}

void prueba_destruir_free_vacia() {
	heap_t *heap = heap_crear(NULL);
	print_test("Se creo correctamente la heap para ser destruida vacia recienbiendo free de parametro", heap_esta_vacio(heap));
	heap_destruir(heap, free);
}

void prueba_crear_con_arreglo() {
	int a = 1, b = 8, c = 9, d = 65, e = 200, f = 999;
	void *arreglo[6] = {&c, &e, &a, &d, &b, &f};
	// arreglo_esperado del heap: {999, 200, 9, 65, 8, 1};
	heap_t *heap = heap_crear_arr(arreglo, 6, comparar_numeros);
	print_test("Se creo un heap con el arreglo. La cantidad de elementos del heap es 6", heap_cantidad(heap) == 6);
	print_test("Ver maximo me devuelve el correspondiente", heap_ver_max(heap) == &f);
	print_test("Desencolar me devuelve el correspondiente", heap_desencolar(heap) == &f);
	print_test("La cantidad de elementos del heap es 5", heap_cantidad(heap) == 5);
	print_test("Ver maximo me devuelve el correspondiente", heap_ver_max(heap) == &e);
	print_test("Desencolar me devuelve el correspondiente", heap_desencolar(heap) == &e);
	print_test("La cantidad de elementos del heap es 4", heap_cantidad(heap) == 4);
	print_test("Ver maximo me devuelve el correspondiente", heap_ver_max(heap) == &d);
	print_test("Desencolar me devuelve el correspondiente", heap_desencolar(heap) == &d);
	print_test("La cantidad de elementos del heap es 3", heap_cantidad(heap) == 3);
	print_test("Ver maximo me devuelve el correspondiente", heap_ver_max(heap) == &c);
	print_test("Desencolar me devuelve el correspondiente", heap_desencolar(heap) == &c);
	print_test("La cantidad de elementos del heap es 2", heap_cantidad(heap) == 2);
	print_test("Ver maximo me devuelve el correspondiente", heap_ver_max(heap) == &b);
	print_test("Desencolar me devuelve el correspondiente", heap_desencolar(heap) == &b);
	print_test("La cantidad de elementos del heap es 1", heap_cantidad(heap) == 1);
	print_test("Ver maximo me devuelve el correspondiente", heap_ver_max(heap) == &a);
	print_test("Desencolar me devuelve el correspondiente", heap_desencolar(heap) == &a);
	print_test("La cantidad de elementos del heap es 0", heap_cantidad(heap) == 0);
	print_test("El heap esta vacio. El heap se creo correctamente con el arreglo.", heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
}
	
void prueba_heapsort() {
    int a = 3, b = 1, c = 4, d = 2, e = 5;
    void *arreglo[5] = {&a, &b, &c, &d, &e};
    
	heap_sort(arreglo, 5, comparar_numeros);
    bool sort = true;
    for (size_t i = 0; i < 5; i++){
        if (*(int*)arreglo[i] != i+1){
            sort = false;
        }
    }
    print_test("El arreglo se ordena correctamente con heapsort", sort);
}


void pruebas_heap_estudiante() {
    prueba_crear_destruir_vacia();
	prueba_crear_destruir();
	prueba_invariante();
	prueba_volumen(1000);
	prueba_NULL();
	prueba_reciclaje();
	prueba_acciones_vacia();
	prueba_reciclaje_vacia();
	prueba_destruir_free();
	prueba_destruir_free_vacia();
	prueba_heapsort();
    prueba_crear_con_arreglo();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif