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
    RWLock locks_lista[CANT_ENTRADAS];
    RWLock lock_iterador;
    RWLock lock_valor_maximo;

    void* maximumInternal(void* unused);
    int fHash(char x);

    struct Multithreading_data{
        Multithreading_data() : max_key("Lista Vacia"), max_value(0), index_fila_actual(0) {} 
        string max_key;
        unsigned int max_value;
        int index_fila_actual;
        Lista<pair<string, unsigned int> >::Iterador iterador_siguiente_nodo;
    };


};

#endif
