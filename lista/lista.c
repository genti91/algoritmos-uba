#include "lista.h"
#include <stdlib.h>

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