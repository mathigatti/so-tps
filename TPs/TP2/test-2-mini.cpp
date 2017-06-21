#include <iostream>
#include "ConcurrentHashMap.h"
#include <assert.h>     /* assert */

using namespace std;

int main(void) {
	ConcurrentHashMap h1, h2, h3, h4;
	h1 = ConcurrentHashMap::count_words_ej2("10-gatos-20-perros");
	h2 = ConcurrentHashMap::count_words_ej2("50-gatos-50-perros");
	h3 = ConcurrentHashMap::count_words_ej2("100-gatos-10-perros");
	h4 = ConcurrentHashMap::count_words_ej2("200-zorros");

	assert(h1.member("gato")==true);
	assert(h1.member("perro")==true);
	assert(h1.member("casa")==false);

	assert(h2.member("gato")==true);
	assert(h2.member("perro")==true);
	assert(h2.member("casa")==false);

	assert(h3.member("gato")==true);
	assert(h3.member("perro")==true);
	assert(h3.member("casa")==false);

	assert(h4.member("gato")==false);
	assert(h4.member("perro")==false);
	assert(h4.member("zorro")==true);

	pair<string, unsigned int> maximo1 = h1.maximum(1);
	assert(maximo1.first=="perro");
	assert(maximo1.second==20);

	pair<string, unsigned int> maximo2 = h2.maximum(2);
	assert(maximo2.first=="gato");
	assert(maximo2.second==50);

	pair<string, unsigned int> maximo3 = h3.maximum(3);
	assert(maximo3.first=="gato");
	assert(maximo3.second==100);

	pair<string, unsigned int> maximo4 = h4.maximum(4);
	assert(maximo4.first=="zorro");
	assert(maximo4.second==200);



	printf("\tTODOS LOS TESTS PASARON EXITOSAMENTE\n");

	

	return 0;
}

