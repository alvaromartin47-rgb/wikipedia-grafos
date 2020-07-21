#include "lista.h"

/* *****************************************************************
 *               TDA LISTA ENLAZADA Y SUS PRIMITIVAS
 * *****************************************************************/

struct nodo {
    void *dato;
    nodo_t *sig;
};

struct lista {
    nodo_t *prim;
    nodo_t *ult;
    size_t largo;
};

lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    
    if (!lista) return NULL;
    
    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    
    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->largo == 0 ? true : false;
}
    
bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t *nodo = nodo_crear(dato);
    if (!nodo) return false;
    
    if (lista_esta_vacia(lista)) lista->ult = nodo;
    else nodo->sig = lista->prim;

    lista->prim = nodo;
    lista->largo ++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t *nodo = nodo_crear(dato);
    if (!nodo) return false;
    
    if (lista_esta_vacia(lista)) lista->prim = nodo;
    else lista->ult->sig = nodo;
    
    lista->ult = nodo;
    lista->largo ++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;

    void *dato = lista->prim->dato;
    nodo_t *ref_prim = lista->prim;

    if (lista->largo == 1) {
        lista->prim = NULL;
        lista->ult = NULL;
    } 
    else lista->prim = ref_prim->sig;
    
    nodo_destruir(ref_prim);
    lista->largo --;
    return dato;
}

void *lista_ver_primero(const lista_t *lista) {
    return lista_esta_vacia(lista) ? NULL : lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    return lista_esta_vacia(lista) ? NULL : lista->ult->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void*)) {
    while (lista->prim) {
        void *dato = lista_borrar_primero(lista);
        
        if (destruir_dato != NULL) destruir_dato(dato);
    }
    free(lista);
}

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t *iter = lista->prim;
    while (iter) {
        if (!visitar(iter->dato, extra)) break;
        iter = iter->sig;
    }
}

/* ------------------------------------------------------------------- */

/* *****************************************************************
 *               TDA ITERADOR EXTERNO Y PRIMITIVAS
 * *****************************************************************/

struct lista_iter {
    lista_t *lista; // Puntero a la lista enlazada
    nodo_t *actual; // Puntero al nodo actual de la lista (inialmente el primero)
    nodo_t *anterior; // Puntero al nodo anterior de la lista (inicialmente NULL)
};

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));
    
    if (iter == NULL) return NULL;

    iter->lista = lista;
    iter->anterior = NULL;
    iter->actual = iter->lista->prim;
    
    return iter;
}

/*---------------------------------------------------------------*/
bool lista_iter_al_inicio(lista_iter_t *iter);
/*---------------------------------------------------------------*/

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->actual  == NULL;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    return lista_iter_al_final(iter) ? NULL : iter->actual->dato;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return false;

    iter->anterior = iter->actual;
    iter->actual = iter->actual->sig;
    return true;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t *nodo = nodo_crear(dato);
    if (!nodo) return false;

    nodo->sig = iter->actual;

    if (lista_iter_al_inicio(iter)) iter->lista->prim = nodo;
    else iter->anterior->sig = nodo;

    if(lista_iter_al_final(iter)) iter->lista->ult = nodo;

    iter->actual = nodo;
    iter->lista->largo ++;

    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;

    void *dato = iter->actual->dato;
    nodo_t *ref_sig = iter->actual->sig;

    if(lista_iter_al_inicio(iter)) iter->lista->prim = iter->actual->sig;
    else iter->anterior->sig = iter->actual->sig;

    if (!iter->actual->sig) iter->lista->ult = iter->anterior;

    nodo_destruir(iter->actual);
    iter->actual = ref_sig;
    iter->lista->largo --;

    return dato;
}

/* *****************************************************************
 *                         FUNCIONES AUXILIARES
 * *****************************************************************/

/*
Recibe un iterador y devuelve true si este se encuentra al inicio de
la lista, de lo contrario false.
*/
bool lista_iter_al_inicio(lista_iter_t *iter) {
    return iter->anterior == NULL;
}
