#include <iostream>
#include "ListaAtomica.hpp"
#include "ConcurrentHashMap.h"
#include <assert.h>     /* assert */

using namespace std;
#define PTHREADS_CORRIENDO_SIMULTANEAMENTE 800

ConcurrentHashMap h_global;

void* addAndIncPerro(void*){
	h_global.addAndInc("perro");
}

void* addAndIncGato(void*){
	h_global.addAndInc("gato");
}

void* maximumAux(void*){
	h_global.maximum(10);
}

void* memberAux(void*){
	h_global.member("perro");
}

int main(void) {
	Lista<int> listaAtomica;
	printf("TESTS UNITARIOS\n");

	listaAtomica.push_front(1);
	printf("\tTESTEANDO LISTA ATOMICA...\n");
	assert(listaAtomica.front() == 1);
	listaAtomica.push_front(2);
	assert(listaAtomica.front() == 2);
	listaAtomica.push_front(3);
	assert(listaAtomica.front() == 3);
	printf("\tTODOS LOS TESTS PASARON EXITOSAMENTE\n");

	printf("\tTESTEANDO CONCURRENT_HASH_MAP...\n");

	ConcurrentHashMap h;
	int i;

	h.addAndInc("casa");
	assert(h.member("casa")==true);
	assert(h.member("perro")==false);
	pair<string, unsigned int> maximo = h.maximum(1);
	assert(maximo.first=="casa");
	assert(maximo.second==1);
	h.addAndInc("casa");
	h.addAndInc("caasa");
	h.addAndInc("cafsfasa");
	h.addAndInc("perro");
	maximo = h.maximum(1);
	assert(maximo.first=="casa");
	assert(maximo.second==2);
	h.addAndInc("a");

	maximo = h.maximum(30);
	assert(maximo.first=="casa");
	assert(maximo.second==2);
	h.addAndInc("a");
	maximo = h.maximum(1);
	assert(maximo.first=="a");
	assert(maximo.second==2);
	h.addAndInc("a");
	maximo = h.maximum(1);
	assert(maximo.first=="a");
	assert(maximo.second==3);

	printf("\tTODOS LOS TESTS PASARON EXITOSAMENTE\n");

	printf("TESTS DE CONCURRENCIA\n");
	printf("\tTESTEANDO CONCURRENCIA...\n");

    pthread_t pthrds[PTHREADS_CORRIENDO_SIMULTANEAMENTE];
    // si hay elementos en la lista, corremos
    for(int i=0; i<PTHREADS_CORRIENDO_SIMULTANEAMENTE; i++){
    	if( i%4 == 0){
	        pthread_create(&pthrds[i], NULL, addAndIncPerro, NULL);
	    }
    	if( i%4 == 1){
	        pthread_create(&pthrds[i], NULL, addAndIncGato, NULL);
	    }
	    if( i%4 == 2){
	        pthread_create(&pthrds[i], NULL, memberAux, NULL);
	    }
	    else{
	        pthread_create(&pthrds[i], NULL, maximumAux, NULL);	    	
	    }
    }
    /** espero que terminen los threads de correr **/
    for(int i=0; i<PTHREADS_CORRIENDO_SIMULTANEAMENTE; i++)
        pthread_join(pthrds[i], NULL);
    // Calculo el maximo y corroboro que sea gato con 200 apariciones
    maximo = h_global.maximum(5);
    assert(maximo.first == "gato");
    assert(maximo.second == 200);
	printf("\tTODOS LOS TESTS DE CONCURRENCIA PASARON EXITOSAMENTE\n");

	return 0;
}

