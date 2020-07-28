#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "src/funciones_tp2.h"
#include "src/strutil.h"
#include "src/mensajes.h"
#include "src/sala/sala.h"
#include "src/clinica/clinica.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

bool es_numero_valido_parametros(char **strv, size_t validos) {
    size_t c_sep = 0;
    for (size_t i=0; strv[i]; i++) c_sep ++;
	return c_sep == validos;
}

void procesar_comando(const char* comando, char **parametros, clinica_t *clinica) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (!es_numero_valido_parametros(parametros, 3)) printf(ENOENT_PARAMS, comando);
		else clinica_pedir_turno(clinica, parametros[0], parametros[1], parametros[2]);
	} 
	else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!es_numero_valido_parametros(parametros, 1)) printf(ENOENT_PARAMS, comando);
		else clinica_atender_siguiente(clinica, parametros[0]);
	} 
	else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (!es_numero_valido_parametros(parametros, 2)) printf(ENOENT_PARAMS, comando);
		else {
			char *inicio = parametros[0];
			char *fin = parametros[1];
			if (strcmp(parametros[0], "") == 0) inicio = NULL;
			if (strcmp(parametros[1], "") == 0) fin = NULL;
			clinica_crear_informe(clinica, inicio, fin);
		}
	} 
	else {
		printf(ENOENT_CMD, comando);
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

	if (argc != 3) {
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}

	hash_t *sectores = cargar_sectores(argv[1]);
	abb_t *doctores = cargar_doctores(argv[1]); 
	hash_t *pacientes = cargar_pacientes(argv[2]); 

	if (!sectores || !doctores || !pacientes) {
		if (sectores) hash_destruir(sectores);
		if (doctores) abb_destruir(doctores);
		if (pacientes) hash_destruir(pacientes);
		return 1;
	}

	clinica_t *clinica = clinica_crear(sectores, doctores, pacientes);
	if (!clinica) return 1;
	
	procesar_entrada(clinica);

	hash_destruir(sectores);
  	abb_destruir(doctores);
  	hash_destruir(pacientes);
	clinica_destruir(clinica);
	
	return 0;
}
