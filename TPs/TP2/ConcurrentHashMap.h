#ifndef ConcurrentHashMap_h
#define ConcurrentHashMap_h
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>
#include "ListaAtomica.hpp"
#include "rwlock/RWLock.h"
#include <stdio.h>

using namespace std;

#define CANT_ENTRADAS 26

class ConcurrentHashMap {
   public:
    ConcurrentHashMap();
    ~ConcurrentHashMap();

    void addAndInc(string key);
    bool member(string key);
    pair<string, unsigned int> maximum(unsigned int nt);

   private:
    Lista<pair<string, unsigned int> > **tabla;
    int fHash(char x);

};

#endif
