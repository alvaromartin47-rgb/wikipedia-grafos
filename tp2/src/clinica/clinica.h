#ifndef CLINICA_H
#define CLINICA_H

#include "../hash/hash.h"
#include "../abb/abb.h"
#include "../persona/persona.h"
#include <stdbool.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

/*
 * Tipo de dato clinica, que representa una clinica donde se atienden
 * pacientes con patologias o problemas cotidianos.
 */
typedef struct clinica clinica_t;

/* *****************************************************************
 *                   PRIMITIVAS DE LA CLINICA                      *
 * *****************************************************************/

/*
 * Crea una clinica que contendra información en distintas estructuras de 
 * datos, adecuadas a ciertas necesidades. 
 * Recibe: 
 * 
 * - Un hash que representa los sectores de la clinica, donde las
 * claves son cada sector o especialidad (ejemplo: neumonología) y el valor
 * una sala donde se atenderá pacientes.
 * 
 * - Un arbol binario de busqueda que contendrá un listado de doctores de la
 * forma (clave=id_doctor, valor=doctor_t).
 * 
 * - Un hash que representa los pacientes inscriptos en la clinica de la forma
 * (clave=id_paciente, valor=paciente_t).
 * 
 * Pre: todos los tda recibidos por parámetro fueron creados.
 */
clinica_t *clinica_crear(hash_t *sectores, abb_t *doctores, hash_t *pacientes);

/*
 * Recibe una clinica y un sector (especialidad), busca el sector en el hash y si
 * lo encuentra devuelve el valor, de lo contrario NULL.
 * Pre: la clinica fue creada.
 */
void *clinica_obtener_sala(clinica_t *clinica, const char *sector);

/*
 * Recibe una clinica y el id de un doctor, busca el id en el abb y si
 * lo encuentra devuelve el valor, de lo contrario NULL.
 * Pre: la clinica fue creada.
 */
void *clinica_obtener_doctor(clinica_t *clinica, const char *id_doctor);

/*
 * Recibe una clinica y el id de un paciente, busca el id en el hash y si
 * lo encuentra devuelve el valor, de lo contrario NULL.
 * Pre: la clinica fue creada.
 */
void *clinica_obtener_paciente(clinica_t *clinica, const char *id_paciente);

/*
 * Recibe una clinica, el id de un paciente, un sector (especialidad) y
 * una prioridad. Si _id_paciente_ pertenece al listado de pacientes de
 * la clinica encola el paciente _id_paciente_ en la sala del sector _sector_ en 
 * una cola segun su prioridad, en ambos casos imprime un mensaje por salida 
 * standard indicando el estado de la petición.
 * Pre: la clinica fue creada.
 */
void clinica_pedir_turno(clinica_t *clinica, const char *id_paciente, const char *sector, const char *prioridad);

/*
 * Recibe una clinica, el id de un doctor. Si _id_doctor_ es válido, de acuerdo a su
 * especialidad atiende al siguiente paciente con mayor prioridad en su sector. 
 * Pre: la clinica fue creada.
 */
void clinica_atender_siguiente(clinica_t *clinica, const char *id_doctor);

/*
 * Recibe una clinica y un rango de id de doctores, genera un informe // falta.
 * Si el rango no es válido no existe alguno de los id o ambos, por lo tanto imprimirá
 * por salida standard un mensaje de error mas el/los id no encontrados. 
 * Pre: la clinica fue creada.
 */
void clinica_crear_informe(clinica_t *clinica, char *id_ini, char *id_fin);

/*
 * Destruye la clinica liberando la memoria dinámica pedida.
 * Pre: la clinica fue creada.
 */
void clinica_destruir(clinica_t *clinica);

#endif // CLINICA_H