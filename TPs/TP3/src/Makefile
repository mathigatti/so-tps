.PHONY : all clean run new

MPIROOT = /usr

CFLAGS+= -Wall -g $(INCL)
CC=gcc
MPICC=  $(MPIROOT)/bin/mpic++
INCL= -I$(MPIROOT)/include
SRCS= consola.cpp nodo.cpp HashMap.cpp
BIN= dist_hashmap

all: dist_hashmap

dist_hashmap: $(SRCS)
	$(MPICC) $(CFLAGS) -o $(BIN) main.cpp $(SRCS)

clean:
	rm -f $(BIN)

new: clean all
