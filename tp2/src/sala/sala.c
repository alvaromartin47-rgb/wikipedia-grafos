#include "sala.h"

/* *****************************************************************
 *               DEFINICION DE LOS TIPOS DE DATOS                  *
 * *****************************************************************/

struct sala {
    cola_t *urgentes;
    heap_t *regulares;
    size_t restantes;
};

/* *****************************************************************
 *                DEFINICION PRIMITIVAS DE SALA                    *
 * *****************************************************************/

sala_t *sala_crear(heap_t *regulares, cola_t *urgentes) {
    sala_t *sala = malloc(sizeof(sala_t));
    if (!sala) return NULL;

    sala->urgentes = urgentes;
    sala->regulares = regulares;
    sala->restantes = 0;

    return sala;
}

size_t sala_obtener_restantes(sala_t *sala) {
    return sala->restantes;
}

persona_t *sala_atender_paciente(sala_t *sala);

void sala_destruir(sala_t *sala) {
    free(sala);
}

/*******************************************************************/