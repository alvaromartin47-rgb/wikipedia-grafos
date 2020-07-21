#ifndef NODO_ABB_H
#define NODO_ABB_H

#include "../abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ******************************************************************
 *                DECLARACION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

typedef struct nodo_abb nodo_abb_t;

/* ******************************************************************
 *                    DECLARACION DE PRIMITIVAS                     *
 * *****************************************************************/

nodo_abb_t *nodo_abb_crear(char *clave, void *valor);

void nodo_abb_destruir(nodo_abb_t *nodo, abb_destruir_dato_t destruir_dato);

#endif // NODO_ABB_H