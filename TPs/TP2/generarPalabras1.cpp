#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.h"
#include <sys/time.h>
#include <vector>
#include <algorithm>
#include <string>


using namespace std;

int main(int argc, char **argv) {


	if (argc != 2) {
		cerr << "uso: " << argv[0] << " #palabras" << endl;
		return 1;
	}

	int cant_palabras = atoi(argv[1]);
	string archivo = "palabrasCaso1.";

	for (int i = 0; i < 26; ++i){
		ofstream salida(archivo + to_string(i),std::ofstream::out);

		for (int j = 0; j < cant_palabras; ++j){

			if(j != cant_palabras - 1){

				salida<<"arbol"<<endl;

			}else{

				salida<<"arbol";

			}
		}
	}
	return 0;
}

