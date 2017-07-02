#include "nodo.hpp"

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
			case TAG_LOAD:
				fin = nodeLoad(msg, h);
				break;
            case TAG_MAXIMUM:
                fin = nodeMaximum(h);
                break;
    	}
		trabajarArduamente();
    }
}

void trabajarArduamente() {
    //int r = rand() % 2500000 + 500000;
    //usleep(r);
}

bool nodeQuit(){
	return true;
}

bool nodeLoad(char msg[], HashMap &h){
    string str(msg);
    h.load(str);
    h.printAll();
    return false;
}

// Esta mal, deberia devolver la palabra maxima entre todos los HashMaps mergeados
bool nodeMaximum(HashMap &h){
    pair<string, unsigned int> result = h.maximum();
    MPI_Send(&result.second,1,MPI_INT,CONSOLE_RANK,TAG_MAXIMUM_NUMBER_RESPONSE,MPI_COMM_WORLD);

    MPI_Request request;   // required variable for non-blocking calls
    char msg[BUFFER_SIZE];
    strcpy(msg, (result.first).c_str());
    MPI_Isend(&msg,BUFFER_SIZE,MPI_CHAR,CONSOLE_RANK,TAG_MAXIMUM_WORD_RESPONSE,MPI_COMM_WORLD,&request);
    return false;
}