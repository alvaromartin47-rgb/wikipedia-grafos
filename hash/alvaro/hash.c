#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/************CONSTANTES Y DEFINICIONES*************/

#define TAM_HASH 1000
#define FACTOR_CARGA 0.7
typedef enum estados {OCUPADO, BORRADO} estados_t;

/**************************************************/

/* *****************************************************************
 *                      ESTRUCTURAS PRINCIPALES
 * *****************************************************************/

struct hash_iter {
    struct hash *hash;
    size_t pos_alan;
    size_t pos_barbara;
};

struct hash {
    struct celda **tabla;
    size_t ocupadas;
    size_t borradas;
    size_t largo;
    hash_destruir_dato_t destruir_dato;
};

/* *****************************************************************
 *              ESTRUCTURAS AUXILIARES Y SUS PRIMITIVAS
 * *****************************************************************/

typedef struct celda {
    char *clave;
    void *valor;
    estados_t estado;
} celda_t;

celda_t *celda_crear() {
    celda_t *celda = malloc(sizeof(celda_t));
    if (!celda) return NULL;
}

/*********************************************************************/

/* *****************************************************************
 *                        FUNCIONES AUXILIARES
 * *****************************************************************/

size_t calc_hash(const char *str) {
    size_t hash = 5381;
    size_t c;

    while ((c = (size_t)*str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

size_t obtener_indice(const char *clave, size_t largo) {
    return calc_hash(clave) % largo;
}

bool hash_redimensionar(hash_t *hash, size_t nuevo_largo);

/*********************************************************************/

/* *****************************************************************
 *                        PRIMITIVAS HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;

    celda_t **hash_table = calloc(TAM_HASH, sizeof(celda_t));
    if (!hash_table) {
        free(hash);
        return NULL;
    }

    hash->tabla = hash_table;
    hash->ocupadas = 0;
    hash->largo = TAM_HASH;
    hash->destruir_dato = destruir_dato;
    return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    size_t i = obtener_indice(clave, hash->largo);

    if((double)hash->ocupadas + (double)hash->borradas / (double)hash->largo >= FACTOR_CARGA) {
        if(!hash_redimensionar(hash, hash->largo * 2)) return false;
    }
    
    while (hash->tabla[i] && hash->tabla[i]->estado == OCUPADO) {
        if (strcmp(hash->tabla[i]->clave, clave) == 0) {
            if (hash->destruir_dato) hash->destruir_dato(hash->tabla[i]->valor);
            hash->tabla[i]->valor = dato;
            return true;
        }
        i++;
    }

    if (!hash->tabla[i]) {
        celda_t *celda = celda_crear();
        if(!celda) return false;
        hash->tabla[i] = celda;
    
    } else {
        hash->borradas--;
    }
    
    char *copia = strdup(clave);
    hash->tabla[i]->clave = copia;
    hash->tabla[i]->valor = dato;
    hash->tabla[i]->estado = OCUPADO;
    hash->ocupadas++;

    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    // Verificar redimension
    
    size_t i = obtener_indice(clave, hash->largo);
    
    while (hash->tabla[i]) {
        if (strcmp(hash->tabla[i]->clave, clave) == 0) {
            free(hash->tabla[i]->clave);
            void *dato = hash->tabla[i]->valor;
            hash->tabla[i]->valor = NULL;
            hash->tabla[i]->estado = BORRADO;
            hash->ocupadas--;
            return dato;
        }
        i++;
    }

    return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    size_t i = obtener_indice(clave, hash->largo);
    
    while (hash->tabla[i]) {
        if (strcmp(hash->tabla[i]->clave, clave) == 0) {
            return hash->tabla[i]->valor;
        }
        i++;
    }

    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    size_t i = obtener_indice(clave, hash->largo);
    
    while (hash->tabla[i]) {
        if (strcmp(hash->tabla[i]->clave, clave) == 0) {
            return true;
        }
        i++;
    }

    return false;
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->ocupadas;
}

void hash_destruir(hash_t *hash) {
    for (size_t i=0; i < hash->largo; i++) {
        if (hash->tabla[i]) {
            if (hash->destruir_dato) hash->destruir_dato(hash->tabla[i]->valor);
            if (hash->tabla[i]->estado == OCUPADO) free(hash->tabla[i]->clave);
            free(hash->tabla[i]);
        }
    }
    free(hash->tabla);
}

/* *****************************************************************
 *                     PRIMITIVAS ITERADOR HASH
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
    return (iter->pos_barbara == iter->hash->ocupadas) ? true : false;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (iter->hash->ocupadas == 0) return false;

    while (!iter->hash->tabla[iter->pos_alan]) iter->pos_alan++;

    if (hash_iter_al_final(iter)) return false;
    iter->pos_barbara++;
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    return (!iter->hash->tabla[iter->pos_alan]) ? NULL : iter->hash->tabla[iter->pos_alan]->clave; 
}

void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}