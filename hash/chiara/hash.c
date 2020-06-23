#define _POSIX_C_SOURCE 200809L
#include "hash.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CAPACIDAD_MINIMA 97
#define FACTOR_CARGA_MAX 0.7
#define FACTOR_CARGA_MIN 0.7 * 0.10
typedef enum{VACIO, OCUPADO, BORRADO} estado_t;

typedef struct celda{
    char* clave;
    void* valor;
    estado_t estado;
} celda_t;

struct hash{
    celda_t* tabla;
    size_t capacidad;
    size_t cantidad;
    size_t borrados;
    hash_destruir_dato_t destructor_dato;
};

//FUNCIONES AUXILIARES

//función by djb2
size_t funcion_hash(const char* clave){
    size_t hash = 5381;
    int c;
    
    while((c = *clave++)){
        hash = ((hash << 5) + hash) + c;
    }
            
    return hash;
}

void iniciar_tabla(celda_t* tabla, size_t capacidad){
    for(size_t i = 0; i < capacidad; i++){
        tabla[i].clave = NULL;
        tabla[i].valor = NULL;
        tabla[i].estado = VACIO;
    }
}

size_t arreglar_colision(const hash_t* hash, const char* clave){
    size_t pos;
    for(size_t i = 1; i < hash->capacidad; i++){
        pos = (funcion_hash(clave) + i) % hash->capacidad;

        if(hash->tabla[pos].estado != OCUPADO || hash->tabla[pos].clave == clave){
            break;
        }
    }
    return pos;
}

size_t obtener_posicion(const hash_t* hash, const char* clave){
    size_t pos = funcion_hash(clave) % hash->capacidad;
    
    if(hash->tabla[pos].estado != OCUPADO || hash->tabla[pos].clave == clave){
        return pos;
    } else{
        return arreglar_colision(hash, clave);
    }
}

bool crear_nueva_tabla(hash_t* hash, celda_t* tabla_ant, size_t capacidad_ant){
    for(size_t i = 0; i < capacidad_ant; i++){
        if(tabla_ant[i].estado == OCUPADO){
            size_t pos = obtener_posicion(hash, tabla_ant[i].clave);

            if(pos < hash->capacidad){
                return false;
            }
            hash->tabla[pos].clave = tabla_ant[i].clave;
            hash->tabla[pos].valor = tabla_ant[i].valor;
            hash->tabla[pos].estado = OCUPADO;
        }
    }
    return true;
}

bool hash_redimensionar(hash_t *hash, size_t nueva_capacidad){
    celda_t* tabla_ant = hash->tabla;

    hash->tabla = malloc(sizeof(celda_t) * nueva_capacidad);
    if(!hash->tabla){
        hash->tabla = tabla_ant;
        return false;
    }

    size_t capacidad_ant = hash->capacidad;
    hash->capacidad = nueva_capacidad;
    if(!crear_nueva_tabla(hash, tabla_ant, capacidad_ant)){
        free(hash->tabla);
        hash->tabla = tabla_ant;
        hash->capacidad = capacidad_ant;
        return false;
    }

    free(tabla_ant);
    return true;
}

//PRIMITIVAS

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t* hash = malloc(sizeof(hash_t));
    if(!hash){
        return NULL;
    }

    hash->tabla = malloc(sizeof(celda_t) * CAPACIDAD_MINIMA);
    if(!hash->tabla){
        return NULL;
    }

    iniciar_tabla(hash->tabla, CAPACIDAD_MINIMA);

    hash->cantidad = 0;
    hash->borrados = 0;
    hash->capacidad = CAPACIDAD_MINIMA;
    if(!destruir_dato){
        hash->destructor_dato = NULL;
    } else{
        hash->destructor_dato = destruir_dato;
    }
    
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    if(!clave || !dato){
        return false;
    }
    
    char* copia_clave = strdup(clave);
    if(!copia_clave){
        return false;
    }

    if((double)hash->cantidad / (double)hash->cantidad >= FACTOR_CARGA_MAX){
        if(!hash_redimensionar(hash, hash->capacidad * 2)){
            return false;
        }
    }

    size_t pos = obtener_posicion(hash, clave);
    if(pos > hash->capacidad){
        return false;
    }

    if(hash->tabla[pos].estado == OCUPADO && hash->destructor_dato){
        hash->destructor_dato(hash->tabla[pos].valor);
    } else{
        hash->tabla[pos].clave = copia_clave;
    }

    hash->tabla[pos].valor = dato;
    hash->tabla[pos].estado = OCUPADO;
    hash->cantidad++;

    return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if((double)hash->cantidad / (double)hash->capacidad <= FACTOR_CARGA_MIN){
        if(!hash_redimensionar(hash, hash->capacidad / 2)){
            return false;
        }
    }
    size_t pos = obtener_posicion(hash, clave);
    if(hash->tabla[pos].estado == VACIO){
        return NULL;
    }

    hash->tabla[pos].estado = BORRADO;
    hash->borrados++;
    hash->cantidad--;
    free(hash->tabla[pos].clave);
    return hash->tabla[pos].valor;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    size_t pos = obtener_posicion(hash, clave);
    if(hash->tabla[pos].estado != OCUPADO){
        return NULL;
    }
    return hash->tabla[pos].valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t pos = obtener_posicion(hash, clave);

    if(hash->tabla[pos].estado != OCUPADO){
        return false;
    }
    return true;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){
    for(size_t i = 0; i < hash->cantidad; i++){
        void* valor = hash_borrar(hash, hash->tabla[i].clave);
        free(hash->tabla[i].clave);
        if(hash->destructor_dato){
            hash->destructor_dato(valor);
        }
    }
    free(hash->tabla);
    free(hash);
}