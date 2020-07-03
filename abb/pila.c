#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INICIAL 10
#define CAPACIDAD_MINIMA 1
#define DIVIDIR / 2
#define MULTIPLICAR *2



/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

bool pila_dimensionar(pila_t* pila, size_t capacidad_nueva);

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
pila_t* pila_crear(void){

    pila_t* pila = malloc(sizeof(pila_t));

    if(pila == NULL){
        return NULL;
    }

    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;
    pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));

    if(pila->datos == NULL){
        free(pila);
        return NULL;
    }

    return pila;

}

void pila_destruir(pila_t* pila){
    
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
    return (pila->cantidad == 0);
}

bool pila_apilar(pila_t *pila, void* valor){

    if (pila->cantidad == pila->capacidad && pila->capacidad > CAPACIDAD_MINIMA){
        if(!pila_dimensionar(pila, pila->capacidad MULTIPLICAR)){
            return false;
        }
    }

    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;

    return true;
}

void* pila_ver_tope(const pila_t *pila){

    if(pila_esta_vacia(pila)){
        return NULL;
    }

    return pila->datos[pila->cantidad - 1];
}

void* pila_desapilar(pila_t *pila){

    if(pila_esta_vacia(pila)){
        return NULL;
    }

    if(pila->cantidad <= pila->capacidad / 4 && pila->capacidad > CAPACIDAD_MINIMA){
        pila_dimensionar(pila, pila->capacidad DIVIDIR);
    }

    pila->cantidad--;
    return pila->datos[pila->cantidad];
}


// FUNCIONES AUXILIARES 


bool pila_dimensionar(pila_t* pila, size_t capacidad_nueva){

    void* datos_nuevos = realloc(pila->datos, capacidad_nueva * sizeof(void*));
    
    if(datos_nuevos == NULL){
        return false;
    }

    pila->datos = datos_nuevos;
    pila->capacidad = capacidad_nueva;
    
    return true;

}
