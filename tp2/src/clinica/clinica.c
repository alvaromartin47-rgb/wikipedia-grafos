#include "clinica.h"
#include "../sala/sala.h"
#include "../persona/persona.h"
#include "../mensajes.h"
#include <string.h>

/* *****************************************************************
 *                      FUNCIONES AUXILIARES                       *
 * *****************************************************************/

bool contar_doctores(const char *clave, void *valor, void *extra) {
    size_t *cant = extra;
    *cant = *cant + 1;
    return true;
}

bool imprimir_informe(const char *clave, void *valor, void *extra) {
    size_t *cant = extra;
    *cant = *cant + 1;

    char *especialidad = persona_obtener_info(valor, "Especialidad");
    size_t *atendidos = persona_obtener_info(valor, "Atendidos");

    printf(INFORME_DOCTOR, *cant, clave, especialidad, *atendidos);

    return true;
}

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

struct clinica {
    hash_t *sectores;
    abb_t *doctores;
    hash_t *pacientes;
};

/* *****************************************************************
 *                   PRIMITIVAS DE LA CLINICA                      *
 * *****************************************************************/

clinica_t *clinica_crear(hash_t *sectores, abb_t *doctores, hash_t *pacientes) {
    clinica_t *clinica = malloc(sizeof(clinica_t));
    if (!clinica) return NULL;

    clinica->sectores = sectores;
    clinica->doctores = doctores;
    clinica->pacientes = pacientes;

    return clinica;
}

void *clinica_obtener_sala(clinica_t *clinica, const char *sector) {
    return hash_obtener(clinica->sectores, sector);
}

void *clinica_obtener_doctor(clinica_t *clinica, const char *id_doctor) {
    return abb_obtener(clinica->doctores, id_doctor);
}

void *clinica_obtener_paciente(clinica_t *clinica, const char *id_paciente) {
    return hash_obtener(clinica->pacientes, id_paciente);
}

void clinica_pedir_turno(clinica_t *clinica, const char *id_paciente, const char *sector, const char *prioridad) {

    void *sala = clinica_obtener_sala(clinica, sector);
    void *paciente = clinica_obtener_paciente(clinica, id_paciente);
    
    if (!sala) printf(ENOENT_ESPECIALIDAD, sector);
    else if (!paciente) printf(ENOENT_PACIENTE, id_paciente);
    else if (strcmp(prioridad, "URGENTE") != 0 && strcmp(prioridad, "REGULAR") != 0) {
       printf(ENOENT_URGENCIA, prioridad);
    }
    
    else {
        bool fue_encolado = sala_encolar_paciente(sala, paciente, prioridad);
        
        if (fue_encolado) {
            printf(PACIENTE_ENCOLADO, persona_obtener_id(paciente));
            printf(CANT_PACIENTES_ENCOLADOS, sala_obtener_restantes(sala), sector);
        }
        else printf(ENOENT_ENCOLAR);
    }
}

void clinica_atender_siguiente(clinica_t *clinica, const char *id_doctor) {
    
    void *doctor = clinica_obtener_doctor(clinica, id_doctor);
    if (!doctor) printf(ENOENT_DOCTOR, id_doctor);

    else {
        char *especialidad = persona_obtener_info(doctor, "Especialidad");

        sala_t *sala = clinica_obtener_sala(clinica, especialidad);

        void *atendido = sala_atender_paciente(sala);
        if (!atendido) printf(SIN_PACIENTES);

        else {
            size_t *atendidos = persona_obtener_info(doctor, "Atendidos");
            *atendidos = *atendidos + 1;

            printf(PACIENTE_ATENDIDO, persona_obtener_id(atendido));
            printf(CANT_PACIENTES_ENCOLADOS, sala_obtener_restantes(sala), especialidad);

            hash_borrar(clinica->pacientes, persona_obtener_id(atendido));
            persona_destruir(atendido);
        }
    }
}

void clinica_crear_informe(clinica_t *clinica, char *id_ini, char *id_fin){
    size_t cant = 0;
    abb_in_order(clinica->doctores, id_ini, id_fin, contar_doctores, &cant);
    printf(DOCTORES_SISTEMA, cant);
    cant = 0;
    abb_in_order(clinica->doctores, id_ini, id_fin, imprimir_informe, &cant);
}

void clinica_destruir(clinica_t *clinica) {
    free(clinica);
}
