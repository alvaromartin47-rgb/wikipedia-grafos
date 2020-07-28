#include "aux.h"

#define CAPACIDAD_MINIMA 97
#define FACTOR_CARGA 0.7

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
    
    if (estado == OCUPADO && hash->destructor_dato) hash->destructor_dato(hash->tabla[pos].valor);
    
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
            free(hash->tabla[i].clave);
            if (hash->destructor_dato) hash->destructor_dato(hash->tabla[i].valor);
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