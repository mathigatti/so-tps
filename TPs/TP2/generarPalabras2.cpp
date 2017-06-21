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
	string archivo = "palabrasCaso2.";
	vector<string> palabras = { "a", "b", "c", "d", "e", "f", "g", "h",
							"i", "j", "k", "l", "m", "n", "o", "p", "q",
							"r", "s", "t", "u", "v", "w", "x", "y", "z" };

	for (int i = 0; i < 26; ++i){
		ofstream salida(archivo + to_string(i),std::ofstream::out);

		for (int j = 0; j < cant_palabras; ++j){

			if(j != cant_palabras - 1){

				salida<<palabras[i]+to_string(j + 1)<<endl;

			}else{

				salida<<palabras[i]+to_string(j + 1);

			}
		}
	}
	return 0;
}

