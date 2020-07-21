#include "persona.h"

/* ******************************************************************
 *                DECLARACION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

struct persona {
    hash_t *ficha;
    char *id;
    size_t atendidos;
    roles_t rol;
    char *ingreso;
};

/* *****************************************************************
 *                       PRIMITIVAS PERSONA                        *
 * *****************************************************************/

persona_t *persona_crear(hash_t *ficha, char *id, roles_t rol, char *ingreso) {
    persona_t *persona = malloc(sizeof(persona_t));
    if (!persona) return NULL;

    persona->ficha = ficha;
    persona->id = id;
    persona->atendidos = 0;
    persona->ingreso = ingreso;

    return persona;
}

size_t persona_doctor_atendidos(persona_t *persona) {
    return persona->atendidos;
}

char *persona_obtener_id(persona_t *persona) {
    return persona->id;
}

void *persona_obtener_dato(persona_t *persona, char *campo) {
    return persona->ficha ? hash_obtener(persona->ficha, campo) : NULL;
}

void persona_destruir(persona_t *persona) {
    free(persona);
}

/*******************************************************************/