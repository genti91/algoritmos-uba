#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

// Es la función que determina si una clave es mayor o menor que otra.
// Si la primera clave recibida por parámetro es mayor se devuelve 1,
// en caso contrario -1, y si son iguales 0.
typedef int (*abb_comparar_clave_t) (const char *, const char *);

// Es la función que corresponde para destruir los datos en el caso de
// pisar un dato vinculado con una clave ya existente o destruir el árbol.
typedef void (*abb_destruir_dato_t) (void *);

// Crea un árbol binario de búsqueda.
// Post: devuelve un ABB vacío.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda un elemento en el ABB, si la clave ya se encuentra en la
// estructura, la reemplaza. De no poder guardarlo devuelve false.
// Pre: La estructura ABB fue inicializada.
// Post: Se almacenó el par (clave, dato) en el ABB.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra una clave del hash y devuelve el dato asociado. Devuelve NULL si la 
// clave no existía en el ABB.
// Pre: La estructura ABB fue inicializada.
// Post: La clave fue borrada de la estructura y se devolvió el dato asociado,
// en el caso de que estuviera guardado.
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el valor de una clave del ABABBsi la clave no se encuentra
// devuelve NULL.
// Pre: La estructura ABB fue inicializada.
void *abb_obtener(const abb_t *arbol, const char *clave);

// Determina si clave pertenece o no al ABB.
// Pre: La estructura ABB fue inicializada.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de elementos del ABB.
// Pre: La estructura ABB fue inicializada.
size_t abb_cantidad(const abb_t *arbol);

// Destruye la estructura liberando la memoria pedida y llamando a la función
// destruir para cada par (clave, dato).
// Pre: La estructura ABB fue inicializada
// Post: La estructura ABB fue destruida
void abb_destruir(abb_t *arbol);

// Recorre todos los elementos del ABB en orden y les aplica a cada dato la función recibida por parámetro
// hasta que ésta devuelva falso. La función visitar recibe por parámetro la clave y dato actual y una condición
// extra compartida en las iteraciones.
// Pre: La estructura ABB fue inicializada.
// Post: Se aplicó la función visitar a los datos en orden hasta que visitar devolvió falso.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);

// Iterador del hash //

// Crea iterador
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza iterador
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Comprueba si terminó la iteración
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye iterador
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H