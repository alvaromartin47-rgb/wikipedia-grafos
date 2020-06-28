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
    size_t pos_iter;
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
void iniciar_tabla(hash_t *hash){
    for(size_t i = 0; i < hash->capacidad; i++){
        hash->tabla[i].clave = NULL;
        hash->tabla[i].valor = NULL;
        hash->tabla[i].estado = VACIO;
    }
    hash->cantidad = 0;
    hash->borrados = 0;
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
            if (!hash_guardar(hash, tabla_ant[i].clave, tabla_ant[i].valor)) return false;
            free(tabla_ant[i].clave);
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
    iniciar_tabla(hash);
    
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
estados_t resolver_colision(const hash_t *hash, const char *clave, size_t *pos, size_t tipo) {
    estados_t estado = hash->tabla[*pos].estado;
    
    while (estado != VACIO) {
        // Si me llamó guardar y el estado actual es borrado devuelvo borrado para insertar alli.
        if (tipo == 1 && estado == BORRADO) return BORRADO;
        
        // De lo contrario sin importar cual llamó si la clave actual es igual a la clave inicial
        // devuelvo un ocupado, si llamó guardar debo reemplazar el valor en esa celda y mantener
        // la clave, si me llamó buscar quiere decir que la clave fué hallada y debo devolver su valor.
        if (hash->tabla[*pos].clave && strcmp(hash->tabla[*pos].clave, clave) == 0) return OCUPADO;
        
        // Actualizo la posición, verifico si es igual a la capacidad para comenzar a iterar desde
        // el principio del arreglo, y actualizo el estado.
        *pos = *pos + 1;
        if (*pos == hash->capacidad) *pos = 0;
        estado = hash->tabla[*pos].estado;
    }

    // Si llega acá, quiere decir que el estado actual es vacio, por lo tanto lo devuelvo y, si me
    // llamó guardar significa que debo guardar en esta posición y si me llamó buscar quiere decir que
    // no se encontro la clave y debo devolver NULL.
    return VACIO;
}

/*
Recibe un hash, una clave (string), un dato, una posición, y un estado.
Dependiendo el valor de _estado_ modifica una celda de la tabla de _hash_,
en ambos casos se actualiza el estado de la celda.
Pre: el hash fue creado, _pos_ es una posición valida de la tabla de _hash_.
Post: se moficó una celda y su estado cambió  a _estado_.
*/
void modif_celda(hash_t *hash, char *clave, void *dato, size_t pos, estados_t estado) {
    celda_t *celda = &hash->tabla[pos];
    
    if (estado == BORRADO) {
        free(celda->clave);
        hash->cantidad--;
        hash->borrados++;
        celda->clave = NULL;
        celda->valor = NULL;
        
    } else {
        if (celda->estado == OCUPADO) {
            free(hash->tabla[pos].clave);
        }
        else {
            hash->cantidad++;
        }
        celda->clave = clave;
        celda->valor = dato;
    }
    
    celda->estado = estado;
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
        estado = resolver_colision(hash, clave, pos, 2);
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

    hash->capacidad = CAPACIDAD_MINIMA;
    iniciar_tabla(hash);
    hash->destructor_dato = destruir_dato;
    
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if (!clave) return NULL;
    
    if ((double)(hash->cantidad + hash->borrados) / (double)hash->capacidad >= FACTOR_CARGA) {
        if (!hash_redimensionar(hash, hash->capacidad * 2)) return false;
    }

    char *clave_copia = strdup(clave);
    if (!clave_copia) return false;
    
    size_t pos = obtener_posicion(hash, clave);
    estados_t estado = hash->tabla[pos].estado;
    
    if (estado == OCUPADO) estado = resolver_colision(hash, clave_copia, &pos, 1);
    
    if (estado == OCUPADO) {
        if (hash->destructor_dato) hash->destructor_dato(hash->tabla[pos].valor);
    }
    
    modif_celda(hash, clave_copia, dato, pos, OCUPADO);

    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    size_t pos = obtener_posicion(hash, clave);

    void *dato = buscar_clave(hash, clave, &pos);

    if (hash->tabla[pos].estado == VACIO) return NULL;

    modif_celda(hash, NULL, dato, pos, BORRADO);

    if ((double)(hash->cantidad + hash->borrados) / (double)hash->capacidad <= (FACTOR_CARGA * 0.10)) {
        if (!hash_redimensionar(hash, hash->capacidad / 2)) return false;
    }

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
    for (size_t i=0; i < hash->capacidad; i++) {
        if (hash->tabla[i].estado == OCUPADO) {
            void *valor = hash_borrar(hash, hash->tabla[i].clave);
            if (hash->destructor_dato) hash->destructor_dato(valor);
        }
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

    if (hash->cantidad == 0) iter->pos_iter = iter->hash->capacidad;
    else if (iter->hash->tabla[0].estado == OCUPADO) iter->pos_iter = 0;
    else {
        iter->pos_iter = 0;
        hash_iter_avanzar(iter);
    }
    
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    bool ok = true;

    if (hash_iter_al_final(iter)) ok = false;
    
    if (ok) {
        while (ok) {
            iter->pos_iter++;
            if (hash_iter_al_final(iter)) ok = false;
            else if (iter->hash->tabla[iter->pos_iter].estado == OCUPADO) break;    
        }
    }

    return ok;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    return hash_iter_al_final(iter) ? NULL : iter->hash->tabla[iter->pos_iter].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->pos_iter == iter->hash->capacidad ? true : false;
}

void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}

/*********************************************************************/