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
RWLock lock_it_pnode;
RWLock lock_val_max;


/**
    Impl CHM:
    
    - Mods en general:
        - Quite el flag "writing" de RWLocks, para alinearlo a las necesidades de nuestro tp. 
            Con esto permitimos que los addAndInc corran en listas paralelas.
        - Alinee lo de RWLocks en los métodos del CHM, que encapsula justamente lo que queríamos para comunicar el add con el max 
            (entiendo que es la implementación de un tp pasado para comunicar escritores y lectores, literalmente). 
            Esta muy bueno el wrapper, basicamente alinea via variable de condicion y contador de threads corriendo cada funcion, para que corran de a tandas separadas. 
            En nuestro caso el escritor es addAndInc, y el lector es maximum. Deje comentarios en el CHM de como vi que funca.
    - Mods en addAndInc():
        - Removi un bug (siempre hacia push_front(key,1) después del for, sin importar si existiese la clave)
        - Agregue lock a nivel de lista (array de mutexes).
        - El lock es para acceder la lista si o si (leer comentario a continuación)
        * Comentario: La pase para el orto un buen rato con lo de, cito: 
            "Se debe garantizar que solo haya contencion en caso de colision de hash. Esto es, deberia haber locking a nivel de cada elemento del array."
            Lo digo porque colision habría cuando hay dos keys distintas compitiendo por el mismo bucket. 
            Después de pensarlo, no me cierra que no se lockee de cero al acceder la lista, sea cual sea el caso del addAndInc. 
            Asiq lo mande de una. Si quieren lo discutimos, tengo buenas excusas para argumentar acá
    - Mods en maximum():
        - Lo rearme entero para que implemente la concurrencia interna. Para mirar con amor esto! 
        - Resumen: maximum prepara un struct que reciben por parametro todos mini_maximum, con el cual van iterando al siguiente. 
            Hay dos mutex para manipular el struct, uno para avanzar el iterador del CHM, y otro para mantener el maximo de cada momento.

    Comentario: son todos RWLocks, pero el unico que usa las funciones de read y write es el rw_lock. Los otros solo usan el mutex

**/


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
          tabla[i] = new Lista->CrearIt()<pair<string, unsigned int> >;
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
        RWLock lock_index = locks_lista[index].lock();
        
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
        lock_index.unlock();

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

    /** Cositas sucias para maximum - Funcion de concurrencia interna para maximum y struct para bufferear la info **/
    struct it_multithread_data{

        // key-val restringidos por lock_val_max
        string max_key;
        unsigned int max_val;
        
        // iterador actual al CHM accesible via lock_it_pnode
        int index_fila_to_process;
        Iterador it_to_process;
    };

    void *maximumInternal(void *it_max_hmap) {

        // recupero prm
        struct it_multithread_data *it_max_hmap;
        it_max_hmap = (struct it_multithread_data *) it_max_hmap;
        auto it_pair_actual;

        // Bucle agresivo
        while(it_max_hmap->it_to_process != NULL){     // SI NO HAY MAS SE APUNTA A NULL


            // lockeo para leer que dato voy a trabajar
            lock_it_pnode.lock();
            haySiguiente = it_max_hmap->it_to_process.HaySiguiente();      // Vuelvo a chequear por si cambio en el interin

            if(haySiguiente){
                // obtengo el dato y actualizo el iterador global para que se siga procesando en paralelo
                it_pair_actual = it_max_hmap->it_to_process.Siguiente();
                it_max_hmap->it_to_process.Avanzar();

                if(not it_max_hmap->it_to_process.HaySiguiente()){
                    locks_lista[it_max_hmap->index_fila_to_process].unlock();
                    // hay que buscar en otra letra (fila)
                    int index_fila = it_max_hmap->index_fila_to_process + 1;
                    auto it_fila = NULL;
                    while(index_fila<CANT_ENTRADAS && it_lista == NULL){
                        index_fila++;
                        it_fila = tabla[index_fila]->CrearIt();
                    }
                    it_max_hmap->it_to_process = it_fila;
                    it_max_hmap->index_fila_to_process = index_fila;
                    locks_lista[it_max_hmap->index_fila_to_process].lock();
                }
            }
            // deslockeo
            lock_it_pnode.unlock();

            // chequeo si hay nuevo maximo
            if(it_pair_actual != NULL){
                string it_key = it_pair_actual.first;
                unsigned int it_val = it_pair_actual.second;

                // pido el recurso del max actual para actualizarlo de ser necesario
                lock_val_max.lock();
                if(it_max_hmap->max_val < it_val){
                    it_max_hmap->max_val = it_val;
                    it_max_hmap->max_key = it_key;
                }
                lock_val_max.unlock();
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

        /** los locks para lectura funcionan 'casi' semejantes a los de escritura
            La diferencia radica en que incrementan el counter post-signal, con lo cual no frenan a nuevos escritores al tomar el lock
            Lo podemos discutir esto ultimo despues! **/

        /** armo concurrencia interna para busqueda del maximo **/        
        // inicializamos variables

        pthread_t pthrds[nt];
        it_multithread_data it_max_hmap = {};
        it_max_hmap.max_key = "Tabla vacia";
        it_max_hmap.max_val = 0;
        it_max_hmap.index_fila_to_process = 0;

        auto it_lista = NULL;
        for(int i=0; i<CANT_ENTRADAS && it_lista == NULL; i++){
            it_lista = tabla[i]->CrearIt();
            it_max_hmap.index_fila_to_process=i;
        }
        locks_lista[it_max_hmap.index_fila_to_process].lock();
        it_max_hmap.it_to_process = it_lista;

        // si hay qué correr, corremos
        if(it_lista != NULL){
            for(t=0; t<nt; t++)
            pthread_create(&pthrds[t], NULL, maximumInternal, (void *)&it_max_hmap);

            /** espero que terminen los threads de correr **/
            for(t=0; t<nt; t++)
                pthread_join(thread[t], &status);
        }

        return make_pair(it_max_hmap.max_key, it_max_hmap.max_val);
    }

    Lista<pair<string, unsigned int> > **tabla;

   protected:

    int fHash(char x){
        return (int)x - 97;
    }
};

#endif
