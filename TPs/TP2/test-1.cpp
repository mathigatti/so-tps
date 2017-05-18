#include <iostream>
#include "ListaAtomica.hpp"
#include <assert.h>     /* assert */

using namespace std;

int main(void) {
	Lista<int> listaAtomica;

	listaAtomica.push_front(1);
	printf("TESTEANDO...\n");
	assert(listaAtomica.front() == 1);
	listaAtomica.push_front(2);
	assert(listaAtomica.front() == 2);
	listaAtomica.push_front(3);
	assert(listaAtomica.front() == 3);
	printf("TODOS LOS TESTS PASARON EXITOSAMENTE\n");

	return 0;
}

