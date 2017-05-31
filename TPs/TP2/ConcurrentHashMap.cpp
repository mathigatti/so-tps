#include "ConcurrentHashMap.h"

RWLock locks_lista[CANT_ENTRADAS];
RWLock rw_lock;

struct Count_words_data{
    ConcurrentHashMap* h;
    string file;
};

struct Multithreading_data{
    Multithreading_data() : max_key("Lista Vacia"), max_value(0), index_fila_actual(0) {} 

    string max_key;
    unsigned int max_value;
    int index_fila_actual;

    Lista<pair<string, unsigned int> >::Iterador iterador_siguiente_nodo;
    Lista<pair<string, unsigned int> > **tabla;
    RWLock lock_iterador;
    RWLock lock_valor_maximo;
};

void* count_words_aux(void* data){
    Count_words_data* words_data = (Count_words_data*) data;
    ConcurrentHashMap::count_words(words_data->file,words_data->h);
}

void* maximumInternal(void* multithreading_data) {

    Multithreading_data* data = (Multithreading_data*) multithreading_data;

    while(data->iterador_siguiente_nodo.HaySiguiente()){
        // lockeo para leer que dato voy a trabajar
        data->lock_iterador.wlock();

        if(data->iterador_siguiente_nodo.HaySiguiente()){
            // obtengo el dato y actualizo el iterador global para que se siga procesando en paralelo
            pair<string, unsigned int> par_actual = data->iterador_siguiente_nodo.Siguiente();
            data->iterador_siguiente_nodo.Avanzar();

            if(not data->iterador_siguiente_nodo.HaySiguiente()){
                // hay que buscar en otra letra (fila)
                data->index_fila_actual++;
                data->iterador_siguiente_nodo = (data->tabla[data->index_fila_actual])->CrearIt();

                for(int i=data->index_fila_actual+1; i<CANT_ENTRADAS && not data->iterador_siguiente_nodo.HaySiguiente(); i++){
                    data->iterador_siguiente_nodo = (data->tabla[i])->CrearIt();
                    data->index_fila_actual=i;
                }
            }

            // chequeo si hay nuevo maximo
            string key = par_actual.first;
            unsigned int value = par_actual.second;

            // pido el recurso del max actual para actualizarlo de ser necesario
            data->lock_valor_maximo.wlock();
            if(data->max_value < value){
                data->max_value = value;
                data->max_key = key;
            }

            data->lock_valor_maximo.wunlock();

        }

        data->lock_iterador.wunlock();

    }
    pthread_exit(NULL);     // UFF..! :B
}


ConcurrentHashMap::ConcurrentHashMap(){
    tabla = new Lista<pair<string, unsigned int> >*[CANT_ENTRADAS];
    for(int i = 0; i < CANT_ENTRADAS; ++i){
      tabla[i] = new Lista<pair<string, unsigned int> >;
    }
}

ConcurrentHashMap::ConcurrentHashMap(const ConcurrentHashMap& aCopiar){
    tabla = new Lista<pair<string, unsigned int> >*[CANT_ENTRADAS];
    for(int i = 0; i < CANT_ENTRADAS; ++i){
      tabla[i] = new Lista<pair<string, unsigned int> >;
    }

    for (int i = 0; i < 26; i++) {
        for (auto it = aCopiar.tabla[i]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
            auto t = it.Siguiente();
            pair<string, unsigned int> value = make_pair(t.first,t.second);
            tabla[i]->push_front(value);
        }
    }
}

ConcurrentHashMap ConcurrentHashMap::count_words(string arch, ConcurrentHashMap* h){

    ifstream palabras(arch);
    string linea;
    while (getline(palabras, linea)){
        h->addAndInc(linea);
    }

    return *h;

}

ConcurrentHashMap ConcurrentHashMap::count_words_ej2(string arch){
    ConcurrentHashMap h;
    return count_words(arch,&h);
}

ConcurrentHashMap ConcurrentHashMap::count_words_ej3(list<string>archs){
    int size = archs.size();
    pthread_t pthrds[size];

    ConcurrentHashMap h;

    Count_words_data data[size];

    int i = 0;

    for (auto it = archs.begin(); it != archs.end(); ++it){
        data[i].h = &h;
        data[i].file = *it;
        pthread_create(&pthrds[i], NULL, count_words_aux, &data[i]);
        i++;
    }
    for(int t=0; t<size; t++){
        pthread_join(pthrds[t], NULL);
    }

    return h;

}


/*
void addAndInc(string key): Si key existe, incrementa su valor, si no existe, crea el par
(key, 1). Se debe garantizar que sólo haya contención en caso de colisión de hash. Esto es,
deberá haber locking a nivel de cada elemento del array.
*/

void ConcurrentHashMap::addAndInc(string key){

    rw_lock.wlock();
    
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

    rw_lock.wunlock();
}

/*
bool member(string key): true si y solo si el par (key, x) pertenece al hash map para algún
x. Esta operación deberá ser wait-free.
*/

bool ConcurrentHashMap::member(string key){
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
pair<string, unsigned int> ConcurrentHashMap::maximum(unsigned int nt){

    RWLock lock_iterador;
    RWLock lock_valor_maximo;

    Multithreading_data* data = new Multithreading_data();
    data->iterador_siguiente_nodo = tabla[0]->CrearIt();
    data->tabla = tabla;
    data->lock_iterador = lock_iterador;
    data->lock_valor_maximo = lock_valor_maximo;

    rw_lock.rlock();

    for(int i=1; i<CANT_ENTRADAS && not data->iterador_siguiente_nodo.HaySiguiente(); i++){
        data->iterador_siguiente_nodo = tabla[i]->CrearIt();
        data->index_fila_actual=i;
    }

    pthread_t pthrds[nt];

    // si hay elementos en la lista, corremos
    if(data->iterador_siguiente_nodo.HaySiguiente()){
        for(int t=0; t<nt; t++){
            pthread_create(&pthrds[t], NULL, maximumInternal, data);
        }
        /** espero que terminen los threads de correr **/
        void* status = NULL;
        for(int t=0; t<nt; t++)
            pthread_join(pthrds[t], NULL);
    }

    rw_lock.runlock();

    return make_pair(data->max_key, data->max_value);
}

int ConcurrentHashMap::fHash(char x){
    return (int)x - 97;
}