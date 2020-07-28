#include "sala.h"
#include "../mensajes.h"
#include <string.h>
#include <stdlib.h>

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

sala_t *sala_crear(cmp_func_t cmp) {
    sala_t *sala = malloc(sizeof(sala_t));
    if (!sala) return NULL;

    heap_t *regulares = heap_crear(cmp); 
    cola_t *urgentes = cola_crear();

    if (!regulares || !urgentes) {
        if (regulares) heap_destruir(regulares, NULL);
        if (urgentes) cola_destruir(urgentes, NULL);
        return NULL;
    }

    sala->urgentes = urgentes;
    sala->regulares = regulares;
    sala->restantes = 0;

    return sala;
}

size_t sala_obtener_restantes(sala_t *sala) {
    return sala->restantes;
}

void *sala_atender_paciente(sala_t *sala) {
    if (cola_esta_vacia(sala->urgentes) && heap_esta_vacio(sala->regulares)) {
        return NULL;
    } 

    void *paciente;
    
    if (cola_esta_vacia(sala->urgentes)) {
        paciente = heap_desencolar(sala->regulares);
    } 
    else {
        paciente = cola_desencolar(sala->urgentes);
    }
    
    if (paciente) sala->restantes--;
    return paciente;
}

bool sala_encolar_paciente(sala_t *sala, persona_t *paciente, const char *prioridad) {
    bool ok = true;
    if (strcmp(prioridad, "URGENTE") == 0) ok = cola_encolar(sala->urgentes, paciente);
    else ok = heap_encolar(sala->regulares, paciente);

    if (ok) sala->restantes++;
    return ok;
}

void sala_destruir(sala_t *sala) {
    cola_destruir(sala->urgentes, NULL);
    heap_destruir(sala->regulares, NULL);
    free(sala);
}