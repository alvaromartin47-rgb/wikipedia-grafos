#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CONSTANTES */

#define TAM_HASH 10
typedef enum estados {OCUPADO, BORRADO} estados_t;

/*----------------------*/

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

unsigned long calc_hash(const char *str) {
    unsigned long hash = 5381;
    unsigned long c;

    while ((c = (unsigned long)*str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

unsigned long obtener_indice(const char *clave, unsigned long largo) {
    return calc_hash(clave) % largo;
}

/*********************************************************************/

struct hash {
    celda_t **tabla;
    size_t ocupadas;
    size_t borradas;
    unsigned long largo;
    hash_destruir_dato_t destruir_dato;
};


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
    unsigned long i = obtener_indice(clave, hash->largo);
    printf("%lu\n", i);

    // Verificar redimension
    
    while (hash->tabla[i] && hash->tabla[i]->estado == OCUPADO) {
        if (hash->tabla[i]->clave == clave) {
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
    
    char *nuevo = calloc((strlen(clave) + 1), sizeof(char));
    if (!nuevo) return false;
    nuevo = clave;
    hash->tabla[i]->clave = nuevo;
    hash->tabla[i]->valor = dato;
    hash->tabla[i]->estado = OCUPADO;
    hash->ocupadas++;

    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    // Verificar redimension
    
    unsigned long i = obtener_indice(clave, hash->largo);
    
    while (hash->tabla[i]) {
        if (hash->tabla[i]->clave == clave) {
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
    unsigned long i = obtener_indice(clave, hash->largo);
    
    while (hash->tabla[i]) {
        if (hash->tabla[i]->clave == clave) {
            return hash->tabla[i]->valor;
        }
        i++;
    }

    return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    unsigned long i = obtener_indice(clave, hash->largo);
    
    while (hash->tabla[i]) {
        if (hash->tabla[i]->clave == clave) {
            return true;
        }
        i++;
    }

    return false;
}

unsigned long hash_cantidad(const hash_t *hash) {
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

// /* Iterador del hash */

// // Crea iterador
// hash_iter_t *hash_iter_crear(const hash_t *hash);

// // Avanza iterador
// bool hash_iter_avanzar(hash_iter_t *iter);

// // Devuelve clave actual, esa clave no se puede modificar ni liberar.
// const char *hash_iter_ver_actual(const hash_iter_t *iter);

// // Comprueba si terminó la iteración
// bool hash_iter_al_final(const hash_iter_t *iter);

// // Destruye iterador
// void hash_iter_destruir(hash_iter_t* iter);