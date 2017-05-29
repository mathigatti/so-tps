#include <iostream>
#include "ListaAtomica.hpp"
#include "ConcurrentHashMap.h"
#include <assert.h>     /* assert */

using namespace std;

int main(void) {
	Lista<int> listaAtomica;

	listaAtomica.push_front(1);
	printf("TESTEANDO LISTA ATOMICA...\n");
	assert(listaAtomica.front() == 1);
	listaAtomica.push_front(2);
	assert(listaAtomica.front() == 2);
	listaAtomica.push_front(3);
	assert(listaAtomica.front() == 3);
	printf("TODOS LOS TESTS PASARON EXITOSAMENTE\n");

	printf("TESTEANDO CONCURRENT_HASH_MAP...\n");

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
	printf("Hasta aca\n");
	maximo = h.maximum(1);
	printf("Aca no llega\n");
	assert(maximo.first=="casa");
	assert(maximo.second==2);
	h.addAndInc("a");

	maximo = h.maximum(1);
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

	printf("TODOS LOS TESTS PASARON EXITOSAMENTE\n");

	return 0;
}

