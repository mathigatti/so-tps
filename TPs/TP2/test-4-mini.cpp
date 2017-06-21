#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.h"
#include <assert.h>     /* assert */

using namespace std;

int main(int argc, char **argv) {
	ConcurrentHashMap h;
	list<string> l = {"10-gatos-20-perros", "50-gatos-50-perros", "100-gatos-10-perros", "100-gatos-10-perros", "200-zorros"};

	h = ConcurrentHashMap::count_words_ej4(atoi(argv[1]), l);

	assert(h.member("gato")==true);
	assert(h.member("perro")==true);
	assert(h.member("zorro")==true);
	assert(h.member("casa")==false);

	pair<string, unsigned int> maximo = h.maximum(1);
	assert(maximo.first=="gato");
	assert(maximo.second==260);

	printf("\tTODOS LOS TESTS PASARON EXITOSAMENTE\n");

	return 0;
}

