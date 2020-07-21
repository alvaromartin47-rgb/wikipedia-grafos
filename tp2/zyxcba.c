#include <stdio.h>
#include <string.h>
#include "src/funciones_tp2.h"
#include "src/strutil.h"
#include "src/mensajes.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void procesar_comando(const char* comando, const char** parametros, clinica_t *clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		// PEDIR_TURNO:NOMBRE_PACIENTE,NOMBRE_ESPECIALIDAD,URGENCIA
		clinica_pedir_turno(clinica, parametros[0], parametros[1], parametros[2]);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		// ATENDER_SIGUIENTE:NOMBRE_DOCTOR
		clinica_atender_siguiente(clinica, parametros[0]);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		// INFORME
		clinica_crear_informe(clinica, id_ini, id_fin);
	} else {

	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(clinica_t *clinica) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, clinica);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}



int main(int argc, char** argv) {
	// Verificar flags y devolver 1 en caso de errores.
	
	hash_t *sectores = cargar_sectores(); // Cargar en memoria cada sector (clave=especialidad, valor=sala)
	abb_t *doctores = cargar_doctores(); // Cargar en memoria cada doctor (clave=id_doctor, valor=persona_t(rol:doctor))
	hash_t *pacientes = cargar_personas(); // Cargar en memoria cada paciente (clave=id_persona, valor=persona_t(rol:paciente))
	
	clinica_t *clinica = clinica_crear(sectores, doctores, pacientes);
	procesar_entrada(clinica);
	
	return 0;
}
