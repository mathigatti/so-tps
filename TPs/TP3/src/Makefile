.PHONY : all clean run new


MPIROOT = /usr

CFLAGS+= -Wall -g $(INCL)
CC=gcc
MPICC= $(MPIROOT)/bin/mpic++
INCL= -I$(MPIROOT)/include
SRCS= consola.cpp nodo.cpp HashMap.cpp
BIN= dist_hashmap

all: dist_hashmap

dist_hashmap: $(SRCS)
	$(MPICC) $(CFLAGS) -o $(BIN) main.cpp $(SRCS)

test: .SILENT 
	# .SILENT permite que no se imprima cada instruccion al ejecutar el makefile
	# de esta manera evitando oscurecer la legibilidad de los tests

.SILENT:
	echo "INICIANDO TESTS"

	echo "1. Cargo 6 archivos con la palabra perro 50 veces y 4 nodos (Uno consola). Deberia darme que el maximo es perro con 300 apariciones sin tirar error y finalizando correctamente del programa."
	$(MAKE) test-1
	echo "\t\tTest finalizado con EXITO :)"

	echo "2. Cargo un archivo con 2500 palabras (Algunas repetidas) a 10 nodos. Chequeo que se carguen correctamente. (Test extraido del TP2)"
	$(MAKE) test-2
	echo "\tTest finalizado con EXITO :)"

	echo "3. Cargo 1 archivo con 10 ovejas, 10 gatos y 11 caballos, luego agrego 2 ovejas. Hay 12 nodos (Uno consola). Primero deberia darme que el maximo es caballo con 11, luego oveja con 12. Deberia aparecer oveja pero no perro y el programa deberia finalizar correctamente."
	$(MAKE) test-3
	echo "\tTest finalizado con EXITO :)"

	echo "4. Igual al test 3 pero con 2 nodos (Uno consola). Primero deberia darme que el maximo es caballo con 11, luego oveja con 12. Deberia aparecer oveja pero no perro y el programa deberia finalizar correctamente."
	$(MAKE) test-4
	echo "\tTest finalizado con EXITO :)"

	echo "TESTS FINALIZADOS EXITOSAMENTE"


test-1: dist_hashmap
	cat tests/test-1.txt | mpiexec -np 4 ./dist_hashmap | grep "El máximo es" | diff -u - tests/test-1-expected-output.txt

test-2: dist_hashmap
	awk -f corpus.awk corpus | sort >corpus-post
	cat tests/test-2.txt | mpiexec -np 10 ./dist_hashmap | sort | diff -u - corpus-post
	rm -f corpus-post

test-3: dist_hashmap
	cat tests/test-3.txt | mpiexec -np 12 ./dist_hashmap | grep "El máximo es" | diff -u - tests/test-3-expected-output.txt
	cat tests/test-3.txt | mpiexec -np 12 ./dist_hashmap | grep "El string"  | diff -u - tests/test-3-expected-output2.txt

test-4: dist_hashmap
	cat tests/test-3.txt | mpiexec -np 2 ./dist_hashmap | grep "El máximo es" | diff -u - tests/test-3-expected-output.txt
	cat tests/test-3.txt | mpiexec -np 2 ./dist_hashmap | grep "El string"  | diff -u - tests/test-3-expected-output2.txt

clean:
	rm -f $(BIN)
	rm -f tail

new: clean all
