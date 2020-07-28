#ifndef SALA_H
#define SALA_H

#include "../heap/heap.h"
#include "../cola/cola.h"
#include "../persona/persona.h"

/* *****************************************************************
 *                DECLARACION DE LOS TIPOS DE DATOS                *
 * *****************************************************************/

/*
 * Tipo de dato sala, que representa una sala de espera.
 */
typedef struct sala sala_t;

/* *****************************************************************
 *                      PRIMITIVAS DE SALA                         *
 * *****************************************************************/

/*
 * Crea una sala de espera que contendrá dos colas, una por orden de
 * llegada para los casos urgentes, y una para los regulares con prioridades
 * (heap de minimos)
 * respecto a la fecha de inscripción en la clinica.
 * Pre: existe la funcion de comparación de prioridades que devuelve 1 si
 * si a < b, 0 si a = b o -1 si a > b.
 * Post: se creó una sala con dos colas y con cero pacientes atendidos.
 */
sala_t *sala_crear(cmp_func_t cmp);

/*
 * Recibe una sala y devuelve la cantidad de pacientes que restan atender
 * en dicha sala.
 * Pre: la sala fue creada.
 */
size_t sala_obtener_restantes(sala_t *sala);

/*
 * Recibe una sala y desencola un paciente. Se tiene prioridad por los urgentes,
 * en caso que no haya urgentes, se atiende a los regulares.
 * Devuelve el paciente desencolado o NULL si no hay pacientes por atender. Aplica 
 * función de destruccion si es que existe.
 * Pre: la sala fue creada.
 */
void *sala_atender_paciente(sala_t *sala);

/*
 * Recibe una sala y encola un paciente dependiendo su prioridad.
 * Pre: la sala fue creada.
 */
bool sala_encolar_paciente(sala_t *sala, persona_t *paciente, const char *prioridad);

/*
 * Recibe una sala, la destruye liberando la memoria dinamica que habia reservado,
 * Pre: la sala fue creada.
 */
void sala_destruir(sala_t *sala);


#endif // SALA_H