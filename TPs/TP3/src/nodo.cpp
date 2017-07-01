#include "nodo.hpp"
#include "HashMap.hpp"
#include "mpi.h"
#include <unistd.h>
#include <stdlib.h>

using namespace std;

void nodo(unsigned int rank) {
    printf("Soy un nodo. Mi rank es %d \n", rank);

    // TODO: Implementar
    // Creo un HashMap local
    HashMap h;
    MPI_Status status;   // required variable for receive routines


    bool fin = false;
    char msg[BUFFER_SIZE];
    while (!fin) {
    	MPI_Recv(&msg,BUFFER_SIZE,MPI_CHAR,CONSOLE_RANK,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
    	switch (status.MPI_TAG){
			case TAG_QUIT:
	    		fin = nodeQuit();
				break;
    	}
		trabajarArduamente();



    }
}

void trabajarArduamente() {
    int r = rand() % 2500000 + 500000;
    usleep(r);
}

bool nodeQuit(){
	return true;
}
