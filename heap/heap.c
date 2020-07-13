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

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;

    void **n_datos = malloc(sizeof(void*) * CAP_INICIAL);
    if (!n_datos) {
        free(heap);
        return NULL;
    }

    heap->datos = n_datos;
    heap->cantidad = 0;
    heap->capacidad = CAP_INICIAL;
    heap->cmp = cmp;

    return heap;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (heap_cantidad(heap) == heap->capacidad) {
        if (!heap_redimensionar(heap, (heap->capacidad AUMENTAR))) return false;
    }
    
    heap->datos[heap_cantidad(heap)] = elem;

    upheap(heap->datos, (int)heap_cantidad(heap), heap->cmp, (int)heap_cantidad(heap));
    heap->cantidad++;
    
    return true;
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;

    void *dato = heap->datos[heap_cantidad(heap) - 1];
    heap->datos[0] = heap->datos[heap_cantidad(heap) - 1];
    
    downheap(heap->datos, (int)heap_cantidad(heap), heap->cmp, 0);
    heap->cantidad--;

    if (heap_cantidad(heap) == (heap->capacidad / 4)) {
        heap_redimensionar(heap, heap->capacidad DISMINUIR);
    }

    return dato;
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cantidad;
}
bool heap_esta_vacio(const heap_t *heap) {
    return heap_cantidad(heap) == 0 ? true : false;
}

void *heap_ver_max(const heap_t *heap) {
    return heap_cantidad(heap) > 0 ? heap->datos[0] : NULL;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)){
    for (size_t i = 0; i < heap->cantidad && destruir_elemento; i++){
        destruir_elemento(heap->datos[i]);
    }
    free(heap->datos);
    free(heap);
}

/*******************************************************************/