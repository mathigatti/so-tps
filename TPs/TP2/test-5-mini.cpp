#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.h"
#include <assert.h>     /* assert */

using namespace std;

int main(int argc, char **argv) {
	ConcurrentHashMap h;
	list<string> l = {"10-gatos-20-perros", "50-gatos-50-perros", "100-gatos-10-perros", "100-gatos-10-perros", "200-zorros"};

	pair<string, unsigned int> p = ConcurrentHashMap::maximum(atoi(argv[1]), atoi(argv[2]), l);

	assert(p.first=="gato");
	assert(p.second==260);

	printf("\tTODOS LOS TESTS PASARON EXITOSAMENTE\n");

	return 0;
}

