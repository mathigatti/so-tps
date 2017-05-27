#include <cassert>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>   
#include "RWLock.h"

using namespace std;

unsigned int shared_data = 0;
RWLock rwl;


enum OperationType {
    Read,
    Write
};

struct log {
    OperationType type;
    unsigned int data_value;
};

vector<log> logs;
pthread_mutex_t logs_mutex;

void * reader(void * data);
void * writer(void * data);

void mutual_exclusion();
void multiple_readers();
void single_writer();
void starvation_free();
void general_test();
void starvation_free_bigger();

int main(int argc, char ** argv) {
    pthread_mutex_init(&logs_mutex, NULL);

    //----- TESTS BEGIN -------
    mutual_exclusion();  
    multiple_readers();
    single_writer();
    starvation_free();
    general_test();
    starvation_free_bigger();
    //------ TESTS END --------

    pthread_mutex_destroy(&logs_mutex);
    return 0;
}

void mutual_exclusion() {
    cout << "mutual_exclusion... ", cout.flush();
    logs.clear();
    shared_data = 0;

    rwl.rlock();

    pthread_t w;
    pthread_create(&w, NULL, writer, NULL);

    usleep(10000);

    assert(logs.size() == 0);

    rwl.runlock();

    usleep(10000);

    assert(logs.size() == 1);
    assert(logs[0].type == Write);
    assert(logs[0].data_value == 1);

    rwl.wlock();

    pthread_t r;
    pthread_create(&r, NULL, reader, NULL);

    usleep(10000);

    assert(logs.size() == 1);

    rwl.wunlock();

    pthread_join(w, NULL);
    pthread_join(r, NULL);

    assert(logs.size() == 2);
    assert(logs[1].type == Read);
    assert(logs[1].data_value == 1);

    cout << "passed" << endl;
}

void multiple_readers() {
    cout << "multiple_readers... ", cout.flush();
    rwl.rlock();
    rwl.rlock();

    rwl.runlock();
    rwl.runlock();
 
    assert(true);

    cout << "passed" << endl;
}

void single_writer() {
    cout << "single_writer... ", cout.flush();
    logs.clear();
    shared_data = 4;
    rwl.wlock();

    pthread_t w;
    pthread_create(&w, NULL, writer, NULL);

    usleep(10000);

    assert(logs.size() == 0);

    rwl.wunlock();

    pthread_join(w, NULL);

    assert(logs.size() == 1);
    assert(logs[0].type == Write);
    assert(logs[0].data_value == 5);

    cout << "passed" << endl;
}

void starvation_free() {
    cout << "starvation_free... ", cout.flush();
    logs.clear();
    shared_data = 0;

    rwl.rlock();

    pthread_t w1;
    pthread_create(&w1, NULL, writer, NULL);

    usleep(10000);

    assert(logs.size() == 0);

    pthread_t r1;
    pthread_create(&r1, NULL, reader, NULL);

    usleep(10000);

    assert(logs.size() == 0);

    pthread_t w2;
    pthread_create(&w2, NULL, writer, NULL);

    usleep(10000);

    assert(logs.size() == 0);

    pthread_t r2;
    pthread_create(&r2, NULL, reader, NULL);

    usleep(10000);

    assert(logs.size() == 0);

    rwl.runlock();
   
    pthread_join(w1, NULL);
    pthread_join(w2, NULL);
    pthread_join(r1, NULL);
    pthread_join(r2, NULL);
 
    assert(logs.size() == 4);
    assert(logs[0].type == Write);
    assert(logs[0].data_value == 1);
    assert(logs[1].type == Write);
    assert(logs[1].data_value == 2);
    assert(logs[2].type == Read);
    assert(logs[2].data_value == 2);
    assert(logs[3].type == Read);
    assert(logs[3].data_value == 2);

    cout << "passed" << endl;
}

void general_test() {
    cout << "general_test... ", cout.flush();
    logs.clear();
    shared_data = 0;
    unsigned int thread_count = 4000;
    pthread_t threads[thread_count];
   
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    srand((time_t)ts.tv_nsec);


    for(unsigned int i = 0; i < thread_count; i++)
        pthread_create(&threads[i], NULL, rand() % 2 ? reader : writer, NULL); 
   

    for(unsigned int i = 0; i < thread_count; i++)
        pthread_join(threads[i], NULL);

    unsigned int assert_data = 0;
    assert(logs.size() == thread_count);
   
    for(unsigned int i = 0; i < thread_count; i++) {
        if(logs[i].type == Write) {
            assert_data++;
            assert(logs[i].type == Write);
        }
        else
            assert(logs[i].type == Read);

        assert(logs[i].data_value ==  assert_data);
    }
    cout << "passed" << endl;
}

void starvation_free_bigger() {
    cout << "starvation_free_bigger... ", cout.flush();
    logs.clear();
    shared_data = 0;

    unsigned int initial_simultaneous_readers_count = 2500;
    unsigned int writer_thread_count = 1000;
    unsigned int reader_thread_count = 8000;
    pthread_t w_threads[writer_thread_count];
    pthread_t r_threads[reader_thread_count];
   

    for(unsigned int i = 0; i < initial_simultaneous_readers_count; i++)
        rwl.rlock();

    // garantizamos el primer wlock, o los el primer thread podria ser de
    // lectura
    pthread_create(&w_threads[0], NULL, writer, NULL);
    usleep(10000);


    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    srand((time_t)ts.tv_nsec);   

    unsigned int j, k;
    for(j = 1, k = 0; j + k < writer_thread_count + reader_thread_count;) {
        if(j < writer_thread_count && rand() % 2 == 0) {
           pthread_create(&w_threads[j], NULL, writer, NULL);
           j++;
        }
        else {
           pthread_create(&r_threads[k], NULL, reader, NULL);
           k++;
        }
    }

    usleep(500000);

    for(unsigned int i = 0; i < initial_simultaneous_readers_count; i++)
        rwl.runlock();

    for(unsigned int i = 0; i < writer_thread_count; i++)
        pthread_join(w_threads[i], NULL);

    for(unsigned int i = 0; i < reader_thread_count; i++)
        pthread_join(r_threads[i], NULL);

    unsigned int assert_data = 0;
    assert(logs.size() == writer_thread_count + reader_thread_count);
   
    for(unsigned int i = 0; i < writer_thread_count + reader_thread_count; i++) {
        if(i < writer_thread_count) {
            assert_data++;
            assert(logs[i].type == Write);
        }
        else
            assert(logs[i].type == Read);

        assert(logs[i].data_value ==  assert_data);
    }
    cout << " passed" << endl;
}

void * reader(void * data) {
    log l;
    l.type = Read;

    rwl.rlock();
    l.data_value = shared_data;

    pthread_mutex_lock(&logs_mutex);
    logs.push_back(l);
    pthread_mutex_unlock(&logs_mutex);

    rwl.runlock();
    pthread_exit(NULL);
}


void * writer(void * data) {
    log l;
    l.type = Write;

    rwl.wlock();
    l.data_value = ++shared_data;

    pthread_mutex_lock(&logs_mutex);
    logs.push_back(l);
    pthread_mutex_unlock(&logs_mutex);
    rwl.wunlock();
    
    pthread_exit(NULL);
}