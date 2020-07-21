#include "nodo.h"

struct nodo {
    void *dato;
    nodo_t *sig;
};

/* *****************************************************************
 *                       PRIMITIVAS PERSONA                        *
 * *****************************************************************/

nodo_t* nodo_crear(void *dato) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    
    if (!nodo) return NULL;
    
    nodo->dato = dato;
    nodo->sig = NULL;
    
    return nodo;   
}

void nodo_destruir(nodo_t *nodo) {
    free(nodo);
}