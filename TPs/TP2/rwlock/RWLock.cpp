#include "RWLock.h"

RWLock :: RWLock() {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);
    readers = 0;
    writers = 0;
    writing = false;
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
        // notificamos a un escritor, si hay alguno
        signal();
    unlock();
}

void RWLock :: wlock() {
    lock();
    writers++;
    while (writing || readers > 0)
        // esperamos a que nos notifique el escritor actual
        // o el ultimo lector
        wait();
    writing = true;
    unlock();
}

void RWLock :: wunlock() {
    lock();
    writers--;
    writing = false;
    // avisamos a todos
    // si hay escritores sigue alguno
    // y los lectores vuelven a dormir
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