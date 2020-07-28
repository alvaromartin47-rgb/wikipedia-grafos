#ifndef PERSONA_H
#define PERSONA_H

#include "../hash/hash.h"

/* ******************************************************************
 *                DECLARACION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

/* 
 * Tipo utilizado para cada doctor. 
 */
typedef struct persona persona_t;

/* ******************************************************************
 *                    DECLARACION DE PRIMITIVAS                     *
 * *****************************************************************/

/* 
 * Crea una persona.
 * Recibe:
 * 
 * - Un hash que representa una ficha personal, donde cada clave es un campo
 * que tiene un dato asociado.
 * 
 * - Un string que representa el id de la persona.
 * 
 * Pre: el hash fue creado.
 */
persona_t *persona_crear(hash_t *ficha, char *id);

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
void *persona_obtener_info(persona_t *persona, const char *campo);

/* 
 * Recibe una persona, un conjunto de claves, uno de valores y su largo, itera
 * cada clave y valor y los agrega a la ficha de _persona_. Si ocurre que los
 * valores son digitos los guarda como tipo (int) de lo contrario como (char).
 * Si todas las claves y valores se agregaron correctamente devuelve true, de
 * lo contrario false.
 * Pre: persona fue creado.
 */
bool persona_agregar_info(persona_t *persona, char *claves[], char *valores[], size_t cant);

/*
 * Recibe una persona y lo destruye liberando el lugar que ocupa en memoria.
 * Pre: persona fue creado.
 */
void persona_destruir(persona_t *persona);

/*******************************************************************/

#endif // PERSONA_H
