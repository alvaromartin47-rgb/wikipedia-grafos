#include "cola.h"
#include <stdio.h>
#include <stdlib.h>

/* *****************************************************************
 *                  TDA COLA Y SUS PRIMITIVAS
 * *****************************************************************/

struct nodo {
    void *dato;
    nodo_t *sig;
};

struct cola {
    nodo_t *prim;
    nodo_t *ult;
};

bool cola_esta_vacia(const cola_t *cola) {
    return cola->prim == NULL ? true:false;
}

void* cola_ver_primero(const cola_t *cola) {
    return cola_esta_vacia(cola) ? NULL : cola->prim->dato;
}

bool cola_encolar(cola_t *cola, void* dato) {
    nodo_t *nodo = nodo_crear(dato);
    
    if (nodo == NULL) return false;    

    if (cola_esta_vacia(cola)) {
        cola->prim = nodo;
        cola->ult = nodo;
        
        return true;
    }
    
    cola->ult->sig = nodo;
    cola->ult = nodo;

    return true;
}

void* cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) return NULL;

    nodo_t* sig_de_prim_a_borrar = cola->prim->sig;
    void* dato = cola->prim->dato;

    nodo_destruir(cola->prim);

    if(sig_de_prim_a_borrar != NULL) {
        cola->prim = sig_de_prim_a_borrar;
        return dato;
    }

    cola->prim = NULL;
    cola->ult = NULL;

    return dato;
}

cola_t *cola_crear(void) {
    cola_t *cola = malloc(sizeof(cola_t));
    
    if (cola == NULL) return NULL;
    
    cola->prim = NULL;
    cola->ult = NULL;
    
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)) {
    while (!cola_esta_vacia(cola)) {
        void *dato = cola_desencolar(cola);
        
        if (destruir_dato != NULL) destruir_dato(dato);
    }
    free(cola);
}


/* ------------------------------------------------------------------- */