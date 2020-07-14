#include "aux.h"

/* *****************************************************************
 *                        FUNCIONES AUXILIARES                     *
 * *****************************************************************/

size_t funcion_hash(const char *clave) {
    size_t hash = 5381;
    size_t c;
    
    while((c = (size_t)*clave++)){
        hash = ((hash << 5) + hash) + c;
    }
            
    return hash;
}

void iniciar_tabla(hash_t *hash) {
    for(size_t i = 0; i < hash->capacidad; i++){
        hash->tabla[i].clave = NULL;
        hash->tabla[i].valor = NULL;
        hash->tabla[i].estado = VACIO;
    }
    hash->cantidad = 0;
    hash->borrados = 0;
}

size_t obtener_posicion(const hash_t *hash, const char *clave) {
    return funcion_hash(clave) % hash->capacidad;
}

bool reasignar_pos(hash_t *hash, celda_t *tabla_ant, size_t capacidad_ant) {
    for (size_t i = 0; i < capacidad_ant; i++){
        if (tabla_ant[i].estado == OCUPADO) {
            if (!hash_guardar(hash, tabla_ant[i].clave, tabla_ant[i].valor)) return false;
            free(tabla_ant[i].clave);
        }
    }
    return true;
}

bool hash_redimensionar(hash_t *hash, size_t nueva_capacidad) {
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

void *buscar_clave(const hash_t *hash, const char *clave, size_t *pos) {
    estados_t estado = hash->tabla[*pos].estado;
    if (estado != VACIO) {
        estado = resolver_colision(hash, clave, pos, 2);
        if (estado == OCUPADO) return hash->tabla[*pos].valor;
    }
    return NULL;
}

/*******************************************************************/