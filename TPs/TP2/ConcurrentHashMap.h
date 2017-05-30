#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>
#include "ListaAtomica.hpp"
#include <stdio.h>
#include <fstream>

using namespace std;

#define CANT_ENTRADAS 26

class ConcurrentHashMap {
   public:
    ConcurrentHashMap();
    ~ConcurrentHashMap();
    static ConcurrentHashMap count_words(string key);

    void addAndInc(string key);
    bool member(string key);
    pair<string, unsigned int> maximum(unsigned int nt);

    Lista<pair<string, unsigned int> > **tabla;

   private:
    int fHash(char x);
};