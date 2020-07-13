#include "aux.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define CAP_INICIAL 20
#define AUMENTAR * 2
#define DISMINUIR / 2

/* *****************************************************************
 *                        PRIMITIVAS HEAP                          *
 * *****************************************************************/

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t *heap = heap_crear(cmp);
    if (!heap) return NULL;    
    
    for (size_t i=0; i < n; i++) {
        if (!arreglo[i]) return NULL;
        if (heap->cantidad == heap->capacidad) {
            heap_redimensionar(heap, heap->capacidad AUMENTAR);
        }
        heap->datos[i] = arreglo[i];
        heap->cantidad++;
    }

    heapify(heap->datos, (int)heap->cantidad, cmp);

    return heap;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    int _cant = (int)cant;
    heapify(elementos, _cant, cmp);
    for (int i=1; i < cant; i++) {
        swap(elementos, 0, _cant - i);
        downheap(elementos, _cant - i, cmp, 0);
    }
}

/*******************************************************************/