#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

typedef struct abb abb_t;

typedef struct abb_iter abb_iter_t;

//Función que compara claves
typedef int (*abb_comparar_clave_t) (const char *, const char *);

//Función de destrucción de dato
typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *                       PRIMITIVAS DEL ABB                        *
 * *****************************************************************/

//Crea un ABB vacío. Recibe por parametro la función de comparación y de destrucción del dato.
//Post: se crea un abb vacío.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

//Guarda una clave y un dato en el ABB. Devuelve verdadero si se pudieron guardar con éxito, en caso contrario, falso.
//En caso de que la clave ya esté guardada, se reemplaza el dato por el nuevo pasado por parámetro.
//Pre: el abb fue creado.
//Post: el abb tiene una nueva clave y dato.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

//Borra una clave del abb. Devuelve el valor del dato de esa clave borrada.
//Pre: el abb fue creado.
//Post: el abb tiene una clave menos.
void *abb_borrar(abb_t *arbol, const char *clave);

//Devuelve el dato asociado a la clave pasada por parámetro.
//Pre: el abb fue creado.
//Post: se devolvió el dato.
void *abb_obtener(const abb_t *arbol, const char *clave);

//Devuelve verdadero en caso de que una clave pertenezca al abb, y en caso contrario, falso.
//Pre: el abb fue creado.
bool abb_pertenece(const abb_t *arbol, const char *clave);

//Devuelve la cantidad de claves insertadas en el abb.
//Pre: el abb fue creado.
size_t abb_cantidad(abb_t *arbol);

//Destruye el abb y en caso de que exista función de destrucción, destruye todos los datos del mismo.
//Pre: el abb fue creado.
//Post: el abb fue destruido.
void abb_destruir(abb_t *arbol);

//Iterador intero del abb

//Recorre al abb en orden in order y se le aplica la función visitar en cada clave, dato.
//Pre: el abb fue creado.
//Post: se aplicó la función visitar.
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


//Iterador externo del abb

//Crea un iterador externo del abb.
//Pre: el abb fue creado.
//Post: se creó un iterador externo en la primera posición del abb.
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//Avanza un lugar en el iterador del abb. Devuelve verdadero si puede avanzar y falso, en caso contrario.
//Pre: el abb y su iterador externo fueron creados.
//Post: avanza una posición.
bool abb_iter_in_avanzar(abb_iter_t *iter);

//Devuelve la clave en donde se encuentra el iterador.
//Pre: el abb y su iteradon externo fueron creados.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

//Devuelve verdadero si el iterador llegó al último elemento del abb, en caso contrario, falso.
//Pre: el abb y su iterador externo fueron creados.
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Destruye el iterador externo del abb.
//Pre: el abb y su iterador externo fueron creados.
//Post: se destruyó el iterador externo del abb.
void abb_iter_in_destruir(abb_iter_t* iter);

#endif