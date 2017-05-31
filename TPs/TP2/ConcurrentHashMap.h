#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>
#include "ListaAtomica.hpp"
#include <stdio.h>
#include <fstream>
#include <list>

using namespace std;

#define CANT_ENTRADAS 26

class ConcurrentHashMap {
   public:
    ConcurrentHashMap();
    ConcurrentHashMap(const ConcurrentHashMap& aCopiar);
    static ConcurrentHashMap count_words_ej2(string arch);
    static ConcurrentHashMap count_words_ej3(list<string>archs);
    static ConcurrentHashMap count_words(string arch, ConcurrentHashMap h);

    void addAndInc(string key);
    bool member(string key);
    pair<string, unsigned int> maximum(unsigned int nt);

    Lista<pair<string, unsigned int> > **tabla;

    //ConcurrentHashMap operator=(const ConcurrentHashMap& otro);

   private:
    int fHash(char x);
};