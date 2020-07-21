#ifndef AUX_H
#define AUX_H

#include "heap.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* *****************************************************************
 *                          ESTRUCTURAS                            *
 * *****************************************************************/

/* Principal */

struct heap {
    void **datos;
    size_t cantidad;
    size_t capacidad;
    cmp_func_t cmp;
};

/*******************************************************************/

/*
 * Recibe un arreglo genérico y su tamaño, una función de comparación
 * y una posición inicial.
 * Aplica upheap desde _pos_ini_ que consiste en comparar a traves de
 * _cmp_ con su padre y si su prioridad es mayor hacer un swap.
*/
void upheap(void *elementos[], int cant, cmp_func_t cmp, int pos_ini);

/*
 * Recibe un arreglo genérico y su tamaño, una función de comparación
 * y una posición inicial.
 * Aplica downheap desde _pos_ini_ que consiste en comparar a traves de
 * _cmp_ con sus hijos y si su prioridad es menor hacer un swap con el 
 * mayor de ellos.
*/
void downheap(void *elementos[], int cant, cmp_func_t cmp, int pos_ini);

/*
 * Recibe un arreglo genérico y su tamaño, y una función de comparación.
 * Heapify consiste en aplicar downheap a cada uno de los elementos del
 * arreglo convirtiendo el arreglo en un heap. Esto resulta o(n).
*/ 
void heapify(void *elementos[], int cant, cmp_func_t cmp);

/*
 * Recibe un arreglo genérico y dos posiciones del mismo.
 * Intercambia las posiciones entre _pos_a_ y _pos_b_.
 * Pre: el arreglo existe y las posiciones son válidas.
 * Post: se realizó el swap.
*/
void swap(void *elementos[], int pos_a, int pos_b);

/*
 * Recibe una cantidad que conformará un intervalo entre [0, cant - 1],
 * una una función de comparación y una posición inicial.
 * Devuelve la posición donde se encuentra el padre de _pos_ini_ si es
 * que este se encuentra en el intervalo. De lo contrario -1.
 * Pre: _cant_ debe ser mayor o igual a 0.
*/
int obt_padre(int cant, cmp_func_t cmp, int pos_ini);

/*
 * Recibe un tamaño que representará un intervalo entre [0, cant - 1],
 * la posición del elemento del que se quiere obtener sus hijos y el número
 * de hijo (1 o 2), ya que puede tener hasta dos hijos. Devuelve la posición
 * del hijo _n_hijo_ de _pos_ini_ si es que este se encuentra en el intervalo,
 * de lo contrario -1.
*/  
int obt_hijo(int cant, int pos_ini, int n_hijo);

/*
 * Recibe un arreglo genérico y su tamaño, una función de comparación y una
 * posición inicial.
 * Devuelve la posición donde se encuentra el hijo mayor de _pos_ini_, si la
 * prioridad de sus hijos es la misma en ambos se devuelve la posición del hijo
 * izquierdo, de lo contrario -1.
*/ 
int obt_hijo_mayor(void *elementos[], int cant, cmp_func_t cmp, int pos_ini);

/*
 * Recibe un heap y un tamaño.
 * Redimensiona el heap asignandole _tam_nuevo_ y devuelve true. De lo contrario
 * false.
*/
bool heap_redimensionar(heap_t *heap, size_t tam_nuevo);

/*******************************************************************/

#endif // AUX_H