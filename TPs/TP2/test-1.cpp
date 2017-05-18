#include <iostream>
#include "ListaAtomica.hpp"

using namespace std;

int main(void) {
	Lista<int> listaAtomica;
	listaAtomica.push_front(1);
	listaAtomica.push_front(2);
	listaAtomica.push_front(3);
	printf("El numero al frente es %d\n",listaAtomica.front());

	return 0;
}

