#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

// Structs a usar.
typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;

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

/* *****************************************************************
 *                    ITERADORES Y PRIMITIVAS                      *
 * *****************************************************************/

/* Iterador interno del abb */

/*
Recorre al abb en orden in order y se le aplica la función visitar en cada clave, dato.
Pre: el abb fue creado y la función visitar existe.
Post: se aplicó la función visitar.
*/
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


/* Iterador externo del abb */

/*
Crea un iterador externo del abb.
Pre: el abb fue creado.
Post: se creó un iterador externo en la primera posición del abb.
*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/*
Avanza un lugar en el iterador del abb. Devuelve verdadero si puede avanzar y falso, en caso contrario.
Pre: el abb y su iterador externo fueron creados.
Post: avanza una posición.
*/
bool abb_iter_in_avanzar(abb_iter_t *iter);

/*
Devuelve la clave en donde se encuentra el iterador.
Pre: el abb y su iterador externo fueron creados.
*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/*
Devuelve verdadero si el iterador llegó al último elemento del abb, en caso contrario, falso.
Pre: el abb y su iterador externo fueron creados.
*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/*
Destruye el iterador externo del abb.
Pre: el abb y su iterador externo fueron creados.
Post: se destruyó el iterador externo del abb.
*/
void abb_iter_in_destruir(abb_iter_t* iter);

#endif // ABB_H