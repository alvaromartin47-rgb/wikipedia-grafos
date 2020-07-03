#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

// Structs a usar.
typedef struct abb abb_t;

// Funcion de comparacion de claves y funcion de destruccion de datos.
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *                     PRIMITIVAS DEL ABB                          *
 * *****************************************************************/

/* 
Crea el abb (Arbol Binario de Búsqueda).
*/
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/*
Recibe un arbol binario de busqueda, una clave y un dato.
Guarda en el abb el dato con su respectiva clave y devuelve true. De lo
contrario false.
Pre: el arbol fue creado.
Post: si devolvió true se guardo el dato con su clave.
*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/*
Recibe un arbol binario de busqueda y una clave.
Borra el dato asociado a _clave_ si es que esta existe y lo devuelve,
si _clave_ no se encuentra en el abb devuelve NULL.
Pre: el arbol fue creado.
Post: si devolvió el dato, éste fue borrado del abb y ahora hay un 
elemento menos.
*/
void *abb_borrar(abb_t *arbol, const char *clave);

/*
Recibe un arbol binario de busqueda y una clave.
Devuelve el dato asociado a _clave_ si es que esta se encuentra, de
lo contrario NULL.
Pre: el arbol fue creado.
Post: el arbol no fue modificado.
*/
void *abb_obtener(const abb_t *arbol, const char *clave);

/*
Recibe un arbol binario de busqueda y una clave.
Devuelve true si _clave_ se encuentra en el abb, de lo contrario
false.
Pre: el arbol fue creado.
Post: el arbol no fue modificado.
*/
bool abb_pertenece(const abb_t *arbol, const char *clave);

/*
Recibe un arbol binario de busqueda.
Devuelve la cantidad de elementos que contiene el abb.
Pre: el arbol fue creado.
Post: se devolvió un numero mayor que cero con la cantidad
de elementos del abb.
*/
size_t abb_cantidad(abb_t *arbol);

/*
Recibe un arbol binario de busqueda.
Destruye el abb y sus elementos.
Pre: el arbol fue creado.
Post: el abb y sus elementos no existen mas en memoria.
*/
void abb_destruir(abb_t *arbol);

#endif // ABB_H
