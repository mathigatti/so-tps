#include <iostream>
#include <cstdlib>
#include "ConcurrentHashMap.h"

using namespace std;

int main(int argc, char **argv) {
	ConcurrentHashMap h;
	list<string> l = { "palabras1", "palabras2", "palabras3", "palabras4", "palabras5"};
	//list<string> l = { "corpus-0", "corpus-1", "corpus-2", "corpus-3", "corpus-4" };
	//list<string> l = { "corpus"};
	int i;

	h = ConcurrentHashMap::count_words_ej3(l);
	for (i = 0; i < 26; i++) {
		for (auto it = h.tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
			auto t = it.Siguiente();
			cout << t.first << " " << t.second << endl;
		}
	}

	//cout<<"HOLAA"<<h.member("ginebra")<<endl;


	return 0;
}

