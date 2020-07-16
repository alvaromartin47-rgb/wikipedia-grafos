CFLAGS = -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror
ARCH_C = *.c testing/*.c testing/tdas-aux/*.c

pruebas: $(ARCH_C)
	$(CC) $(CFLAGS) $(ARCH_C) -o pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

clean:
	rm -rf prog.o
	rm -rf pruebas
