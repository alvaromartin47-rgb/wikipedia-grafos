#ifndef AUX_H
#define AUX_H

#define _POSIX_C_SOURCE 200809L
#include "abb.h"
#include "pila.h"
#include "nodo/nodo_abb.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* *****************************************************************
 *                            ESTRUCTURAS                          *
 * *****************************************************************/

typedef enum tipos {BUSCAR, GUARDAR, BORRAR} tipos_t;

struct abb {
    nodo_abb_t *raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

struct abb_iter{
    pila_t *pila;
};

struct nodo_abb {
    nodo_abb_t *h_izq;
    nodo_abb_t *h_der;
    char *clave;
    void *valor;
};

/* *****************************************************************
 *                            FUNCIONES                            *
 * *****************************************************************/

/*
Busca recursivamente un nodo a traves de la comparacion de la claves y lo devuelve
si es que lo encuentra, de lo contrario devuelve NULL. Actualiza el nodo padre del
buscado en cada llamado.
Pre: _cmp_ es una funcion de comparacion.
Post: se devolvio un nodo o NULL.
*/
nodo_abb_t *buscar_nodo(nodo_abb_t *actual, nodo_abb_t **padre, const char *clave, abb_comparar_clave_t cmp);

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
nodo_abb_t *wrp_buscar_nodo(const abb_t *abb, const char *clave, tipos_t tipo);

/*
Recibe un abb, y dos nodos que poseen misma clave. Reemplaza el valor que se encuentra
en _nodo_ por el que se encuentra en _actual_ y destruye _nodo_.
Pre: el arbol y los nodos fueron creados y las claves de ambos nodos son iguales.
Post: _actual_ posee el valor que tenia _nodo_ y _nodo_ se destruyó.
*/
void reemplazar_dato(abb_t *abb, nodo_abb_t *actual, nodo_abb_t *nodo);

/*
Recibe un abb y dos nodos, padre e hijo. Hace que el nuevo hijo derecho de
padre sea _nodo_hijo_ y que _nodo_hijo_ apunte -si es que tenia- a los hijos
derechos de _nodo_padre_, o por el contrario a NULL.
Pre: el arbol y los nodos fueron creados, el arbol es un abb y la clave de
_nodo_hijo_ es mayor a la de _nodo_padre_.
Post: _nodo_hijo_ ahora es el hijo derecho de _nodo_padre_ y se mantuvo la
condición de abb.
*/
void insertar_derecha(abb_t *abb, nodo_abb_t *nodo_padre, nodo_abb_t *nodo_hijo);

/*
Recibe un abb y dos nodos, padre e hijo. Hace que el nuevo hijo izquierdo de
padre sea _nodo_hijo_ y que _nodo_hijo_ apunte -si es que tenia- a los hijos
izquierdos de _nodo_padre_, o por el contrario a NULL.
Pre: el arbol y los nodos fueron creados, el arbol es un abb y la clave de
_nodo_hijo_ es menor a la de _nodo_padre_.
Post: _nodo_hijo_ ahora es el hijo izquierdo de _nodo_padre_ y se mantuvo la
condición de abb.
*/
void insertar_izquierda(abb_t *abb, nodo_abb_t *nodo_padre, nodo_abb_t *nodo_hijo);

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
void modif_nodo(abb_t *abb, nodo_abb_t *nodo_padre, nodo_abb_t *nodo_hijo);

/*
Recibe un nodo y una funcion de destruir. Destruye un abb a partir de su
nodo raíz utilizando el recorrido post order de forma recursiva.
*/
void destruir_recursivo(nodo_abb_t *nodo, abb_destruir_dato_t destruir_dato);

/*
Recibe un abb y destruye todos sus nodos a partir de su raiz, liberando
el espacio que ocupaban en memoria y aplicando la destruccion de sus valores
si existe una función de destrucción.
Pre: el arbol fue creado y es un abb.
Post: se destruyeron todos sus nodos.
*/
void wrp_destruir_recursivo(abb_t *abb);

/*
Recibe un abb y un nodo sin hijos y lo borra haciendo que su padre apunte
en la direccion de _actual_ a NULL.
Pre: tanto el arbol como el nodo fueron creados y _actual_ no posee hijos.
Post se borró _actual_ del arbol y se mantuvo la condición de abb.
*/
void *borrar_sin_hijos(abb_t *abb, nodo_abb_t *actual);

/*
Recibe un abb y un nodo con un hijo y lo borra haciendo que el padre apunte a
su nieto en la dirección de _actual_.
Pre: tanto el arbol como el nodo fueron creados y _actual_ posee solo un hijo.
Post se borró _actual_ del arbol y se mantuvo la condición de abb.
*/
void *borrar_con_un_hijo(abb_t *abb, nodo_abb_t *actual);

/*
Recibe un nodo y devuelve su reemplazante recursivamente. El criterio es desplazarse
hacia la izquierda hasta encontrar el primer nodo el cual no contenga hijo izquierdo.
Pre: el nodo fue creado.
Post: se devolvió el primer nodo que no tenga hijo izquierdo.
*/
nodo_abb_t *buscar_reemplazante(nodo_abb_t *actual);

/*
Recibe un abb y un nodo con dos hijos. El procedimiento de borrar consiste en
buscar un reemplazante para _actual_ y borrar la posicion donde se encontraba
su reemplazante.
Pre: tanto el arbol como el nodo fueron creados y _actual_ posee dos hijos.
Post se borró _actual_ del arbol y se mantuvo la condición de abb.
*/
void *borrar_con_dos_hijos(abb_t *abb, nodo_abb_t *actual);

/*
Recibe un abb y un nodo perteneciente al abb. Realiza el proceso de borrado de
borrado de _actual_ manteniendo la estructura de abb.
Pre: el arbol y el nodo fueron creados, el arbol es un abb y _actual_ pertenece al
abb.
Post: se borro _actual_ y su dato si el abb posee función de destrucción. Se mantuvo
la condición de abb. 
*/
void *borrar_nodo(abb_t *abb, nodo_abb_t *actual);

/*
Recibe el nodo actual de donde se encuentra el iterador, la función de callback visitar y el extra.
Itera el abb in order de manera recursiva y le aplica la función visitar en el nodo que esté parado.
*/
void abb_iterar(nodo_abb_t *actual, bool visitar(const char *, void *, void *), void *extra, bool *ok);

/*
Recibe un nodo del abb y una pila. Apila el nodo actual pasado por parametro y los hijos izquierdos del nodo, 
en caso de tenerlos, en la pila.
*/
void apilar_izq(nodo_abb_t *actual, pila_t *pila);

#endif // AUX_H