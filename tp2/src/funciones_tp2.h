#ifndef F_TP2_H
#define F_TP2_H

#include "hash/hash.h"
#include "abb/abb.h"

/*
 * Recibe la ruta de un archivo de doctores de la forma 
 * DOCTOR,ESPECIALIDAD y devuelve un hash donde las claves son
 * cada especialidad y el valor es una struct sala_t que representa
 * la sala para ese sector o especialidad. En caso de error devuelve
 * NULL.
 */
hash_t *cargar_sectores(char *ruta_archivo);

/*
 * Recibe la ruta de un archivo de doctores de la forma 
 * DOCTOR,ESPECIALIDAD y devuelve un abb donde las claves son
 * cada doctor y el valor es una struct persona_t que contiene
 * datos del doctor. En caso de error devuelve NULL.
 */
abb_t *cargar_doctores(char *ruta_archivo);

/*
 * Recibe la ruta de un archivo de pacientes de la forma 
 * PACIENTE,AÑO y devuelve un hash donde las claves son
 * cada paciente y el valor es una struct persona_t que contiene
 * datos del paciente. En caso de error devuelve NULL.
 */
hash_t *cargar_pacientes(char *ruta_archivo);

#endif // F_TP2_H
