#include "aux.h"

/* *****************************************************************
 *                      FUNCIONES AUXILIARES                       *
 * *****************************************************************/

void upheap(void *elementos[], int cant, cmp_func_t cmp, int pos_ini) {
    int pos_padre = obt_padre(cant, cmp, pos_ini);

    if (pos_padre == -1) return;

    int res_cmp = cmp(elementos[pos_ini], elementos[pos_padre]);
    if (res_cmp > 0) {
        swap(elementos, pos_ini, pos_padre);
        upheap(elementos, cant, cmp, pos_padre);
    }
}

void downheap(void *elementos[], int cant, cmp_func_t cmp, int pos_ini) {
    int pos_hijo_mayor = obt_hijo_mayor(elementos, cant, cmp, pos_ini);
    if (pos_hijo_mayor == -1) return;

    int res_cmp = cmp(elementos[pos_ini], elementos[pos_hijo_mayor]);
    if (res_cmp < 0) {
        swap(elementos, pos_ini, pos_hijo_mayor);
        downheap(elementos, cant, cmp, pos_hijo_mayor);
    }
}

void heapify(void *elementos[], int cant, cmp_func_t cmp) {
    for (int i=(cant-1); i >= 0; i--) downheap(elementos, cant, cmp, i); 
}

void swap(void *elementos[], int pos_a, int pos_b) {
    void *gen_b = elementos[pos_b];
    elementos[pos_b] = elementos[pos_a];
    elementos[pos_a] = gen_b;
}

int obt_hijo(int cant, int pos_ini, int n_hijo) {
    int pos_hijo = (pos_ini * 2) + n_hijo;  
    return pos_hijo >= 0 && pos_hijo <= (cant-1) ? pos_hijo : -1; 
}

int obt_hijo_mayor(void *elementos[], int cant, cmp_func_t cmp, int pos_ini) {
    int pos_hijo_mayor;

    int h_izq = obt_hijo(cant, pos_ini, 1);
    int h_der = obt_hijo(cant, pos_ini, 2);
    
    if (h_izq > -1 && h_der > -1) {
        int res_cmp = cmp(elementos[h_izq], elementos[h_der]);
        if (res_cmp >= 0) pos_hijo_mayor = h_izq; 
        else pos_hijo_mayor = h_der;
    }
    
    else if (h_der > -1) pos_hijo_mayor = h_der;

    else pos_hijo_mayor = h_izq;

    return pos_hijo_mayor;
}

int obt_padre(int cant, cmp_func_t cmp, int pos_ini) {
    int pos_padre = (pos_ini - 1) / 2;  
    return pos_padre >= 0 && pos_padre <= (cant-1) ? pos_padre : -1; 
}

bool heap_redimensionar(heap_t *heap, size_t tam_nuevo) {
    if (tam_nuevo > 0) {
    	void** datos_nuevo = realloc(heap->datos, tam_nuevo * sizeof(void*));
    	
    	if (!datos_nuevo) return false;
    	
    	heap->datos = datos_nuevo;
    	heap->capacidad = tam_nuevo;
    	
        return true;
    }
    
    return false;
}

/*******************************************************************/
