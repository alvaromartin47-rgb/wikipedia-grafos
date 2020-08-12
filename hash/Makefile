# CREADO POR ALVARO MARTIN EN PYTHON :)

CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

# OBJETOS A BORRAR
REMOVE = hash_t/hash.o hash_t/aux.o testing/hash_pruebas.o testing/main.o testing/testing.o


# COMPILACION DE OBJETOS

hash_t/hash.o: hash_t/hash.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

hash_t/aux.o: hash_t/aux.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/hash_pruebas.o: testing/hash_pruebas.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/main.o: testing/main.c
	@- $(CC) -c $(CFLAGS) $^ -o $@

testing/testing.o: testing/testing.c
	@- $(CC) -c $(CFLAGS) $^ -o $@


# COMPILACION DE EJECUTABLES

hash: hash_t/hash.o hash_t/aux.o testing/hash_pruebas.o testing/main.o testing/testing.o
	@- $(CC) $(CFLAGS) $^ -o $@


# LIMPIAR

clean:
	@- rm -r $(REMOVE)
