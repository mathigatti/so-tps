#include "RWLock.h"

RWLock :: RWLock() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
    readers = 0;
    writers = 0;
}

RWLock :: ~RWLock() {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
}

void RWLock :: rlock() {
    lock();
    while(writers > 0)
        // esperamos a que terminen los escritores;
        wait();
    readers++;
    unlock();
}

void RWLock :: runlock() {
    lock();
    readers--;
    if(readers == 0)
        // notificamos a los escritores, de haber esperando
        broadcast();
    unlock();
}

void RWLock :: wlock() {
    lock();
    writers++;
    while (readers > 0)
        // esperamos a que nos notifique el escritor actual
        // o el ultimo lector
        wait();
    unlock();
}

void RWLock :: wunlock() {
    lock();
    writers--;
    if(writers == 0)
        // notificamos a los lectores 
        broadcast();
    unlock();
}

void RWLock :: lock() {
    pthread_mutex_lock(&mutex);
}

void RWLock :: unlock() {
    pthread_mutex_unlock(&mutex);
}

void RWLock :: wait() {
    pthread_cond_wait(&condition, &mutex);
}

void RWLock :: signal() {
    pthread_cond_signal(&condition);
}

void RWLock :: broadcast() {
    pthread_cond_broadcast(&condition);
}