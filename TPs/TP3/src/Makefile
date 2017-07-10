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

test-1: dist_hashmap
	awk -f corpus.awk corpus | sort >corpus-post
	cat test-1.txt | mpiexec -np 10 ./dist_hashmap | sort | diff -u - corpus-post
	rm -f corpus-post

clean:
	rm -f $(BIN)

new: clean all
