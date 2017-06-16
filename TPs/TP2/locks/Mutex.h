#ifndef Mutex_h
#define Mutex_h
#include <iostream>
#include <pthread.h>

class Mutex {

    private:
	pthread_mutex_t mutex;
	
    public:
    RWLock();
    ~RWLock();
    void lock();
	void unlock();
};

#endif
