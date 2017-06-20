#include "ConcurrentHashMap.h"

Mutex locks_lista[CANT_ENTRADAS];

struct Count_words_data{
    ConcurrentHashMap* h;
    string file;
    std::atomic_uint* next_available;
    list<string>archs;
};


struct Multithreading_data{
    Multithreading_data() : max_key("Lista Vacia"), max_value(0), index_fila_actual(0) {} 

    string max_key;
    unsigned int max_value;
    int index_fila_actual;

    Lista<pair<string, unsigned int> >::Iterador iterador_siguiente_nodo;
    Lista<pair<string, unsigned int> > **tabla;
    Mutex lock_iterador;
    Mutex lock_valor_maximo;
};

string iesimo(list<string>archs, int iesimo){
    if (archs.size() > iesimo){
        std::list<string>::iterator it = archs.begin();
        std::advance(it, iesimo);
        return *it;
    }
    return "";
}

void* count_words_aux(void* data){
    Count_words_data* words_data = (Count_words_data*) data;
    ConcurrentHashMap::count_words(words_data->file,words_data->h);
    pthread_exit(NULL); 
}

void* count_words_aux2(void* data){
    Count_words_data* words_data = (Count_words_data*) data;
    int size = (words_data->archs).size();
    std::atomic_uint* next_available = words_data->next_available;

    unsigned int proximo = next_available->fetch_add(1);
    string arch;

    while(proximo < size){
        if(proximo < size){
            arch = iesimo((words_data->archs),proximo);
            ConcurrentHashMap::count_words(arch,words_data->h);
        }
        proximo = next_available->fetch_add(1);
    }
    pthread_exit(NULL); 
}

void* maximumInternal(void* multithreading_data) {

    Multithreading_data* data = (Multithreading_data*) multithreading_data;

    while(data->iterador_siguiente_nodo.HaySiguiente()){
        // lockeo para leer que dato voy a trabajar
        data->lock_iterador.lock();

        if(data->iterador_siguiente_nodo.HaySiguiente()){
            // obtengo el dato y actualizo el iterador global para que se siga procesando en paralelo
            pair<string, unsigned int> par_actual = data->iterador_siguiente_nodo.Siguiente();
            data->iterador_siguiente_nodo.Avanzar();

            if(not data->iterador_siguiente_nodo.HaySiguiente() and data->index_fila_actual != CANT_ENTRADAS - 1){
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
            data->lock_valor_maximo.lock();
            if(data->max_value < value){
                data->max_value = value;
                data->max_key = key;
            }

            data->lock_valor_maximo.unlock();

        }

        data->lock_iterador.unlock();

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

    for (int i = 0; i < CANT_ENTRADAS; i++) {
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

ConcurrentHashMap ConcurrentHashMap::count_words_ej4(unsigned int n,
list<string>archs){
    int size = archs.size();
    pthread_t pthrds[n];

    Count_words_data data;
    data.archs = archs;
    ConcurrentHashMap h;
    data.h = &h;
    std::atomic_uint next_available;
    next_available.store(0);
    data.next_available = &next_available;

    int i = 0;
    for (auto it = archs.begin(); i < n && it != archs.end(); ++it){
        pthread_create(&pthrds[i], NULL, count_words_aux2, &data);
        i++;
    }

    for(int t=0; t<n; t++){
        pthread_join(pthrds[t], NULL);
    }

    return h;

}

struct Maximum_data{
    ConcurrentHashMap* chMaps;
    std::atomic_uint* next_available;
    list<string>archs;
};

void* maximum_aux(void* data){
    Maximum_data* m_data = (Maximum_data*) data;
    int size = (m_data->archs).size();
    std::atomic_uint* next_available = m_data->next_available;

    unsigned int proximo = next_available->fetch_add(1);    // fetch_add: aumenta y retorna el valor precargado
    string arch;

    // mientras haya algun archivo a procesar
    while(proximo < size){
        // lo obtenemos
        arch = iesimo((m_data->archs),proximo);

        // obtenemos el array de mapas (un mapa por archivo)
        ConcurrentHashMap* chMaps = m_data->chMaps;

        // y creamos su CHMap asociado
        ConcurrentHashMap::count_words(arch, chMaps+proximo);
        proximo = next_available->fetch_add(1);
    }

    pthread_exit(NULL); 
}

pair<string, unsigned int> ConcurrentHashMap::maximum(unsigned int p_archivos, unsigned int p_maximos, list<string> archs){

    /** inicializamos estructuras **/
    pthread_t pthrds_files[p_archivos];
    ConcurrentHashMap chMaps[archs.size()];
    std::atomic_uint next_available;
    next_available.store(0);
    
    Maximum_data m_data;
    m_data.chMaps = chMaps;
    m_data.next_available = &next_available;
    m_data.archs = archs;

    /** leemos los archivos, un thread por archivo - asumimos p_archivos <= |archs| **/
    for (int i = 0; i < p_archivos; i++){
        pthread_create(&pthrds_files[i], NULL, maximum_aux, &m_data);
        i++;
    }

    /** joineamos para esperar que terminen **/
    for(int t = 0; t < p_archivos; t++){
        pthread_join(pthrds_files[t], NULL);
    }

    /** los mergeamos al 1er mapa **/
    ConcurrentHashMap fullMap = chMaps[0];

    for(int i = 1; i < archs.size(); i++){
        // obtenemos el mapa a mergear
        ConcurrentHashMap itMap = chMaps[i];

        // iteramos cada una de sus listas
        for(int index = 0; index < CANT_ENTRADAS; index++){
            for (auto itList = itMap.tabla[index]->CrearIt(); itList.HaySiguiente(); itList.Avanzar()) {
                pair<string, unsigned int>& t_merge = itList.Siguiente();
                
                // por cada palabra a mergear, chequeamos si existe iterando la lista correspondiente
                bool esMiembro = false;
                for (auto itFullList = fullMap.tabla[index]->CrearIt(); itFullList.HaySiguiente() && !esMiembro; itFullList.Avanzar()) {
                    pair<string, unsigned int>& t_actual = itFullList.Siguiente();

                    // si pertenece le sumamos el valor
                    if(t_actual.first == t_merge.first){
                        t_actual.second += t_merge.second;
                        esMiembro = true;
                    }
                }

                // si no pertenece lo agregamos a la lista
                if(!esMiembro){
                    fullMap.tabla[index]->push_front(t_merge);
                }
            }
        }
    }

    /** calculamos el maximo y lo retornamos **/
    return fullMap.maximum(p_maximos);
}


pair<string, unsigned int> ConcurrentHashMap::maximum_ej6(unsigned int p_archivos, unsigned int p_maximos, list<string> archs){

    ConcurrentHashMap h = count_words_ej4(p_archivos, archs);

    return h.maximum(p_maximos);

}


/*
void addAndInc(string key): Si key existe, incrementa su valor, si no existe, crea el par
(key, 1). Se debe garantizar que sólo haya contención en caso de colisión de hash. Esto es,
deberá haber locking a nivel de cada elemento del array.
*/

void ConcurrentHashMap::addAndInc(string key){    
    /** obtenemos acceso a la lista correspondiente y la lockeamos **/
    int index = fHash(key[0]);
    locks_lista[index].lock();
    
    /** agregamos o incrementamos segun corresponda **/
    bool pertenece = false;
    for (auto it = tabla[index]->CrearIt(); it.HaySiguiente(); it.Avanzar()) {
        pair<string, unsigned int>& t = it.Siguiente();
        if(t.first == key){
            t.second++;
            pertenece = true;
        }
    }

    if(!pertenece){
        pair<string, unsigned int> value = make_pair(key,1);
        tabla[index]->push_front(value);
    }
    /** habilitamos acceso a la lista para otros threads que hagan addAndInc() **/
    locks_lista[index].unlock();

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

    Mutex lock_iterador;
    Mutex lock_valor_maximo;

    Multithreading_data* data = new Multithreading_data();
    data->iterador_siguiente_nodo = tabla[0]->CrearIt();
    data->tabla = tabla;
    data->lock_iterador = lock_iterador;
    data->lock_valor_maximo = lock_valor_maximo;

    for(int i = 0; i < CANT_ENTRADAS; i++){
        locks_lista[i].lock();
    }
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

    for(int i = 0; i < CANT_ENTRADAS; i++){
        locks_lista[i].unlock();
    }

    return make_pair(data->max_key, data->max_value);
}

int ConcurrentHashMap::fHash(char x){
    return (int)x - 97;
}