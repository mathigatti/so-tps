#ifndef __SCHED_PSJF__
#define __SCHED_PSJF__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

struct PSJF {

	bool operator()(const int& pid1, const int& pid2){
		
		/** params[0]==prioridad | params[1]==tiempo_cpu **/
		vector<int>* v1 = tsk_params(pid1);
		vector<int>* v2 = tsk_params(pid2);

		return (*v1)[1] > (*v2)[1]; 	// La queue da mayor prioridad a los tiempos mas chicos
	};

};

typedef std::priority_queue<int, std::vector<int>, PSJF > pqueue;
typedef std::vector<pqueue> pvector;

class SchedPSJF : public SchedBase {
	public:
		SchedPSJF(std::vector<int> argn);
        ~SchedPSJF();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		pvector qs;
		int sig(); 					// Retorna el pid de la tarea mas prioritaria en espera
		int siggyPop(); 			// Retorna el pid de la tarea mas prioritaria en ready para correrla (la elimina de la espera)
		int prioridad(int pid);		// Devuelve la prioridad de la tarea del argumento (1 a 5)
		int dameLaPosta(int pidActual, int pidPrioritaria); 	// Define el proceso a correr entre los dados. Quita de espera al que corre y deja al otro en queues

		/**
		 *	Queues: 
		 *		- es un vector con cinco colas de prioridad por tiempos (a menor tiempo mayor prioridad)
		 *		- cada indice del vector va a una cola que representa las prioridades (0>>4)
		 *		- todo pid alojado representa una tarea ready, ordenado por prioridad y tiempo total de CPU (en ese orden)
		 **/

};

#endif
