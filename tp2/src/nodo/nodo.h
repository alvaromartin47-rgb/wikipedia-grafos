#ifndef NODO_H
#define NODO_H

#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                DECLARACION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

typedef struct nodo nodo_t;

/* ******************************************************************
 *                    DECLARACION DE PRIMITIVAS                     *
 * *****************************************************************/

/*
 * Crea un nodo.
 */
nodo_t* nodo_crear(void *dato);

/*
 * Destruye un nodo liberando la memoria dinámica que ocupaba.
 * Pre: el nodo fue creado.
 */
void nodo_destruir(nodo_t *nodo);

#endif // NODO_H