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
            case TAG_MEMBER:
                fin = nodeMember(msg,h);
                break;
            case TAG_ADDANDINC:
                fin = nodeAddAndInc(msg,h);
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
    HashMap::iterator it = h.begin();

    char msg[BUFFER_SIZE];

    while(it != h.end()){
        strcpy(msg, (*it).c_str());
        MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,CONSOLE_RANK,TAG_MAXIMUM_WORD,MPI_COMM_WORLD);
        it++;
    }
    trabajarArduamente();
    MPI_Send(&msg,BUFFER_SIZE,MPI_CHAR,CONSOLE_RANK,TAG_MAXIMUM_END,MPI_COMM_WORLD);
    
    return false;
}

bool nodeMember(char msg[], HashMap &h){
    string str(msg);
    bool esta = h.member(str);
    MPI_Send(&esta,1,MPI_C_BOOL,CONSOLE_RANK,TAG_MEMBER,MPI_COMM_WORLD);
    return false;
}

bool nodeAddAndInc(char msg[], HashMap &h){
    string str(msg);
    MPI_Send(NULL,0,MPI_C_BOOL,CONSOLE_RANK,TAG_ADDANDINC,MPI_COMM_WORLD);
 
    bool ack;
    MPI_Status status;   // required variable for receive routines

    MPI_Recv(&ack,1,MPI_C_BOOL,MPI_ANY_SOURCE,TAG_ADDANDINC_ACK,MPI_COMM_WORLD,&status);
    if(ack) h.addAndInc(str);
    return false;
}