#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "testing.h"
#include "heap.h"
#include "hash.h"
#include <time.h>

/* ******************************************************************
 *                       FUNCIONES AUXILIARES                       *
 * *****************************************************************/

int cmp_enteros(const void *str1, const void *str2) {
    int num1 = atoi((char*)str1);
    int num2 = atoi((char*)str2);
    if (num1 > num2) return 1;
    if (num1 < num2) return -1;
    return 0;
}

void obt_aleatorios_y_guardar(hash_t *hash, char **claves, size_t largo) {
    
    srand((unsigned int)time(NULL));

    for (size_t i=0; i < largo; i++) {
        char *clave = malloc(sizeof(char) * 10);
        size_t num = (size_t)rand()%(largo) + 1;
        sprintf(clave, "%ld", num);
        
        while (hash_pertenece(hash, clave)) {
            num = (size_t)rand()%(largo) + 1;
            sprintf(clave, "%ld", num);
        }

        hash_guardar(hash, clave, &num);
        claves[i] = clave;
    }

}

/*******************************************************************/

/* ******************************************************************
 *                        PRUEBAS UNITARIAS                         *
 * *****************************************************************/

void prueba_heap_crear_arr(size_t largo) {
    hash_t *hash = hash_crear(NULL);
    char **claves = malloc(sizeof(char*) * largo);

    obt_aleatorios_y_guardar(hash, claves, largo);
    
    heap_t *heap = heap_crear_arr(claves, largo, cmp_enteros);
    //heap_sort(claves, largo, cmp_enteros);
    
    bool ok = true;

    while (!heap_esta_vacio(heap)) {
        char *desencolado = heap_desencolar(heap);
        
        if (heap_cantidad(heap) > 1) {
            ok = cmp(desencolado, (char*)heap_ver_max(heap)) > -1;
            if (!ok) break;
        }

        ok = hash_pertenece(hash, desencolado);
        if (!ok) break;
    }

    print_test("Prueba heap crear a partir de arreglo", ok);

    hash_destruir(hash);
    heap_destruir(heap, NULL);
    for (size_t i=0; i < largo; i++) free(claves[i]);
    free(claves);
}

void prueba_heap_sort(size_t largo) {
    hash_t *hash = hash_crear(NULL);
    char **claves = malloc(sizeof(char*) * largo);

    obt_aleatorios_y_guardar(hash, claves, largo);
    
    heap_sort(claves, largo, cmp_enteros);
    
    bool ok = true;

    for (size_t i=0; i < largo; i++) {
        if (i < largo - 1) ok = cmp_enteros(claves[i], claves[i + 1]) < 1;  
        if (!ok) break;
    }

    print_test("Prueba ordenar con heapsort a partir de arreglo", ok);

    hash_destruir(hash);
    for (size_t i=0; i < largo; i++) free(claves[i]);
    free(claves);
}


void pruebas_heap_alumno() {
    prueba_heap_crear_arr(1000);
    prueba_heap_sort(1000);
}

/*******************************************************************/