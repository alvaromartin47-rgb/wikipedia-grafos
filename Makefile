CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic
VFLAGS = --leak-check=full --track-origins=yes --show-reachable=yes

# ARCHIVOS .C DE COMPILACION DE CADA TDA
ARCH_C_HASH = hash/hash_t/*.c hash/testing/*.c
ARCH_C_ABB = abb/abb_t/*.c abb/abb_t/nodo/*.c abb/testing/*.c abb/testing/tdas-aux/*.c
ARCH_C_HEAP = heap/heap_t/*.c heap/testing/*.c heap/testing/tdas-aux/*.c
ARCH_C_TP2 = tp2/zyxcba.c tp2/src/*.c tp2/src/clinica/*.c tp2/src/hash/*.c tp2/src/heap/*.c tp2/src/sala/*.c tp2/src/csv/*.c tp2/src/cola/*.c tp2/src/lista/*.c tp2/src/persona/*.c tp2/src/abb/*.c tp2/src/nodo/*.c tp2/src/abb/nodo/*.c

# EJECUTABLES A BORRAR
REMOVE = pruebas_hash pruebas_abb pruebas_heap pruebas_tp2
PATH_TESTING_TP2 = tp2/pruebas_zyxcba/pruebas_tp2

all: 
	@- echo "ERROR, no se detectó ningún TDA."
	
hash_t: $(ARCH_C_HASH)
	$(CC) $(CFLAGS) $(ARCH_C_HASH) -o pruebas_hash
	valgrind $(VFLAGS) ./pruebas_hash
	
abb_t: $(ARCH_C_ABB)
	$(CC) $(CFLAGS) $(ARCH_C_ABB) -o pruebas_abb
	valgrind $(VFLAGS) ./pruebas_abb
	
heap_t: $(ARCH_C_HEAP)
	$(CC) $(CFLAGS) $(ARCH_C_HEAP) -o pruebas_heap
	valgrind $(VFLAGS) ./pruebas_heap

tp2_t: $(ARCH_C_TP2)
	$(CC) $(CFLAGS) $(ARCH_C_TP2) -o $(PATH_TESTING_TP2)

clean:
	@- rm -rf $(REMOVE)
