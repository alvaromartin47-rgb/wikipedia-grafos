#ifndef PERSONA_H
#define PERSONA_H

#include <stdlib.h>
#include "../hash/hash.h"

/* ******************************************************************
 *                DECLARACION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

/* 
 * Tipo utilizado para cada doctor. 
 */
typedef struct persona persona_t;

typedef enum roles {DOCTOR, PACIENTE} roles_t;

/* ******************************************************************
 *                    DECLARACION DE PRIMITIVAS                     *
 * *****************************************************************/

/* 
 * Crea una persona.
 * Recibe:
 * 
 * - Un hash que representa una ficha personal o NULL si no se desea implementar,
 * donde cada clave es un campo que tiene un dato asociado.
 * 
 * - Un string que representa el id de la persona.
 * 
 * Pre: el hash fue creado o es NULL.
 */
persona_t *persona_crear(hash_t *ficha, char *id, roles_t rol, char *ingreso);

/* 
 * Recibe un doctor y devuelve la cantidad de pacientes que lleva
 * atendidos.
 * Pre: _persona_ tiene rol de doctor.
 */
size_t persona_doctor_atendidos(persona_t *doctor);

/* 
 * Recibe una persona y devuelve su id.
 * Pre: persona fue creado.
 */
char *persona_obtener_id(persona_t *persona);

/* 
 * Recibe una persona y un campo, devuelve el dato relacionado a _campo_ que
 * se encuentra en la ficha de _persona_. Si este no se encuentra devuelve NULL.
 * Pre: persona fue creado.
 */
void *persona_obtener_dato(persona_t *persona, char *campo);

/*
 * Recibe una persona y lo destruye liberando el lugar que ocupa en memoria.
 * Pre: persona fue creado.
 */
void persona_destruir(persona_t *persona);

/*******************************************************************/

#endif // PERSONA_H
