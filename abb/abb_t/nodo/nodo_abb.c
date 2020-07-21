#include "nodo_abb.h"

struct nodo_abb {
    nodo_abb_t *h_izq;
    nodo_abb_t *h_der;
    char *clave;
    void *valor;
};

/* *****************************************************************
 *                        PRIMITIVAS NODO                          *
 * *****************************************************************/

nodo_abb_t *nodo_abb_crear(char *clave, void *valor) {
    nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));
    if (!nodo) {
        free(clave);
        return NULL;
    }

    nodo->h_izq = NULL;
    nodo->h_der = NULL;
    nodo->clave = clave;
    nodo->valor = valor;

    return nodo;
}

void nodo_abb_destruir(nodo_abb_t *nodo, abb_destruir_dato_t destruir_dato) {
    free(nodo->clave);
    if (destruir_dato) destruir_dato(nodo->valor);
    free(nodo);
}