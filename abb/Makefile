# CREATED FOR ALVARO MARTIN IN PYTHON :)

CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

# OBJETOS A BORRAR
REMOVE = abb_t/abb.o abb_t/pila.o abb_t/aux.o abb_t/nodo/nodo_abb.o testing/pruebas_alumno_abb.o testing/main_abb.o testing/testing.o testing/tdas-aux/aux.o testing/tdas-aux/hash.o


# COMPILACION DE OBJETOS

abb_t/abb.o: abb_t/abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb_t/pila.o: abb_t/pila.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb_t/aux.o: abb_t/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

abb_t/nodo/nodo_abb.o: abb_t/nodo/nodo_abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/pruebas_alumno_abb.o: testing/pruebas_alumno_abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/main_abb.o: testing/main_abb.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/testing.o: testing/testing.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/tdas-aux/aux.o: testing/tdas-aux/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/tdas-aux/hash.o: testing/tdas-aux/hash.c
	@- $(CC) -c $(CFLAGS) $^ -o $@


# COMPILACION DE EJECUTABLES

abb: abb_t/abb.o abb_t/pila.o abb_t/aux.o abb_t/nodo/nodo_abb.o testing/pruebas_alumno_abb.o testing/main_abb.o testing/testing.o testing/tdas-aux/aux.o testing/tdas-aux/hash.o
	@- $(CC) $(CFLAGS) $^ -o $@


# LIMPIAR

clean:
	@- rm -r $(REMOVE)
