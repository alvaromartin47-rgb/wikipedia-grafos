#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>
#include "../nodo/nodo.h"


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

/*
Post: Si la lista se pudo crear devuelve un puntero a la lista, de lo contrario
devuelve NULL.
*/
lista_t *lista_crear(void);

/*
Devuelve true si la lista no posee elementos, de lo contrario false.
Pre: La lista fue creada. 
*/
bool lista_esta_vacia(const lista_t *lista);

/*
Recibe un nuevo elemento a agregar a la lista, y lo inserta en la primer posición.
Pre: la lista fue creada.
Post: se agregó un nuevo elemento a la lista, dato es su primer elemento. 
*/
bool lista_insertar_primero(lista_t *lista, void *dato);

/*
Recibe un nuevo elemento a agregar a la lista, y lo inserta en la última posición.
Pre: la lista fue creada.
Post: se agregó un nuevo elemento a la lista, dato es su último elemento.
*/
bool lista_insertar_ultimo(lista_t *lista, void *dato);

/*
Si la lista no se encuentra vacia borra el primer elemento de la lista y devuelve su dato, 
de lo contrario devuelve NULL.
Pre: la lista fue creada.
Post: si la lista no estaba vacía, se borro el primer elemento de la lista y fue devuelto. 
El nuevo primer elemento será el elemento siguiente al borrado.
*/
void *lista_borrar_primero(lista_t *lista);

/*
Si la lista no se encuentra vacia devuelve el primer elemento de la lista, de lo contrario
devuelve NULL.
Pre: la lista fue creada. 
Post: si la lista no estaba vacía, se devolvió el primer elemento de la lista.
*/
void *lista_ver_primero(const lista_t *lista);

/*
Si la lista no se encuentra vacia devuelve el último elemento de la lista, de lo contrario
devuelve NULL.
Pre: la lista fue creada.
Post: si la lista no estaba vacia, se devolvió el último elemento de la lista.
*/
void *lista_ver_ultimo(const lista_t* lista);

/*
Devuelve el largo actual de la lista.
Pre: la lista fue creada.
Post: Se devolvio el largo actual de la lista.
*/
size_t lista_largo(const lista_t *lista);

/*
Destruye la lista. Si se recibe la función destruir_dato por parámetro,
para cada uno de los elementos de la lista llama a destruir_dato.
Pre: la lista fue creada. destruir_dato es una función capaz de destruir
los datos de la lista, o NULL en caso de que no se la utilice.
Post: se eliminaron todos los elementos de la lista.
*/
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *                      PRIMITIVAS DE ITERADORES
 * *****************************************************************/

/* ITERADOR INTERNO */

/*
Recibe una lista, la itera y a cada dato le aplica la funcion pasada por parámetro.
Esta funcion que devuelve un bool marcará hasta donde iterar, si devuelve false la iteración
para, de lo contrario continuará hasta el final. Recibe extra que será aplicado a visitar y
puede tener múltiples usos.
Pre: la lista fue creada, la funcion debe existir y debe devolver bool, si no se utiliza extra se
le debe pasar NULL.
Post: se iteró la lista y se aplicó visitar hasta donde se estableció en la función, si extra
no fue NULL, se aplico extra a visitar.
*/
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ITERADOR EXTERNO */

/*
Crea un iterador de una lista.
Pre: la lista fue creada.
Post: se creo un iterador.
*/
lista_iter_t *lista_iter_crear(lista_t *lista);

/*
Avanza el iterador una posición en la lista, si ya no hay posiciones devuelve false, de lo
contrario true. 
Pre: el iterador fue creado.
Post: se avanzó una posición y devolvió true si aun quedan posiciones, si no false.
*/
bool lista_iter_avanzar(lista_iter_t *iter);

/*
Devuelve el dato que hay en la posicion donde se encuentra el iterador.
Pre: el iterador fue creado.
Post: se devolvio el dato.
*/
void *lista_iter_ver_actual(const lista_iter_t *iter);

/*
Devuelve un bool que significa si el iterador se encuentra en el final o no.
En el final no hay ningun dato guardado.
Pre: el iterador fue creado.
Post: se devolvió true si esta al final, de lo contrario false.
*/
bool lista_iter_al_final(const lista_iter_t *iter);

/*
Destruye el iterador.
Pre: el iterador fue creado.
Post: el iterador fue destruido.
*/
void lista_iter_destruir(lista_iter_t *iter);

/*
Inserta un elemento en la posición donde se encuentra el iterador.
Pre: el iterador fue creado.
Post: se inserto el elemento y si no hubo errores devolvió true.
*/
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

/*
Borra un elemento de la posición donde se encuentra el iterador y lo devuelve.
Pre: el iterador fue creado.
Post: se borró el elemento y fué devuelto.
*/
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/

/*
Realiza pruebas sobre la implementación del alumno.
*/
void pruebas_cola_alumno(void);

#endif // LISTA_H