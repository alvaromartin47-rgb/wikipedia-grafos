#include "aux.h"

/* *****************************************************************
 *                         PRIMITIVAS ABB                          *
 * *****************************************************************/

abb_t *abb_crear(abb_cmp_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t *abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;

    abb->cantidad = 0;
    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;
    abb->raiz = NULL;

    return abb;
}

bool abb_guardar(abb_t *abb, const char *clave, void *valor) {
    
    if (!clave) return false;

    char *clave_copia = strdup(clave);
    if (!clave_copia) return false;

    nodo_abb_t *nodo_hijo = nodo_abb_crear(clave_copia, valor);
    if (!nodo_hijo) return false;

    nodo_hijo->clave = clave_copia;
    nodo_hijo->valor = valor;

    nodo_abb_t *nodo_padre = wrp_buscar_nodo(abb, clave, GUARDAR);

    modif_nodo(abb, nodo_padre, nodo_hijo);

    return true; 

}

void *abb_borrar(abb_t *abb, const char *clave) {

    if (!clave) return NULL;

    nodo_abb_t *nodo = wrp_buscar_nodo(abb, clave, BUSCAR);

    // Si el arbol está vacío o no coincide su clave
    if (!nodo || abb->cmp(clave, nodo->clave) != 0) return NULL;
    
    // Si existe
    void *dato = borrar_nodo(abb, nodo);

    return dato;

}

size_t abb_cantidad(abb_t *abb) {
    return abb->cantidad;
}

void *abb_obtener(const abb_t *abb, const char *clave) {
    nodo_abb_t *nodo = wrp_buscar_nodo(abb, clave, BUSCAR);
    return !nodo || abb->cmp(clave, nodo->clave) != 0 ? NULL : nodo->valor;
}

bool abb_pertenece(const abb_t *abb, const char *clave) {
    nodo_abb_t *nodo = wrp_buscar_nodo(abb, clave, BUSCAR);
    return !nodo || abb->cmp(clave, nodo->clave) != 0 ? false : true;
}

void abb_destruir(abb_t *abb) {
    wrp_destruir_recursivo(abb);
    free(abb);
}

/************************PRIMITIVAS ITERADORES**********************/

void abb_in_order(abb_t *arbol, char *ini, char *fin, bool visitar(const char *, void *, void *), void *extra) {
    bool ok = true;
    abb_iterar(arbol->raiz, ini, fin, visitar, extra, &ok, arbol->cmp);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;

    iter->pila = pila_crear();
    if (!iter->pila) {
        free(iter);
        return NULL;
    }

    apilar_inicial(arbol->raiz, iter->pila);

    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) return false;

    nodo_abb_t *desapilado = pila_desapilar(iter->pila);
    if (desapilado->h_der) {
        apilar_inicial(desapilado->h_der, iter->pila);
    }

    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if (pila_esta_vacia(iter->pila)) return NULL;
    nodo_abb_t *tope = pila_ver_tope(iter->pila);
    return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return (pila_esta_vacia(iter->pila));
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}

/*******************************************************************/
