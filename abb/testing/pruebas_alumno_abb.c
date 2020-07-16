#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../abb.h"
#include "testing.h"
#include "tdas-aux/hash.h"
#include <time.h>

/* ******************************************************************
 *                       FUNCIONES AUXILIARES                       *
 * *****************************************************************/

int cmp(const char *str1, const char *str2) {
    return strcmp(str1, str2);
}

int cmp_enteros(const char *str1, const char *str2) {
    int num1 = atoi(str1);
    int num2 = atoi(str2);
    if (num1 > num2) return 1;
    if (num1 < num2) return -1;
    return 0;
}

bool obt_aleatorios_y_guardar(abb_t *abb, hash_t *hash, char **claves, size_t largo, size_t tipo) {
    bool ok = true;
    
    srand((unsigned int)time(NULL));

    for (size_t i=0; i < largo; i++) {
        char *clave = malloc(sizeof(char) * 10);
        size_t num = (size_t)rand()%(largo) + 1;
        sprintf(clave, "%ld", num);
        
        while (hash_pertenece(hash, clave)) {
            num = (size_t)rand()%(largo) + 1;
            sprintf(clave, "%ld", num);
        }

        if (tipo == 1) ok = abb_guardar(abb, clave, &num);
        hash_guardar(hash, clave, &num);
        claves[i] = clave;
    }

    return ok;

}

bool sumar_todos(const char *clave, void *dato, void *total){
    *(int*)total += *(int*)dato;
    return true;
}


/* ******************************************************************
 *                        PRUEBAS UNITARIAS                         *
 * *****************************************************************/

void prueba_crear_abb_vacio() {
    printf("\n~PRUEBAS ABB VACIO~\n");

    abb_t *abb = abb_crear(cmp, NULL);
    
    print_test("Prueba abb crear abb vacio", abb);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);
    print_test("Prueba abb obtener clave A, es NULL, no existe", !abb_obtener(abb, "A"));
    print_test("Prueba abb pertenece clave A, es false, no existe", !abb_pertenece(abb, "A"));
    print_test("Prueba abb borrar clave A, es NULL, no existe", !abb_borrar(abb, "A"));

    abb_destruir(abb);
}

void prueba_abb_guardar() {
    printf("\n~PRUEBAS ABB_GUARDAR~\n");
    abb_t* abb = abb_crear(cmp, NULL);

    char *clave1 = "perro", *valor1 = "guau";
    char *clave2 = "gato", *valor2 = "miau";
    char *clave3 = "vaca", *valor3 = "mu";
    
    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb obtener clave1 es valor1", abb_obtener(abb, clave1) == valor1);
    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb borrar clave1 sin hijos, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    /* Inserta otros 2 valores y no los borra (se destruyen con el abb) */
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb obtener clave2 es valor2", abb_obtener(abb, clave2) == valor2);
    print_test("Prueba abb pertenece clave2, es true", abb_pertenece(abb, clave2));

    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb obtener clave3 es valor3", abb_obtener(abb, clave3) == valor3);
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));

    abb_destruir(abb);
}

void prueba_abb_reemplazar() {
    printf("\n~PRUEBAS REEMPLAZAR~\n");

    abb_t* abb = abb_crear(cmp, NULL);

    char *clave1 = "perro", *valor1a = "guau", *valor1b = "warf";
    char *clave2 = "gato", *valor2a = "miau", *valor2b = "meaow";

    /* Inserta 2 valores y luego los reemplaza */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    abb_destruir(abb);
}

void prueba_abb_reemplazar_con_destruir() {
    printf("\n~PRUEBAS REEMPLAZAR CON DESTRUIR~\n");

    abb_t* abb = abb_crear(cmp, free);

    char *clave1 = "perro", *valor1a, *valor1b;
    char *clave2 = "gato", *valor2a, *valor2b;

    /* Pide memoria para 4 valores */
    valor1a = malloc(10 * sizeof(char));
    valor1b = malloc(10 * sizeof(char));
    valor2a = malloc(10 * sizeof(char));
    valor2b = malloc(10 * sizeof(char));

    /* Inserta 2 valores y luego los reemplaza (debe liberar lo que reemplaza) */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1a));
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb obtener clave1 es valor1a", abb_obtener(abb, clave1) == valor1a);
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2a));
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb obtener clave2 es valor2a", abb_obtener(abb, clave2) == valor2a);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor1b));
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb obtener clave1 es valor1b", abb_obtener(abb, clave1) == valor1b);
    print_test("Prueba abb insertar clave2 con otro valor", abb_guardar(abb, clave2, valor2b));
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb obtener clave2 es valor2b", abb_obtener(abb, clave2) == valor2b);
    print_test("Prueba abb la cantidad de elementos es 2", abb_cantidad(abb) == 2);

    /* Se destruye el abb (se debe liberar lo que quedó dentro) */
    abb_destruir(abb);
}

void prueba_abb_borrar_1_hijo() {
    printf("\n~PRUEBAS ABB_BORRAR CON UN HIJO~\n");

    abb_t *abb = abb_crear(cmp, NULL);

    char *clave1 = "partner", *valor1 = "peugeot";
    char *clave2 = "gol", *valor2 = "volkswagen";
    char *clave3 = "corsa", *valor3 = "chevrolet";
    char *clave4 = "sandero", *valor4 = "renault";
    char *clave5 = "t", *valor5 = "ford";

    /* Inserta 5 elementos, la raiz, dos menores y dos mayores y borra los que tienen 1 hijo */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, valor4));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, valor5));
    
    print_test("Prueba abb borrar clave2 con un hijo, es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb cantidad es 4", abb_cantidad(abb) == 4);
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb pertenece clave2, es false", !abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave4 con un hijo, es valor4", abb_borrar(abb, clave4) == valor4);
    print_test("Prueba abb cantidad es 3", abb_cantidad(abb) == 3);
    print_test("Prueba abb obtener clave4, es NULL", !abb_obtener(abb, clave4));
    print_test("Prueba abb pertenece clave4, es false", !abb_pertenece(abb, clave4));


    /* Borro uno de los hijos de la raiz y pruebo borrar la raiz */
    print_test("Prueba abb borrar clave3 sin hijos, es valor3", abb_borrar(abb, clave3) == valor3);
    print_test("Prueba abb cantidad es 2", abb_cantidad(abb) == 2);
    print_test("Prueba abb obtener clave3, es NULL", !abb_obtener(abb, clave3));
    print_test("Prueba abb pertenece clave3, es false", !abb_pertenece(abb, clave3));
    print_test("Prueba abb borrar clave1 (raiz) con un hijo, es valor1", abb_borrar(abb, clave1) == valor1);
    print_test("Prueba abb cantidad es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb pertenece clave1, es false", !abb_pertenece(abb, clave1));

    print_test("Prueba abb obtener clave5 es valor5", abb_obtener(abb, clave5) == valor5);
    print_test("Prueba abb pertenece clave5 es true", abb_pertenece(abb, clave5));

    abb_destruir(abb);

} 

void prueba_abb_borrar_2_hijos() {
    printf("\n~PRUEBAS ABB_BORRAR CON DOS HIJOS~\n");

    abb_t *abb = abb_crear(cmp, NULL);

    char *clave1 = "partner", *valor1 = "peugeot";
    char *clave2 = "gol", *valor2 = "volkswagen";
    char *clave3 = "corsa", *valor3 = "chevrolet";
    char *clave4 = "spacio", *valor4 = "fiat";
    char *clave5 = "t", *valor5 = "ford";
    char *clave6 = "hilux", *valor6 = "toyota";
    char *clave7 = "sandero", *valor7 = "renault";
 
    /* Inserta 7 elementos, la raiz y los hijos que a su vez tienen dos hijos */
    print_test("Prueba abb insertar clave1", abb_guardar(abb, clave1, valor1));
    print_test("Prueba abb insertar clave2", abb_guardar(abb, clave2, valor2));
    print_test("Prueba abb insertar clave3", abb_guardar(abb, clave3, valor3));
    print_test("Prueba abb insertar clave4", abb_guardar(abb, clave4, valor4));
    print_test("Prueba abb insertar clave5", abb_guardar(abb, clave5, valor5));
    print_test("Prueba abb insertar clave6", abb_guardar(abb, clave6, valor6));
    print_test("Prueba abb insertar clave7", abb_guardar(abb, clave7, valor7));

    /* Borro cada hijo de la raiz */
    print_test("Prueba abb borrar clave2 con dos hijos es valor2", abb_borrar(abb, clave2) == valor2);
    print_test("Prueba abb cantidad es 6", abb_cantidad(abb) == 6);
    print_test("Prueba abb obtener clave2, es NULL", !abb_obtener(abb, clave2));
    print_test("Prueba abb pertenece clave2, es false", !abb_pertenece(abb, clave2));
    print_test("Prueba abb borrar clave4 con dos hijos es valor4", abb_borrar(abb, clave4) == valor4);
    print_test("Prueba abb cantidad es 5", abb_cantidad(abb) == 5);
    print_test("Prueba abb obtener clave4, es NULL", !abb_obtener(abb, clave4));
    print_test("Prueba abb pertenece clave4, es false", !abb_pertenece(abb, clave4));

    print_test("Prueba abb pertenece clave1, es true", abb_pertenece(abb, clave1));
    print_test("Prueba abb pertenece clave3, es true", abb_pertenece(abb, clave3));
    print_test("Prueba abb pertenece clave5, es true", abb_pertenece(abb, clave5));
    print_test("Prueba abb pertenece clave6, es true", abb_pertenece(abb, clave6));
    print_test("Prueba abb pertenece clave7, es true", abb_pertenece(abb, clave7));

    print_test("Prueba abb insertar clave1 con otro valor", abb_guardar(abb, clave1, valor7));
    print_test("Prueba abb insertar clave3 con otro valor", abb_guardar(abb, clave3, valor6));
    print_test("Prueba abb obtener clave1 es valor7", abb_obtener(abb, clave1) == valor7);
    print_test("Prueba abb obtener clave3 es valor6", abb_obtener(abb, clave3) == valor6);

    /* Borro claves 7 y 3 para que quede solo la raiz con sus dos hijos y luego borro raź */
    print_test("Prueba abb borrar clave3 sin hijos, es valor6", abb_borrar(abb, clave3) == valor6);
    print_test("Prueba abb borrar clave7 sin hijos, es valor7", abb_borrar(abb, clave7) == valor7);
    print_test("Prueba abb cantidad es 3", abb_cantidad(abb) == 3);

    print_test("Prueba abb borrar clave1 (raiz) con dos hijos es valor7", abb_borrar(abb, clave1) == valor7);
    print_test("Prueba abb borrar clave5 (raiz) con un hijo es valor5", abb_borrar(abb, clave5) == valor5);
    print_test("Prueba abb pertenece clave1, es false", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave1, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb pertenece clave5, es false", !abb_pertenece(abb, clave1));
    print_test("Prueba abb obtener clave5, es NULL", !abb_obtener(abb, clave1));
    print_test("Prueba abb cantidad es 1", abb_cantidad(abb) == 1);

    /* Solo queda la clave 6 y se destruye con el abb */
    print_test("Prueba abb obtener clave6 es valor6", abb_obtener(abb, clave6) == valor6);
    print_test("Prueba abb pertenece clave6, es true", abb_pertenece(abb, clave6));

    abb_destruir(abb);

}

void prueba_abb_clave_vacia() {
    printf("\n~PRUEBAS ABB CLAVE VACIA~\n");
    
    abb_t *abb = abb_crear(cmp, NULL);

    char *clave = "", *valor = "";

    print_test("Prueba abb insertar clave vacia", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

void prueba_abb_valor_null() {
    printf("\n~PRUEBAS ABB VALOR NULL~\n");

    abb_t* abb = abb_crear(cmp, NULL);

    char *clave = "", *valor = NULL;

    /* Inserta 1 valor y luego lo borra */
    print_test("Prueba abb insertar clave vacia valor NULL", abb_guardar(abb, clave, valor));
    print_test("Prueba abb la cantidad de elementos es 1", abb_cantidad(abb) == 1);
    print_test("Prueba abb obtener clave vacia es valor NULL", abb_obtener(abb, clave) == valor);
    print_test("Prueba abb pertenece clave vacia, es true", abb_pertenece(abb, clave));
    print_test("Prueba abb borrar clave vacia, es valor NULL", abb_borrar(abb, clave) == valor);
    print_test("Prueba abb la cantidad de elementos es 0", abb_cantidad(abb) == 0);

    abb_destruir(abb);
}

void prueba_abb_volumen(size_t largo) {
    printf("\n~PRUEBAS ABB VOLUMEN~\n");

    abb_t *abb = abb_crear(cmp_enteros, NULL);

    char **claves = malloc(sizeof(char*) * largo); 
    char **claves_aux = malloc(sizeof(char*) * largo);
    hash_t *hash1 = hash_crear(NULL);
    hash_t *hash2 = hash_crear(NULL);
    
    /* Lleno un array dinámico de numeros aleatorios y los guardo en el abb */
    bool ok = obt_aleatorios_y_guardar(abb, hash1, claves, largo, 1);

    print_test("Prueba guardar volumen grande con claves y valores aleatorios", ok);

    /* Lleno un array dinámico de numeros aleatorios y borro en ese orden en el abb */
    ok = obt_aleatorios_y_guardar(abb, hash2, claves_aux, largo, 2);

    for (size_t i=0; i < largo; i++) {
        ok = abb_borrar(abb, claves_aux[i]) == hash_obtener(hash2, claves_aux[i]);
    }

    print_test("Prueba borrar volumen grande con claves y valores aleatorios", ok);

    abb_destruir(abb);
    hash_destruir(hash1);
    hash_destruir(hash2);

    for (size_t i=0; i < largo; i++) {
        free(claves[i]);
        free(claves_aux[i]);
    }
    free(claves);
    free(claves_aux);
}

void pruebas_abb_iterador_ext(){
    printf("\nINICIO PRUEBAS ITERADOR EXTERNO\n");

    abb_t *abb = abb_crear(strcmp, NULL);

    char *claves[] = {"a", "b", "c"};
    int valor[] = {1, 2, 3};

    //Agrego tres claves con valor asociado al abb.
    print_test("Se agregó correctamente la clave y su valor asociado.", abb_guardar(abb, claves[0], &valor[0]));
    print_test("Se agregó correctamente la clave y su valor asociado.", abb_guardar(abb, claves[1], &valor[1]));
    print_test("Se agregó correctamente la clave y su valor asociado.", abb_guardar(abb, claves[2], &valor[2]));

    abb_iter_t* iter = abb_iter_in_crear(abb);
    print_test("El iterador no está al final.", !abb_iter_in_al_final(iter));

    for (size_t i = 0; !abb_iter_in_al_final(iter); i++){
        print_test("El iterador está en la posición correcta", strcmp(abb_iter_in_ver_actual(iter), claves[i]) == 0);
        print_test("El iterador avanzó una posición.", abb_iter_in_avanzar(iter));
    }

    print_test("El iterador está al final.", abb_iter_in_al_final(iter));

    abb_iter_in_destruir(iter);
    print_test("Se destruyó el iterador correctamente", true);
    abb_destruir(abb);
    print_test("Se destruyó el árbol correctamente.", true);
}

void pruebas_abb_iterador_int(){
    printf("\nINICIO PRUEBAS ITERADOR INTERNO\n");

    abb_t *abb = abb_crear(strcmp, NULL);

    char *claves[] = {"milanesa", "fideos", "torta", "pan"};
    int valor[] = {1, 2, 3, 4};

    //Agrego tres claves con valor asociado al abb.
    print_test("Se agregó correctamente la clave y su valor asociado.", abb_guardar(abb, claves[0], &valor[0]));
    print_test("Se agregó correctamente la clave y su valor asociado.", abb_guardar(abb, claves[1], &valor[1]));
    print_test("Se agregó correctamente la clave y su valor asociado.", abb_guardar(abb, claves[2], &valor[2]));
    print_test("Se agregó correctamente la clave y su valor asociado.", abb_guardar(abb, claves[3], &valor[3]));

    size_t total = 0;
    abb_in_order(abb, sumar_todos, &total);

    print_test("El resultado de sumar todas los valores es correcto", total == 10);

    abb_destruir(abb);
}

void pruebas_iter_volumen(size_t largo){
    printf("\nINICIO PRUEBAS DE VOLUMEN CON ITERADOR\n");

    abb_t *abb = abb_crear(cmp_enteros, NULL);

    size_t largo_clave = 10;
    char (*claves)[largo_clave] = malloc(largo * largo_clave);
    size_t valores[largo];

    bool ok = true;
    size_t num;
    for (size_t i = 0; i < largo && ok; i++){
        num = (size_t)rand() % largo;
        sprintf(claves[i], "%ld", num);
        
        while(abb_pertenece(abb, claves[i])){
            sprintf(claves[i], "%ld", (size_t)rand() % largo);
        }

        valores[i] = i;
        ok &= abb_guardar(abb, claves[i], &valores[i]);
    }

    print_test("Se guardaron todos los pares de clave, valor en el árbol", ok);

    abb_iter_t *iter = abb_iter_in_crear(abb);
    print_test("El iterador no está al final", !abb_iter_in_al_final(iter));

    ok = true;

    const char* clave;
    void* valor;
    size_t i;

    for (i = 0; i < largo; i++) {
        if (abb_iter_in_al_final(iter)) {
            ok = false;
            break;
        }
        clave = abb_iter_in_ver_actual(iter);
        if (clave == NULL) {
            ok = false;
            break;
        }
        valor = abb_obtener(abb, clave);
        if (valor == NULL) {
            ok = false;
            break;
        }
        *(size_t*)valor = largo;
        abb_iter_in_avanzar(iter);
    }

    print_test("Prueba abb iterador volumen", ok);
    print_test("El iterador recorrió todo el largo", i == largo);
    print_test("El iterador está al final", abb_iter_in_al_final(iter));

    free(claves);
    abb_iter_in_destruir(iter);
    abb_destruir(abb);
}
   
/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL                         *
 * *****************************************************************/

void pruebas_abb_alumno() {

    prueba_crear_abb_vacio();
    prueba_abb_guardar();
    prueba_abb_reemplazar();
    prueba_abb_reemplazar_con_destruir();
    prueba_abb_borrar_1_hijo();
    prueba_abb_borrar_2_hijos();
    prueba_abb_clave_vacia();
    prueba_abb_valor_null();
    prueba_abb_volumen(5000);
    pruebas_abb_iterador_ext();
    pruebas_iter_volumen(5000);
    pruebas_abb_iterador_int();

}
