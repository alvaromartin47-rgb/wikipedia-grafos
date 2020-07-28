#include "aux.h"

/* *****************************************************************
 *                      FUNCIONES AUXILIARES                       *
 * *****************************************************************/

nodo_abb_t *buscar_nodo(nodo_abb_t *actual, nodo_abb_t **padre, const char *clave, abb_cmp_clave_t cmp) {
    
    if (!actual) return NULL;

    int num = cmp(clave, actual->clave);

    if (num == 0) return actual;

    *padre = actual;

    if (num > 0) return buscar_nodo(actual->h_der, padre, clave, cmp);
    return buscar_nodo(actual->h_izq, padre, clave, cmp);

}

nodo_abb_t *wrp_buscar_nodo(const abb_t *abb, const char *clave, tipos_t tipo) {

    nodo_abb_t *padre = abb->raiz;
    nodo_abb_t *actual = buscar_nodo(abb->raiz, &padre, clave, abb->cmp);

    // Si la busqueda fué para borrar nodo, o fue para guardar y no existe una clave igual
    // a la cual reemplazar devuelvo en ambos casos el padre.
    if (tipo == BORRAR || (tipo == GUARDAR && !actual)) return padre;
    
    // Si solo se trata de una busqueda, o una busqueda para insertar en la cual coinciden
    // las claves, devuelvo el actual.
    return actual;

}

void reemplazar_dato(abb_t *abb, nodo_abb_t *actual, nodo_abb_t *nodo) {
    
    void *valor = nodo->valor;
    nodo->valor = actual->valor;
    nodo_abb_destruir(nodo, abb->destruir_dato);
    
    actual->valor = valor;

}

void insertar_derecha(abb_t *abb, nodo_abb_t *nodo_padre, nodo_abb_t *nodo_hijo) {
    
    // Si _nodo_padre_ tiene hijo, lo comparo con el que insertaré, no puede ser igual.
    if (nodo_padre->h_der) {
        int res_cmp = abb->cmp(nodo_padre->h_der->clave, nodo_hijo->clave);
        
        // Si es mayor será el hijo derecho del insertado. Si es menor, el izquierdo.
        if (res_cmp == 1) nodo_hijo->h_der = nodo_padre->h_der;
        else nodo_hijo->h_izq = nodo_padre->h_der;
    
    }
    
    nodo_padre->h_der = nodo_hijo;
    abb->cantidad++;

}

void insertar_izquierda(abb_t *abb, nodo_abb_t *nodo_padre, nodo_abb_t *nodo_hijo) {
    
    // Si _nodo_padre_ tiene hijo, lo comparo con el que insertaré, no puede ser igual.
    if (nodo_padre->h_izq) {
        int res_cmp = abb->cmp(nodo_padre->h_izq->clave, nodo_hijo->clave);
        
        // Si es mayor sera el hijo derecho del insertado. Si es menor, el izquierdo.
        if (res_cmp > 0) nodo_hijo->h_der = nodo_padre->h_izq;
        else nodo_hijo->h_izq = nodo_padre->h_izq;;
    }
    
    nodo_padre->h_izq = nodo_hijo;
    abb->cantidad++;

}

void modif_nodo(abb_t *abb, nodo_abb_t *nodo_padre, nodo_abb_t *nodo_hijo) {
    
    // Si devolvió NULL no hay elementos, entonces inserto en la raiz.
    if (abb->cantidad == 0) {
        abb->raiz = nodo_hijo;
        abb->cantidad++;
    }
    
    else {
        int res_cmp = abb->cmp(nodo_hijo->clave, nodo_padre->clave);
        
        // Si ya existe, reemplazo el dato en nodo_padre.
        if (res_cmp == 0) reemplazar_dato(abb, nodo_padre, nodo_hijo);

        // Si es mayor inserto a la derecha.
        else if (res_cmp > 0) insertar_derecha(abb, nodo_padre, nodo_hijo);

        // Si es menor inserto a la izquierda.
        else insertar_izquierda(abb, nodo_padre, nodo_hijo);
    
    }

}

void destruir_recursivo(nodo_abb_t *nodo, abb_destruir_dato_t destruir_dato) {

    if (!nodo) return;

    destruir_recursivo(nodo->h_izq, destruir_dato);
    destruir_recursivo(nodo->h_der, destruir_dato);

    nodo_abb_destruir(nodo, destruir_dato);

}

void wrp_destruir_recursivo(abb_t *abb) {
    destruir_recursivo(abb->raiz, abb->destruir_dato);
}

void *borrar_sin_hijos(abb_t *abb, nodo_abb_t *actual) {

    nodo_abb_t *padre = wrp_buscar_nodo(abb, actual->clave, BORRAR);
    void *dato = actual->valor;

    int cmp = abb->cmp(actual->clave, padre->clave);
    
    // Si actual es el hijo izquierdo de su padre
    if (cmp < 0) padre->h_izq = NULL;
    
    // Si es el derecho
    else if (cmp > 0) padre->h_der = NULL;

    abb->cantidad--;
    if (abb->cantidad == 0) abb->raiz = NULL;
    
    nodo_abb_destruir(actual, abb->destruir_dato);

    return dato;

}

void *borrar_con_un_hijo(abb_t *abb, nodo_abb_t *actual) {

    nodo_abb_t *padre = wrp_buscar_nodo(abb, actual->clave, BORRAR);
    

    void *dato = actual->valor;

    int cmp = abb->cmp(actual->clave, padre->clave);

    // Si actual es el hijo izquierdo de su padre
    if (cmp < 0) {
        if (actual->h_izq) padre->h_izq = actual->h_izq;
        else padre->h_izq = actual->h_der;
    }

    // Si es el derecho lo mismo pero para la derecha del padre
    else if (cmp > 0) {
        if (actual->h_izq) padre->h_der = actual->h_izq;
        else padre->h_der = actual->h_der;
    }

    abb->cantidad--;

    // Si queda un nodo significa que se borro la raiz
    if (abb->cmp(padre->clave, actual->clave) == 0) {
        if (actual->h_izq) abb->raiz = actual->h_izq;
        else abb->raiz = actual->h_der;
    }


    nodo_abb_destruir(actual, abb->destruir_dato);
    return dato;

}

nodo_abb_t *buscar_reemplazante(nodo_abb_t *actual) {
    return (!actual->h_izq) ? actual : buscar_reemplazante(actual->h_izq);
}

void *borrar_con_dos_hijos(abb_t *abb, nodo_abb_t *actual) {
    nodo_abb_t *reemplazante = buscar_reemplazante(actual->h_der);
    
    char *clave = strdup(reemplazante->clave);
    void *valor = borrar_nodo(abb, reemplazante);
    
    free(actual->clave);
    void *valor_ant = actual->valor;
    
    actual->clave = clave;
    actual->valor = valor;
    
    return valor_ant;
}

void *borrar_nodo(abb_t *abb, nodo_abb_t *actual) {
    
    // Si no tiene hijos
    if (!actual->h_izq && !actual->h_der) return borrar_sin_hijos(abb, actual);

    // Si tiene un hijo
    else if (!actual->h_izq || !actual->h_der) return borrar_con_un_hijo(abb, actual);

    // Si tiene dos hijos
    else return borrar_con_dos_hijos(abb, actual);

}

void abb_iterar(nodo_abb_t *act, char *ini, char *fin, visitar_t visitar, void *extra, bool *ok, abb_cmp_clave_t cmp) {
    if (!act) return;

    int cmp_ini = 0;
    int cmp_fin = 0;

    if (!ini || !fin) {
        if (ini) cmp_fin = 1;
        else cmp_ini = -1;
    }
    
    if (ini) cmp_ini = cmp(ini, act->clave);
    if (fin) cmp_fin = cmp(fin, act->clave);

    if (cmp_ini < 0) abb_iterar(act->h_izq, ini, fin, visitar, extra, ok, cmp);

    if (*ok && cmp_fin > -1 && cmp_ini < 1) {
        *ok = visitar(act->clave, act->valor, extra);
        if (cmp_fin == 0) *ok = false;
    }
    else if (!*ok) return;

    abb_iterar(act->h_der, ini, fin, visitar, extra, ok, cmp);
}


void apilar_inicial(nodo_abb_t *actual, pila_t *pila) {
    if (!actual) return;
    pila_apilar(pila, actual);
    apilar_inicial(actual->h_izq, pila);
}