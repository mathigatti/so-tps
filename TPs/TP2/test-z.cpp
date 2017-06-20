#include <iostream>
#include "ListaAtomica.hpp"
#include "ConcurrentHashMap.h"
#include <assert.h>     /* assert */

using namespace std;

int main(void) {
	printf("TESTEANDO CONCURRENT_HASH_MAP_CON_Z...\n");

	ConcurrentHashMap h;
	int i;

	h.addAndInc("zumo");
	pair<string, unsigned int> maximo = h.maximum(1);
	assert(maximo.first=="zumo");

	printf("TODOS LOS TESTS PASARON EXITOSAMENTE\n");

	return 0;
}

