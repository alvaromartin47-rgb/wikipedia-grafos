#define _POSIX_C_SOURCE 200809L
#include "hash.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*********************CONSTANTES Y DEFINICIONES*********************/

#define CAPACIDAD_MINIMA 97
#define FACTOR_CARGA 0.7
typedef enum estados {VACIO, OCUPADO, BORRADO} estados_t;

/************************DECLARACIONES*****************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato);
bool hash_guardar(hash_t *hash, const char *clave, void *dato);
void *hash_borrar(hash_t *hash, const char *clave);
void *hash_obtener(const hash_t *hash, const char *clave);
bool hash_pertenece(const hash_t *hash, const char *clave);
size_t hash_cantidad(const hash_t *hash);
void hash_destruir(hash_t *hash);

hash_iter_t *hash_iter_crear(const hash_t *hash);
bool hash_iter_al_final(const hash_iter_t *iter);
const char *hash_iter_ver_actual(const hash_iter_t *iter);
void hash_iter_destruir(hash_iter_t* iter);

/*******************************************************************/

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
    size_t pos_alan;
    size_t pos_barbara;
};

/*******************************************************************/

/* *****************************************************************
 *                        FUNCIONES AUXILIARES                     *
 * *****************************************************************/

/*
Funcion de hash llamada djb2.
*/
size_t funcion_hash(const char *clave){
    size_t hash = 5381;
    size_t c;
    
    while((c = (size_t)*clave++)){
        hash = ((hash << 5) + hash) + c;
    }
            
    return hash;
}

/*
Inicializa una tabla de hash, un conjunto de structs que tendran tres miembros.
Se inician _capacidad_ de structs, todos en estado VACIO y su clave y valor apuntando
a NULL.
*/
void iniciar_tabla(celda_t *tabla, size_t capacidad){
    for(size_t i = 0; i < capacidad; i++){
        tabla[i].clave = NULL;
        tabla[i].valor = NULL;
        tabla[i].estado = VACIO;
    }
}

/*
Recibe un hash y una clave (string). Aplica la función de hash a la clave y luego el
modulo de la capacidad del hash. Devuelve un indice que se encuentra en el rango
[0, _capacidad_ - 1].
Pre: el hash fue creado.
Post se devolvio un numero entre [0, _capacidad_ - 1].
*/
size_t obtener_posicion(const hash_t *hash, const char *clave){
    return funcion_hash(clave) % hash->capacidad;
}

/*
Recibe un hash, una tabla de hash, y su capacidad (estas dos ultimas no pertenecen al
hash).
Recorre la tabla de hash y en cada posición que se encuentre en OCUPADO, toma su clave
y valor y aplica hash_guardar en el hash.
Pre: el hash fue creado, todas las claves de _tabla_ant_ son distintas.
Post: en el hash no se guardaron claves repetidas.
*/
bool reasignar_pos(hash_t *hash, celda_t *tabla_ant, size_t capacidad_ant){
    for (size_t i = 0; i < capacidad_ant; i++){
        if (tabla_ant[i].estado == OCUPADO) {
            if (!hash_guardar(hash, hash->tabla[i].clave, hash->tabla[i].valor)) return false;
        }
    }
    return true;
}

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
bool hash_redimensionar(hash_t *hash, size_t nueva_capacidad){
    celda_t *tabla_ant = hash->tabla;

    hash->tabla = malloc(sizeof(celda_t) * nueva_capacidad);
    if(!hash->tabla){
        hash->tabla = tabla_ant;
        return false;
    }

    size_t capacidad_ant = hash->capacidad;
    hash->capacidad = nueva_capacidad;
    iniciar_tabla(hash->tabla, hash->cantidad);
    
    if (!reasignar_pos(hash, tabla_ant, capacidad_ant)) {
        free(hash->tabla);
        hash->tabla = tabla_ant;
        hash->capacidad = capacidad_ant;
        return false;
    }

    free(tabla_ant);
    return true;
}

/*
Recibe un hash una clave (string) un puntero a una posicion perteneciente a la tabla
de _hash_ y uno a un estado. 
Avanza la posición mientras que haya celdas vacias o bien se encuentre
vacia pero su clave sea igual a _clave_, en el hash. Actualiza el estado correspondiente
segun las situaciones mencionadas anteriormente.
Pre: el hash fue creado, _pos_ es una posición valida de la tabla de _hash_.
Post: se actualizó el estado. 
*/
void resolver_colision(const hash_t *hash, const char *clave, size_t *pos, estados_t *estado) {
    //printf("pos antes del while: %ld\n", *pos);
    while (hash->tabla[*pos].estado != VACIO && strcmp(hash->tabla[*pos].clave, clave) != 0) {
        if (*pos == hash->cantidad - 1) *pos = (size_t)-1;
        *pos = *pos + 1;
        //printf("pos en el while: %ld\n", *pos);
    }        

    //printf("pos fuera del while: %ld\n", *pos);
    if (hash->tabla[*pos].estado == VACIO) *estado = VACIO;
    else if (hash->tabla[*pos].estado == BORRADO) *estado = BORRADO;
    else *estado = OCUPADO;
}

/*
Recibe un hash, una clave (string), un dato, una posición, y un estado.
Dependiendo el valor de _estado_ modifica una celda de la tabla de _hash_,
en ambos casos se actualiza el estado de la celda.
Pre: el hash fue creado, _pos_ es una posición valida de la tabla de _hash_.
Post: se moficó una celda y su estado cambió  a _estado_.
*/
void modif_celda(hash_t *hash, char *clave, void *dato, size_t pos, estados_t estado) {
    if (estado == BORRADO) {
        free(hash->tabla[pos].clave);
        hash->cantidad--;
        hash->borrados++;
    } else {
        if (clave) {
            hash->tabla[pos].clave = clave;
            hash->cantidad++;
        }
        hash->tabla[pos].valor = dato;
        printf("Cantidad es: %ld\n", hash->cantidad);
        printf("Borrados es: %ld\n", hash->borrados);
    }
    
    hash->tabla[pos].estado = estado;
}

/*
Recibe un hash, una clave (string) y una posición. 
Busca _clave_ dentro de la tabla de _hash_, si la encuentra devuelve
el dato asociado, de lo contrario NULL.
Pre: el hash fue creado, _pos_ es una posición valida de la tabla de _hash_.
Post: si se encontró _clave_ en la tabla se devolvió su valor. De lo contrario
se devolvió NULL.
*/
void *buscar_clave(const hash_t *hash, const char *clave, size_t *pos) {
    estados_t estado = hash->tabla[*pos].estado;
    if (estado != VACIO) {
        resolver_colision(hash, clave, pos, &estado);
        if (estado == OCUPADO) return hash->tabla[*pos].valor;
    }
    
    return NULL;
}

/************************DEFINICIONES*******************************/

/* *****************************************************************
 *                        PRIMITIVAS HASH                          *
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t *hash = malloc(sizeof(hash_t));
    if(!hash) return NULL;

    hash->tabla = malloc(sizeof(celda_t) * CAPACIDAD_MINIMA);
    if(!hash->tabla) return NULL;

    iniciar_tabla(hash->tabla, CAPACIDAD_MINIMA);

    hash->cantidad = 0;
    hash->borrados = 0;
    hash->capacidad = CAPACIDAD_MINIMA;
    hash->destructor_dato = destruir_dato;
    
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if ((double)(hash->cantidad + hash->borrados) / (double)hash->capacidad >= FACTOR_CARGA) {
        if (!hash_redimensionar(hash, hash->capacidad * 2)) return false;
    }

    char *clave_copia = strdup(clave);
    if (!clave_copia) return false;
    
    size_t pos = obtener_posicion(hash, clave);
    estados_t estado = hash->tabla[pos].estado;
    
    if (estado == OCUPADO) resolver_colision(hash, clave_copia, &pos, &estado);
    
    if (estado == OCUPADO) {
        if (hash->destructor_dato) hash->destructor_dato(hash->tabla[pos].valor);
        modif_celda(hash, NULL, dato, pos, OCUPADO);
    
    } else modif_celda(hash, clave_copia, dato, pos, OCUPADO);

    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    size_t pos = obtener_posicion(hash, clave);

    void *dato = buscar_clave(hash, clave, &pos);
    if (!dato && hash->tabla[pos].estado == VACIO) return NULL;
    
    modif_celda(hash, NULL, dato, pos, BORRADO);
    return dato;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    size_t pos = obtener_posicion(hash, clave);
    return buscar_clave(hash, clave, &pos);
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    size_t pos = obtener_posicion(hash, clave);
    return !buscar_clave(hash, clave, &pos) && hash->tabla[pos].estado == VACIO ? false : true;
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
    size_t i = 0;
    while (i < hash->capacidad) {
        if (hash->tabla[i].estado == OCUPADO) {
            void *valor = hash_borrar(hash, hash->tabla[i].clave);
            if (hash->destructor_dato && valor) hash->destructor_dato(valor);
        }
        i++;
    }
    free(hash->tabla);
    free(hash);
}

/* *****************************************************************
 *                     PRIMITIVAS ITERADOR HASH                    *
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;

    iter->hash = hash;
    iter->pos_alan = 0;
    iter->pos_barbara = 0;
    return iter;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return (iter->pos_barbara == iter->hash->cantidad) ? true : false;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (iter->hash->cantidad == 0) return false;

    while (iter->hash->tabla[iter->pos_alan].estado != OCUPADO) iter->pos_alan++;

    if (hash_iter_al_final(iter)) return false;
    iter->pos_barbara++;
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (iter->hash->tabla[iter->pos_alan].estado == VACIO) return NULL;
    return iter->hash->tabla[iter->pos_alan].clave;
}

void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}

/*********************************************************************/