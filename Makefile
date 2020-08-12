# CREADO POR ALVARO MARTIN EN PYTHON :)

CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

# OBJETOS A BORRAR
REMOVE = abb/abb_t/abb.o abb/abb_t/pila.o abb/abb_t/aux.o abb/abb_t/nodo/nodo_abb.o abb/testing/pruebas_alumno_abb.o abb/testing/main_abb.o abb/testing/testing.o abb/testing/tdas-aux/aux.o abb/testing/tdas-aux/hash.o hash/hash_t/hash.o hash/hash_t/aux.o hash/testing/hash_pruebas.o hash/testing/main.o hash/testing/testing.o heap/heap_t/heap.o heap/heap_t/aux.o heap/testing/testing.o heap/testing/pruebas_alumno_heap.o heap/testing/main.o heap/testing/tdas-aux/aux.o heap/testing/tdas-aux/hash.o tp2/zyxcba.o tp2/src/strutil.o tp2/src/clinica/clinica.o tp2/src/hash/aux.o tp2/src/hash/hash.o tp2/src/heap/aux.o tp2/src/heap/heap.o tp2/src/sala/sala.o tp2/src/csv/csv.o tp2/src/cola/cola.o tp2/src/lista/lista.o tp2/src/persona/persona.o tp2/src/abb/aux.o tp2/src/abb/abb.o tp2/src/abb/pila.o tp2/src/nodo/nodo.o tp2/src/abb/nodo/nodo_abb.o tp2/src/funciones_tp2.o


# COMPILACION DE OBJETOS

abb/abb_t/abb.o: abb/abb_t/abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/abb_t/pila.o: abb/abb_t/pila.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/abb_t/aux.o: abb/abb_t/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/abb_t/nodo/nodo_abb.o: abb/abb_t/nodo/nodo_abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/testing/pruebas_alumno_abb.o: abb/testing/pruebas_alumno_abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/testing/main_abb.o: abb/testing/main_abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/testing/testing.o: abb/testing/testing.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/testing/tdas-aux/aux.o: abb/testing/tdas-aux/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb/testing/tdas-aux/hash.o: abb/testing/tdas-aux/hash.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

hash/hash_t/hash.o: hash/hash_t/hash.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

hash/hash_t/aux.o: hash/hash_t/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

hash/testing/hash_pruebas.o: hash/testing/hash_pruebas.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

hash/testing/main.o: hash/testing/main.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

hash/testing/testing.o: hash/testing/testing.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/heap_t/heap.o: heap/heap_t/heap.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/heap_t/aux.o: heap/heap_t/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/testing/testing.o: heap/testing/testing.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/testing/pruebas_alumno_heap.o: heap/testing/pruebas_alumno_heap.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/testing/main.o: heap/testing/main.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/testing/tdas-aux/aux.o: heap/testing/tdas-aux/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

heap/testing/tdas-aux/hash.o: heap/testing/tdas-aux/hash.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/zyxcba.o: tp2/zyxcba.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/strutil.o: tp2/src/strutil.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/clinica/clinica.o: tp2/src/clinica/clinica.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/hash/aux.o: tp2/src/hash/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/hash/hash.o: tp2/src/hash/hash.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/heap/aux.o: tp2/src/heap/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/heap/heap.o: tp2/src/heap/heap.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/sala/sala.o: tp2/src/sala/sala.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/csv/csv.o: tp2/src/csv/csv.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/cola/cola.o: tp2/src/cola/cola.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/lista/lista.o: tp2/src/lista/lista.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/persona/persona.o: tp2/src/persona/persona.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/abb/aux.o: tp2/src/abb/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/abb/abb.o: tp2/src/abb/abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/abb/pila.o: tp2/src/abb/pila.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/nodo/nodo.o: tp2/src/nodo/nodo.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/abb/nodo/nodo_abb.o: tp2/src/abb/nodo/nodo_abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

tp2/src/funciones_tp2.o: tp2/src/funciones_tp2.c
	@- $(CC) -c $(CFLAGS) $^ -o $@


# COMPILACION DE EJECUTABLES

abb_t: abb/abb_t/abb.o abb/abb_t/pila.o abb/abb_t/aux.o abb/abb_t/nodo/nodo_abb.o abb/testing/pruebas_alumno_abb.o abb/testing/main_abb.o abb/testing/testing.o abb/testing/tdas-aux/aux.o abb/testing/tdas-aux/hash.o
	@- $(CC) $(CFLAGS) $^ -o $@

hash_t: hash/hash_t/hash.o hash/hash_t/aux.o hash/testing/hash_pruebas.o hash/testing/main.o hash/testing/testing.o
	@- $(CC) $(CFLAGS) $^ -o $@

heap_t: heap/heap_t/heap.o heap/heap_t/aux.o heap/testing/testing.o heap/testing/pruebas_alumno_heap.o heap/testing/main.o heap/testing/tdas-aux/aux.o heap/testing/tdas-aux/hash.o
	@- $(CC) $(CFLAGS) $^ -o $@

zyxcba: tp2/zyxcba.o tp2/src/strutil.o tp2/src/clinica/clinica.o tp2/src/hash/aux.o tp2/src/hash/hash.o tp2/src/heap/aux.o tp2/src/heap/heap.o tp2/src/sala/sala.o tp2/src/csv/csv.o tp2/src/cola/cola.o tp2/src/lista/lista.o tp2/src/persona/persona.o tp2/src/abb/aux.o tp2/src/abb/abb.o tp2/src/abb/pila.o tp2/src/nodo/nodo.o tp2/src/abb/nodo/nodo_abb.o tp2/src/funciones_tp2.o
	@- $(CC) $(CFLAGS) $^ -o $@


# LIMPIAR

clean:
	@- rm -r $(REMOVE)
