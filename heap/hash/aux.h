#ifndef AUX_H
#define AUX_H

#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum estados {VACIO, OCUPADO, BORRADO} estados_t;

/* *****************************************************************
 *                            ESTRUCTURAS                          *
 * *****************************************************************/

typedef struct celda {
    char *clave;
    void *valor;
    estados_t estado;
} celda_t;

struct hash {
    celda_t *tabla;
    size_t capacidad;
    size_t cantidad;
    size_t borrados;
    hash_destruir_dato_t destructor_dato;
};

struct hash_iter {
    const struct hash *hash;
    size_t pos_iter;
};

/*******************************************************************/

/*
Funcion de hash llamada djb2.
*/
size_t funcion_hash(const char *clave);

/*
Inicializa una tabla de hash, un conjunto de structs que tendran tres miembros.
Se inician _capacidad_ de structs, todos en estado VACIO y su clave y valor apuntando
a NULL.
*/
void iniciar_tabla(hash_t *hash);

/*
Recibe un hash y una clave (string). Aplica la función de hash a la clave y luego el
modulo de la capacidad del hash. Devuelve un indice que se encuentra en el rango
[0, _capacidad_ - 1].
Pre: el hash fue creado.
Post se devolvio un numero entre [0, _capacidad_ - 1].
*/
size_t obtener_posicion(const hash_t *hash, const char *clave);

/*
Recibe un hash, una tabla de hash, y su capacidad (estas dos ultimas no pertenecen al
hash).
Recorre la tabla de hash y en cada posición que se encuentre en OCUPADO, toma su clave
y valor y aplica hash_guardar en el hash.
Pre: el hash fue creado, todas las claves de _tabla_ant_ son distintas.
Post: en el hash no se guardaron claves repetidas.
*/
bool reasignar_pos(hash_t *hash, celda_t *tabla_ant, size_t capacidad_ant);

/*
Recibe un hash y una capacidad, aplica una redimensión donde la nueva
capacidad del hash será su capacidad actual multiplicado la nueva capacidad,
si la nueva capacidad se asigno correctamente, hace una copia de las claves
y valores a la nueva tabla de hash, libera la memoria ocupada por la tabla
anterior y devulve true. De lo contrario, false.
Pre: el hash fue creado. La nueva capacidad es mayor a la cantidad de celdas
ocupadas en la tabla a redimensionar.
Post: si devolvio true, el hash tiene una nueva tabla y nueva capacidad con
al menos, cantidad de celdas ocupadas de la tabla anterior. Si devolvió false
todo se mantiene como estaba. 
*/
bool hash_redimensionar(hash_t *hash, size_t nueva_capacidad);

/*
Recibe un hash, una clave (string), un puntero a una posicion perteneciente a la tabla
de _hash_ y un tipo de colisión. 
Dependiendo el tipo de colision devolvera un estado.
Los tipos de colisión pueden ser dos:
Tipo 1: al intentar guardar.
Tipo 2: al intentar buscar.
Pre: el hash fue creado, _pos_ es una posición valida de la tabla de _hash_.
Post: se devolvio un estado, si el tipo fue uno devolvio VACIO, OCUPADO o BORRADO,
si el tipo fue dos devolvió VACIO u OCUPADO.
*/
estados_t resolver_colision(const hash_t *hash, const char *clave, size_t *pos, size_t tipo);

/*
Recibe un hash, una clave (string), un dato, una posición, y un estado.
Dependiendo el valor de _estado_ modifica una celda de la tabla de _hash_,
en ambos casos se actualiza el estado de la celda.
Pre: el hash fue creado, _pos_ es una posición valida de la tabla de _hash_.
Post: se moficó una celda y su estado cambió  a _estado_.
*/
void modif_celda(hash_t *hash, char *clave, void *dato, size_t pos, estados_t estado);

/*
Recibe un hash, una clave (string) y una posición. 
Busca _clave_ dentro de la tabla de _hash_, si la encuentra devuelve
el dato asociado, de lo contrario NULL.
Pre: el hash fue creado, _pos_ es una posición valida de la tabla de _hash_.
Post: si se encontró _clave_ en la tabla se devolvió su valor. De lo contrario
se devolvió NULL.
*/
void *buscar_clave(const hash_t *hash, const char *clave, size_t *pos);

#endif // AUX_H