#define _POSIX_C_SOURCE 200809L
#include "persona.h"
#include "../mensajes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ******************************************************************
 *                DECLARACION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

struct persona {
    hash_t *ficha;
    char *id;
};

/* *****************************************************************
 *                       PRIMITIVAS PERSONA                        *
 * *****************************************************************/

persona_t *persona_crear(hash_t *ficha, char *id) {
    persona_t *persona = malloc(sizeof(persona_t));
    if (!persona) return NULL;

    char *id_copia = strdup(id);
    if (!id_copia) return NULL;

    persona->ficha = ficha;
    persona->id = id_copia;

    return persona;
}

char *persona_obtener_id(persona_t *persona) {
    return persona->id;
}

void *persona_obtener_info(persona_t *persona, const char *campo) {
    return hash_obtener(persona->ficha, campo);
}

bool persona_agregar_info(persona_t *persona, char *claves[], char *valores[], size_t cant) {
    for (size_t i=0; i < cant; i++) {
        int num = atoi(valores[i]);
        if (num != 0 || strcmp(valores[i], "0") == 0) {
            size_t *n_num = malloc(sizeof(size_t));
            if (!n_num) return false;
            *n_num = (size_t)num;
            if (!hash_guardar(persona->ficha, claves[i], n_num)) return false;
        }
        else if (strcmp(claves[i], "Ingreso") == 0) {
            printf(ENOENT_ANIO, valores[i]);
            return false;
        }
        else {
            char *valor = strdup(valores[i]);
            if (!valor || !hash_guardar(persona->ficha, claves[i], valor)) return false;
        }
    }
    return true;
}  

void persona_destruir(persona_t *persona) {
    free(persona->id);
    hash_destruir(persona->ficha);
    free(persona);
}