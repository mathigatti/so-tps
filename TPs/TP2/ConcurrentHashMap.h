#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>
#include "ListaAtomica.hpp"
#include <stdio.h>
#include <fstream>
#include <list>
#include "locks/Mutex.h"

#define CANT_ENTRADAS 26

using namespace std;

class ConcurrentHashMap {
   public:
    ConcurrentHashMap();
    ConcurrentHashMap(const ConcurrentHashMap& aCopiar);
    static ConcurrentHashMap count_words_ej2(string arch);
    static ConcurrentHashMap count_words_ej3(list<string>archs);
    static ConcurrentHashMap count_words_ej4(unsigned int n, list<string>archs);
    static ConcurrentHashMap count_words(string arch, ConcurrentHashMap* h);
    static pair<string, unsigned int> maximum(unsigned int p_archivos, unsigned int p_maximos, list<string> archs);
    static pair<string, unsigned int> maximum_ej6(unsigned int p_archivos, unsigned int p_maximos, list<string> archs);
    void addAndInc(string key);
    bool member(string key);
    pair<string, unsigned int> maximum(unsigned int nt);
    Lista<pair<string, unsigned int> > **tabla;

    //ConcurrentHashMap operator=(const ConcurrentHashMap& otro);

   private:
    bool incrementar(string key,int index);
    int fHash(char x);
};