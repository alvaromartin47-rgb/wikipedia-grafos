#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "pila.h"

/******************************DECLARACIONES*********************************/

typedef struct nodo nodo_t;
typedef enum tipos {BUSCAR, GUARDAR, BORRAR} tipos_t;

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);
void *abb_borrar(abb_t *arbol, const char *clave);
void *abb_obtener(const abb_t *arbol, const char *clave);
bool abb_pertenece(const abb_t *arbol, const char *clave);
size_t abb_cantidad(abb_t *arbol);
void abb_destruir(abb_t *arbol);

nodo_t *buscar_nodo(nodo_t *actual, nodo_t **padre, const char *clave, abb_comparar_clave_t cmp);
nodo_t *wrp_buscar_nodo(const abb_t *abb, const char *clave, tipos_t tipo);
void reemplazar_dato(abb_t *abb, nodo_t *actual, nodo_t *nodo);
void insertar_derecha(abb_t *abb, nodo_t *nodo_padre, nodo_t *nodo_hijo);
void insertar_izquierda(abb_t *abb, nodo_t *nodo_padre, nodo_t *nodo_hijo);
void modif_nodo(abb_t *abb, nodo_t *nodo_padre, nodo_t *nodo_hijo);
void destruir_recursivo(nodo_t *nodo, abb_destruir_dato_t destruir_dato);
void wrp_destruir_recursivo(abb_t *abb);
void *borrar_sin_hijos(abb_t *abb, nodo_t *actual);
void *borrar_con_un_hijo(abb_t *abb, nodo_t *actual);
void *borrar_con_dos_hijos(abb_t *abb, nodo_t *actual);
void *borrar_nodo(abb_t *abb, nodo_t *actual);
void abb_iterar(nodo_t *actual, bool visitar(const char *, void *, void *), void *extra);
void apilar_izq(nodo_t *nodo, pila_t *pila);

/****************************************************************************/

/* *****************************************************************
 *                          ESTRUCTURAS                            *
 * *****************************************************************/
/* Principal */

struct abb {
    nodo_t *raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

/* Auxiliar */

struct nodo {
    struct nodo *h_izq;
    struct nodo *h_der;
    char *clave;
    void *valor;
};

struct abb_iter{
    pila_t *pila;
};

/************************PRIMITIVAS NODO****************************/

nodo_t *nodo_crear(char *clave, void *valor) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
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

void nodo_destruir(nodo_t *nodo, abb_destruir_dato_t destruir_dato) {
    free(nodo->clave);
    if (destruir_dato) destruir_dato(nodo->valor);
    free(nodo);
}

/*******************************************************************/

/* *****************************************************************
 *                         PRIMITIVAS ABB                          *
 * *****************************************************************/

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
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

    nodo_t *nodo_hijo = nodo_crear(clave_copia, valor);
    if (!nodo_hijo) return false;

    nodo_hijo->clave = clave_copia;
    nodo_hijo->valor = valor;

    nodo_t *nodo_padre = wrp_buscar_nodo(abb, clave, GUARDAR);

    modif_nodo(abb, nodo_padre, nodo_hijo);

    return true; 

}

void *abb_borrar(abb_t *abb, const char *clave) {

    if (!clave) return NULL;

    nodo_t *nodo = wrp_buscar_nodo(abb, clave, BUSCAR);

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
    nodo_t *nodo = wrp_buscar_nodo(abb, clave, BUSCAR);
    return !nodo || abb->cmp(clave, nodo->clave) != 0 ? NULL : nodo->valor;
}

bool abb_pertenece(const abb_t *abb, const char *clave) {
    nodo_t *nodo = wrp_buscar_nodo(abb, clave, BUSCAR);
    return !nodo || abb->cmp(clave, nodo->clave) != 0 ? false : true;
}

void abb_destruir(abb_t *abb) {
    wrp_destruir_recursivo(abb);
    free(abb);
}

//Primitivas de iteradores

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
    abb_iterar(arbol->raiz, visitar, extra);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
    abb_iter_t *iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;

    iter->pila = pila_crear();
    if (!iter->pila){
        free(iter);
        return NULL;
    }
    pila_apilar(iter->pila, arbol->raiz);
    apilar_izq((arbol->raiz)->h_izq, iter->pila);

    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
    if (abb_iter_in_al_final(iter)) return false;

    nodo_t *desapilado = pila_desapilar(iter->pila);
    if (desapilado->h_der){
        apilar_izq(desapilado->h_der, iter->pila);
    }

    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
    nodo_t *tope = pila_ver_tope(iter->pila);
    return tope->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
    return (pila_esta_vacia(iter->pila));
}

void abb_iter_in_destruir(abb_iter_t* iter){
    pila_destruir(iter->pila);
    free(iter);
}

/*******************************************************************/

/* *****************************************************************
 *                      FUNCIONES AUXILIARES                       *
 * *****************************************************************/

/*
Busca recursivamente un nodo a traves de la comparacion de la claves y lo devuelve
si es que lo encuentra, de lo contrario devuelve NULL. Actualiza el nodo padre del
buscado en cada llamado.
Pre: _cmp_ es una funcion de comparacion.
Post: se devolvio un nodo o NULL.
*/
nodo_t *buscar_nodo(nodo_t *actual, nodo_t **padre, const char *clave, abb_comparar_clave_t cmp) {
    
    if (!actual) return NULL;

    int num = cmp(clave, actual->clave);

    if (num == 0) return actual;

    *padre = actual;

    if (num > 0) return buscar_nodo(actual->h_der, padre, clave, cmp);
    return buscar_nodo(actual->h_izq, padre, clave, cmp);

}

/*
Recibe un abb, una clave y un tipo. Busca el nodo que contiene _clave_ y a su padre,
luego dependiendo el tipo de busqueda para el que fue llamado la funcion devuelve
el padre o el nodo que posee _clave_. Los tipos pueden ser tres:
Tipo BUSCAR: buscar un nodo en el abb, devuelve el nodo o NULL si no se encuentra.
Tipo GUARDAR: buscar un nodo en el abb para luego guardar, devuelve el padre si el buscado
es NULL (no existe), o el buscado si efectivamente coinciden las claves.
Tipo BORRAR: buscar un nodo en el abb para luego borrar, devuelve siempre el padre.
Pre: el arbol fue creado.
Post: se devolvio un nodo o NULL.
*/
nodo_t *wrp_buscar_nodo(const abb_t *abb, const char *clave, tipos_t tipo) {

    nodo_t *padre = abb->raiz;
    nodo_t *actual = buscar_nodo(abb->raiz, &padre, clave, abb->cmp);

    // Si la busqueda fué para borrar nodo, o fue para guardar y no existe una clave igual
    // a la cual reemplazar devuelvo en ambos casos el padre.
    if (tipo == BORRAR || (tipo == GUARDAR && !actual)) return padre;
    
    // Si solo se trata de una busqueda, o una busqueda para insertar en la cual coinciden
    // las claves, devuelvo el actual.
    return actual;

}

/*
Recibe un abb, y dos nodos que poseen misma clave. Reemplaza el valor que se encuentra
en _nodo_ por el que se encuentra en _actual_ y destruye _nodo_.
Pre: el arbol y los nodos fueron creados y las claves de ambos nodos son iguales.
Post: _actual_ posee el valor que tenia _nodo_ y _nodo_ se destruyó.
*/
void reemplazar_dato(abb_t *abb, nodo_t *actual, nodo_t *nodo) {
    
    void *valor = nodo->valor;
    nodo->valor = actual->valor;
    nodo_destruir(nodo, abb->destruir_dato);
    
    actual->valor = valor;

}

/*
Recibe un abb y dos nodos, padre e hijo. Hace que el nuevo hijo derecho de
padre sea _nodo_hijo_ y que _nodo_hijo_ apunte -si es que tenia- a los hijos
derechos de _nodo_padre_, o por el contrario a NULL.
Pre: el arbol y los nodos fueron creados, el arbol es un abb y la clave de
_nodo_hijo_ es mayor a la de _nodo_padre_.
Post: _nodo_hijo_ ahora es el hijo derecho de _nodo_padre_ y se mantuvo la
condición de abb.
*/
void insertar_derecha(abb_t *abb, nodo_t *nodo_padre, nodo_t *nodo_hijo) {
    
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

/*
Recibe un abb y dos nodos, padre e hijo. Hace que el nuevo hijo izquierdo de
padre sea _nodo_hijo_ y que _nodo_hijo_ apunte -si es que tenia- a los hijos
izquierdos de _nodo_padre_, o por el contrario a NULL.
Pre: el arbol y los nodos fueron creados, el arbol es un abb y la clave de
_nodo_hijo_ es menor a la de _nodo_padre_.
Post: _nodo_hijo_ ahora es el hijo izquierdo de _nodo_padre_ y se mantuvo la
condición de abb.
*/
void insertar_izquierda(abb_t *abb, nodo_t *nodo_padre, nodo_t *nodo_hijo) {
    
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

/*
Recibe un abb, y dos nodos. Padre e hijo. Verifica que efectivamente _nodo_hijo_ sea
el hijo de _nodo_padre_ y lo inserta la derecha si su clave es mayor o a la izquieda si
su clave es menor. En caso de que no sea hijo, es decir que sus claves sean iguales,
reemplazará unicamente el valor de _nodo_padre_ por el de _nodo_hijo_ y éste último se 
destruirá. Si el arbol no tiene nodos inserta _nodo_hijo_ y lo asigna como nueva raíz.
Pre: el arbol y los nodos fueron creados, y el arbol es un abb.
Post si las claves de ambos nodos son distintas, _nodo_hijo_ sera el hijo izquierdo o
derecho de _nodo_padre_ según corresponda. Caso contrario, se intercambiaron los valores y 
se destruyó _nodo_hijo_. Se mantiene la condición de abb.
*/
void modif_nodo(abb_t *abb, nodo_t *nodo_padre, nodo_t *nodo_hijo) {
    
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

/*
Recibe un nodo y una funcion de destruir. Destruye un abb a partir de su
nodo raíz utilizando el recorrido post order de forma recursiva.
*/
void destruir_recursivo(nodo_t *nodo, abb_destruir_dato_t destruir_dato) {

    if (!nodo) return;

    destruir_recursivo(nodo->h_izq, destruir_dato);
    destruir_recursivo(nodo->h_der, destruir_dato);

    nodo_destruir(nodo, destruir_dato);

}

/*
Recibe un abb y destruye todos sus nodos a partir de su raiz, liberando
el espacio que ocupaban en memoria y aplicando la destruccion de sus valores
si existe una función de destrucción.
Pre: el arbol fue creado y es un abb.
Post: se destruyeron todos sus nodos.
*/
void wrp_destruir_recursivo(abb_t *abb) {
    destruir_recursivo(abb->raiz, abb->destruir_dato);
}

/*
Recibe un abb y un nodo sin hijos y lo borra haciendo que su padre apunte
en la direccion de _actual_ a NULL.
Pre: tanto el arbol como el nodo fueron creados y _actual_ no posee hijos.
Post se borró _actual_ del arbol y se mantuvo la condición de abb.
*/
void *borrar_sin_hijos(abb_t *abb, nodo_t *actual) {

    nodo_t *padre = wrp_buscar_nodo(abb, actual->clave, BORRAR);
    void *dato = actual->valor;

    int cmp = abb->cmp(actual->clave, padre->clave);
    
    // Si actual es el hijo izquierdo de su padre
    if (cmp < 0) padre->h_izq = NULL;
    
    // Si es el derecho
    else if (cmp > 0) padre->h_der = NULL;

    abb->cantidad--;
    if (abb->cantidad == 0) abb->raiz = NULL;
    
    nodo_destruir(actual, abb->destruir_dato);

    return dato;

}

/*
Recibe un abb y un nodo con un hijo y lo borra haciendo que el padre apunte a
su nieto en la dirección de _actual_.
Pre: tanto el arbol como el nodo fueron creados y _actual_ posee solo un hijo.
Post se borró _actual_ del arbol y se mantuvo la condición de abb.
*/
void *borrar_con_un_hijo(abb_t *abb, nodo_t *actual) {

    nodo_t *padre = wrp_buscar_nodo(abb, actual->clave, BORRAR);
    

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


    nodo_destruir(actual, abb->destruir_dato);
    return dato;

}

/*
Recibe un nodo y devuelve su reemplazante recursivamente. El criterio es desplazarse
hacia la izquierda hasta encontrar el primer nodo el cual no contenga hijo izquierdo.
Pre: el nodo fue creado.
Post: se devolvió el primer nodo que no tenga hijo izquierdo.
*/
nodo_t *buscar_reemplazante(nodo_t *actual) {
    return (!actual->h_izq) ? actual : buscar_reemplazante(actual->h_izq);
}

/*
Recibe un abb y un nodo con dos hijos. El procedimiento de borrar consiste en
buscar un reemplazante para _actual_ y borrar la posicion donde se encontraba
su reemplazante.
Pre: tanto el arbol como el nodo fueron creados y _actual_ posee dos hijos.
Post se borró _actual_ del arbol y se mantuvo la condición de abb.
*/
void *borrar_con_dos_hijos(abb_t *abb, nodo_t *actual) {
    nodo_t *reemplazante = buscar_reemplazante(actual->h_der);
    
    char *clave = strdup(reemplazante->clave);
    void *valor = borrar_nodo(abb, reemplazante);
    
    free(actual->clave);
    void *valor_ant = actual->valor;
    
    actual->clave = clave;
    actual->valor = valor;
    
    return valor_ant;
}

/*
Recibe un abb y un nodo perteneciente al abb. Realiza el proceso de borrado de
borrado de _actual_ manteniendo la estructura de abb.
Pre: el arbol y el nodo fueron creados, el arbol es un abb y _actual_ pertenece al
abb.
Post: se borro _actual_ y su dato si el abb posee función de destrucción. Se mantuvo
la condición de abb. 
*/
void *borrar_nodo(abb_t *abb, nodo_t *actual) {
    
    // Si no tiene hijos
    if (!actual->h_izq && !actual->h_der) return borrar_sin_hijos(abb, actual);

    // Si tiene un hijo
    else if (!actual->h_izq || !actual->h_der) return borrar_con_un_hijo(abb, actual);

    // Si tiene dos hijos
    else return borrar_con_dos_hijos(abb, actual);

}

/*Recibe el nodo actual de donde se encuentra el iterador, la función de callback visitar y el extra.
Itera el abb in order de manera recursiva y le aplica la función visitar en el nodo que esté parado.*/
void abb_iterar(nodo_t *actual, bool visitar(const char *, void *, void *), void *extra){
    if (!actual) return;

    abb_iterar(actual->h_izq, visitar, extra);
    if (!visitar(actual->clave, actual->valor, extra)) return;
    abb_iterar(actual->h_der, visitar, extra);
}

/*Recibe un nodo del abb y una pila. Apila el nodo actual pasado por parametro y los hijos izquierdos del nodo, 
en caso de tenerlos, en la pila.*/
void apilar_izq(nodo_t *actual, pila_t *pila){
    if (!actual) return;
    pila_apilar(pila, actual);
    apilar_izq(actual->h_izq, pila);
}
/*******************************************************************/
