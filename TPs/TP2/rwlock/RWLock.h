#ifndef RWLock_h
#define RWLock_h
#include <iostream>
#include <pthread.h>

class RWLock {
    private:
        unsigned int readers;
	unsigned int writers;
	unsigned int writing;
	pthread_mutex_t mutex;
	pthread_cond_t condition;

	void lock();
	void unlock();
	void wait();
	void signal();
	void broadcast();
	
    public:
        RWLock();
        ~RWLock();
        void rlock();
        void runlock();
        void wlock();
        void wunlock();
};

#endif
