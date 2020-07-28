#include "funciones_tp2.h"

#define _POSIX_C_SOURCE 200809L
#include "heap/heap.h"
#include "csv/csv.h"
#include "sala/sala.h"
#include "mensajes.h"
#include <string.h>

int cmp_antiguedad(const void *pac_a, const void *pac_b) {
    int *a = persona_obtener_info((persona_t*)pac_a, "Ingreso");
    int *b = persona_obtener_info((persona_t*)pac_b, "Ingreso");
    if (*a < *b) return 1;
    if (*a > *b) return -1;
    return 0;
}

int cmp_id(const char *id_a, const char *id_b) {
    return strcmp(id_a, id_b);
}

void *construir_sectores(char **parse, void *sectores) {
    if (hash_pertenece(sectores, parse[1])) return (void*)true;

    sala_t *sala = sala_crear(cmp_antiguedad);
    if (!sala) return NULL;
    
    if (!hash_guardar(sectores, parse[1], sala)) {
        sala_destruir(sala);
        return NULL;
    }

    return sala;
}

void *construir_doctores(char **parse, void *doctores) {

    char *claves_ficha[4] = {"Nombre", "Especialidad", "Rol", "Atendidos"};
    char *valores_ficha[4] = {parse[0], parse[1], "Doctor", "0"};
    
    hash_t *doctor_ficha = hash_crear(free);
    if (!doctor_ficha) return NULL;

    persona_t *doctor = persona_crear(doctor_ficha, parse[0]);
    if (!doctor) return NULL;

    bool ok = abb_guardar(doctores, persona_obtener_id(doctor), doctor);
    bool ok2 = persona_agregar_info(doctor, claves_ficha, valores_ficha, 4);
    
    if (!ok || !ok2) {
        persona_destruir(doctor);
        return NULL;
    }

    return doctor;
}

void *construir_pacientes(char **parse, void *pacientes) {

    char *claves_ficha[3] = {"Nombre", "Ingreso", "Rol"};
    char *valores_ficha[3] = {parse[0], parse[1], "Paciente"};

    hash_t *paciente_ficha = hash_crear(free);
    if (!paciente_ficha) return NULL;
    
    persona_t *paciente = persona_crear(paciente_ficha, parse[0]);
    if (!paciente) return NULL;

    bool ok = hash_guardar(pacientes, persona_obtener_id(paciente), paciente);
    bool ok2 = persona_agregar_info(paciente, claves_ficha, valores_ficha, 3);

    if (!ok || !ok2) {
        persona_destruir(paciente);
        return NULL;
    }
        
    return paciente;
}

void destruir_registro_persona(void *dato) {
    persona_destruir((persona_t*)dato);
}

void destruir_salas(void *dato) {
    sala_destruir((sala_t*)dato);
}

bool lista_validar(void *dato, void *extra) {
    bool *ok = extra;
    if (!dato) *ok = false;
    return *ok;
}

void *procesar_archivo(char *ruta_archivo, void* (*constructor) (char**, void*), void *tda) {
    bool ok = true;

    lista_t *lista = csv_crear_estructura(ruta_archivo, constructor, tda);
    if (!lista) {
        printf(ENOENT_ARCHIVO, ruta_archivo);
        ok = false;
    }
    else {
        lista_iterar(lista, lista_validar, &ok);
        lista_destruir(lista, NULL);
    }

    return ok ? tda : NULL;
}

hash_t *cargar_sectores(char *ruta_archivo) {
    hash_t *sectores = hash_crear(destruir_salas);
    return (hash_t*)procesar_archivo(ruta_archivo, construir_sectores, sectores);
}

abb_t *cargar_doctores(char *ruta_archivo) {
    abb_t *doctores = abb_crear(cmp_id, destruir_registro_persona);
    return (abb_t*)procesar_archivo(ruta_archivo, construir_doctores, doctores);
}

hash_t *cargar_pacientes(char *ruta_archivo) {
    hash_t *pacientes = hash_crear(destruir_registro_persona);
    return (hash_t*)procesar_archivo(ruta_archivo, construir_pacientes, pacientes);
}
