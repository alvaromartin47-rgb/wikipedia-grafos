#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "testing.h"
#include "../heap.h"
#include "tdas-aux/hash.h"
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

int cmp_numeros(const void *elemento1, const void *elemento2){
    size_t num1 = *(size_t*)elemento1;
    size_t num2 = *(size_t*)elemento2;

    if (num1 > num2) return 1;
    if (num1 == num2) return 0;
    else return -1;
}

/*******************************************************************/

/* ******************************************************************
 *                        PRUEBAS UNITARIAS                         *
 * *****************************************************************/

void prueba_heap_vacio(){
    printf("\nINICIO PRUEBAS HEAP VACIO\n");

    heap_t *heap = heap_crear(cmp_enteros);

    print_test("El heap se creó correctamente", heap != NULL);
    print_test("El heap está vacío", heap_esta_vacio(heap));
    print_test("El max es NULL", heap_ver_max(heap) == NULL);
    print_test("No hay elementos para desencolar", heap_desencolar(heap) == NULL);
    
    heap_destruir(heap, NULL);
    print_test("El heap fue destruido", true);
}

void pruebas_heap_volumen(size_t largo){
    printf("\nINICIO PRUEBAS HEAP MUCHOS ELEMENTOS\n");

    heap_t *heap = heap_crear(cmp_numeros);
    print_test("El heap se creó correctamente", heap != NULL);

    size_t *elementos = malloc(sizeof(size_t) * largo);

    bool ok = true;
    size_t i;

    for (i = 0; i < largo; i++){
        elementos[i] = i;
        ok &= heap_encolar(heap, &elementos[i]);
    }

    print_test("Se encolaron todos los elementos", ok);
    print_test("El heap no está vacío", !heap_esta_vacio(heap));
    print_test("El máximo es correcto", *(size_t*)heap_ver_max(heap) == elementos[largo - 1]);

    for (i = 0; i < largo; i++){
        void *max = heap_ver_max(heap);
        ok &= heap_desencolar(heap) == max;
    }

    print_test("Se desencolaron todos los elementos en el orden correcto", ok);
    print_test("El heap está vacio", heap_esta_vacio(heap));

    free(elementos);
    heap_destruir(heap, NULL);
    print_test("El heap se destruyó correctamente", true);
}

void prueba_heap_NULL(){
    printf("\nINICIO PRUEBAS HEAP ENCOLAR ELEMENTO NULO\n");

    heap_t *heap = heap_crear(cmp_enteros);
    print_test("El heap se creó correctamente", heap != NULL);

    heap_encolar(heap, NULL);

    print_test("El heap no está vacío", !heap_esta_vacio(heap));
    print_test("El máx es correcto", heap_ver_max(heap) == NULL);
    
    heap_destruir(heap, NULL);
}


void prueba_heap_crear_arr(size_t largo) {
    printf("\nINICIO PRUEBAS HEAP CREAR A PARTIR DE ARREGLO\n");

    hash_t *hash = hash_crear(NULL);
    char **claves = malloc(sizeof(char*) * largo);

    obt_aleatorios_y_guardar(hash, claves, largo);
    
    heap_t *heap = heap_crear_arr((void**)claves, largo, cmp_enteros);
    
    bool ok = true;

    while (!heap_esta_vacio(heap)) {
        char *desencolado = heap_desencolar(heap);
        
        if (heap_cantidad(heap) > 1) {
            ok = cmp_enteros(desencolado, (char*)heap_ver_max(heap)) > -1;
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
    printf("\nINICIO PRUEBAS HEAPSORT\n");
    
    hash_t *hash = hash_crear(NULL);
    char **claves = malloc(sizeof(char*) * largo);

    obt_aleatorios_y_guardar(hash, claves, largo);
    
    heap_sort((void**)claves, largo, cmp_enteros);
    
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
    
    prueba_heap_vacio();
    prueba_heap_NULL();
    prueba_heap_crear_arr(1000);
    prueba_heap_sort(1000);
    pruebas_heap_volumen(10);

}

/*******************************************************************/
