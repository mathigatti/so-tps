#ifndef ConcurrentHashMap_h
#define ConcurrentHashMap_h
#include <utility>      // std::pair, std::make_pair
#include <string>       // std::string
#include <iostream>
#include "ListaAtomica.hpp"
#include "rwlock/RWLock.h"

using namespace std;

#define CANT_ENTRADAS 26

RWLock locks_lista[CANT_ENTRADAS];
RWLock lock_iterador;
RWLock lock_valor_maximo;

string max_key;
unsigned int max_value;
int index_fila_actual;
Lista<pair<string, unsigned int> >::Iterador iterador_siguiente_nodo;

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

    ~ConcurrentHashMap() {
        for(int i = 0; i < CANT_ENTRADAS; ++i){
          delete tabla[i];
        }
        delete[] tabla;
    }


    /*
    void addAndInc(string key): Si key existe, incrementa su valor, si no existe, crea el par
    (key, 1). Se debe garantizar que sólo haya contención en caso de colisión de hash. Esto es,
    deberá haber locking a nivel de cada elemento del array.
    */

    void addAndInc(string key){

        /** obtenemos acceso a la lista correspondiente y la lockeamos **/
        int index = fHash(key[0]);      
        locks_lista[index].wlock();
        
        /** agregamos o incrementamos segun corresponda **/
        bool pertenece = false;
        for (auto it = tabla[index]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto t = it.Siguiente();
            if(t.first == key){
                pair<string, unsigned int> newValue = make_pair(key,t.second + 1);
                tabla[index]->update(t,newValue);       // ** SI OBTENEMOS LA REFERENCIA.. NO DEBERIAMOS INCREMENTAR EL PAR DE UNA? **
                pertenece = true;
            }
        }   
        if(!pertenece){
            pair<string, unsigned int> value = make_pair(key,1);
            tabla[index]->push_front(value);
        }
        /** habilitamos acceso a la lista para otros threads que hagan addAndInc() **/
        locks_lista[index].wunlock();
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

    void* maximumInternal(void* unused) {
        while(iterador_siguiente_nodo.HaySiguiente()){     // SI NO HAY MAS SE APUNTA A NULL

            // lockeo para leer que dato voy a trabajar
            lock_iterador.wlock();

            if(iterador_siguiente_nodo.HaySiguiente()){
                // obtengo el dato y actualizo el iterador global para que se siga procesando en paralelo
                pair<string, unsigned int> par_actual = iterador_siguiente_nodo.Siguiente();
                iterador_siguiente_nodo.Avanzar();

                if(not iterador_siguiente_nodo.HaySiguiente()){
                    locks_lista[index_fila_actual].runlock();
                    // hay que buscar en otra letra (fila)
                    index_fila_actual++;
                    iterador_siguiente_nodo = tabla[index_fila_actual]->CrearIt();

                    for(int i=index_fila_actual+1; i<CANT_ENTRADAS && not iterador_siguiente_nodo.HaySiguiente(); i++){
                        iterador_siguiente_nodo = tabla[i]->CrearIt();
                        index_fila_actual=i;
                    }

                    locks_lista[index_fila_actual].rlock();

                }
            
                // deslockeo
                lock_iterador.wunlock();

                // chequeo si hay nuevo maximo
                string key = par_actual.first;
                unsigned int value = par_actual.second;

                // pido el recurso del max actual para actualizarlo de ser necesario
                lock_valor_maximo.wlock();
                if(max_value < value){
                    max_value = value;
                    max_key = key;
                lock_valor_maximo.wunlock();
                }
            }
        }
        pthread_exit(NULL);     // UFF..! :B
    }

    /*
    pair<string, unsigned int>maximum(unsigned int nt): devuelve el par (k, m) tal que
    k es la clave con máxima cantidad de apariciones y m es ese valor. No puede ser concurrente
    con addAndInc, si con member, y tiene que ser implementada con concurrencia interna. El
    parámetro nt indica la cantidad de threads a utilizar. Los threads procesarán una fila del
    array. Si no tienen filas por procesar terminarán su ejecución.
    */
    pair<string, unsigned int> maximum(unsigned int nt){
        iterador_siguiente_nodo = tabla[0]->CrearIt();
        max_key = "Tabla vacia";
        max_value = 0;
        index_fila_actual = 0;

        for(int i=1; i<CANT_ENTRADAS && not iterador_siguiente_nodo.HaySiguiente(); i++){
            iterador_siguiente_nodo = tabla[i]->CrearIt();
            index_fila_actual=i;
        }

        locks_lista[index_fila_actual].rlock();

        pthread_t pthrds[nt];

        // si hay elementos en la lista, corremos
        if(iterador_siguiente_nodo.HaySiguiente()){
            for(int t=0; t<nt; t++){
                int unused = 0;
                pthread_create(&pthrds[t], NULL, maximumInternal, &unused);
            }
            /** espero que terminen los threads de correr **/
            void* status = NULL;
            for(int t=0; t<nt; t++)
                pthread_join(pthrds[t], &status);
        }

        return make_pair(max_key, max_value);
    }

    Lista<pair<string, unsigned int> > **tabla;

   protected:

    int fHash(char x){
        return (int)x - 97;
    }

};

#endif
