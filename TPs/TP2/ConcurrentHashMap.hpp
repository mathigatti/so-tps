#ifndef ConcurrentHashMap_h
#define ConcurrentHashMap_h
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>
#include "ListaAtomica.hpp"

using namespace std;

#define CANT_ENTRADAS 26

class ConcurrentHashMap {
   public:

    /*
    ConcurrentHashMap(): Constructor. Crea la tabla. La misma tendrá 26 entradas (una por
    cada letra del abecedario). Cada entrada consta de una lista de pares (string, entero). La
    función de hash será la primer letra del string.
    */

    ConcurrentHashMap(){
        tabla = new Lista<pair<string, unsigned int> >*[CANT_ENTRADAS];
        for(int i = 0; i < CANT_ENTRADAS; ++i){
          tabla[i] = new Lista<pair<string, unsigned int> >;
        }
    }

    /*
    void addAndInc(string key): Si key existe, incrementa su valor, si no existe, crea el par
    (key, 1). Se debe garantizar que sólo haya contención en caso de colisión de hash. Esto es,
    deberá haber locking a nivel de cada elemento del array.
    */

    void addAndInc(string key){
        int index = fHash(key[0]);
        for (auto it = tabla[index]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto t = it.Siguiente();
            if(t.first == key){
                pair<string, unsigned int> newValue = make_pair(key,t.second + 1);
                tabla[index]->update(t,newValue);
            }
        }        
        pair<string, unsigned int> value = make_pair(key,1);
        tabla[index]->push_front(value);
    }

    /*
    bool member(string key): true si y solo si el par (key, x) pertenece al hash map para algún
    x. Esta operación deberá ser wait-free.
    */

    bool member(string key){
        int index = fHash(key[0]);
        for (auto it = tabla[index]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto t = it.Siguiente();
            if(t.first == key){
                return true;
            }
        }
        return false;
    }

    /*
    pair<string, unsigned int>maximum(unsigned int nt): devuelve el par (k, m) tal que
    k es la clave con máxima cantidad de apariciones y m es ese valor. No puede ser concurrente
    con addAndInc, si con member, y tiene que ser implementada con concurrencia interna. El
    parámetro nt indica la cantidad de threads a utilizar. Los threads procesarán una fila del
    array. Si no tienen filas por procesar terminarán su ejecución.
    */

    pair<string, unsigned int> maximum(unsigned int nt){
        pair<string, unsigned int> maximo = make_pair("Tabla vacia",0);
        for (int i = 0; i < 26; i++) {
            for (auto it = tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
                auto t = it.Siguiente();
                if(maximo.second < t.second){
                    maximo = t;
                }
            }
        }
        return maximo;
    }

    Lista<pair<string, unsigned int> > **tabla;

   protected:


    int fHash(char x){
        return (int)x - 97;
    }

};

#endif
