#include <vector>
#include <queue>
#include <list>
#include "sched_psjf.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedPSJF::SchedPSJF(vector<int> argn) {
	// Inicializo la estructura de task en ready
	for(int i=0; i<5; i++){
		pqueue pq;
		qs.push_back(pq);
	}
}

SchedPSJF::~SchedPSJF() {
	// Nada por aqui!
}

void SchedPSJF::load(int pid) {
	/** llego una tarea nueva -> chequeo su prioridad e inserto en la queue correspondiente **/
	int pr = prioridad(pid);
	qs[pr-1].push(pid);
}

void SchedPSJF::unblock(int pid) {
	// Las tasks priorizadas no son bloqueantes!
}

int SchedPSJF::tick(int cpu, const enum Motivo m) {
	
	int pidActual = current_pid(cpu);
	int pidPrioritaria = sig();

	bool hayTarea = pidPrioritaria != IDLE_TASK;
	bool chauTarea = m == EXIT;
	bool chauIdle = (pidActual == IDLE_TASK) && hayTarea;
	
	if (chauTarea || chauIdle) {
		// Si el pid actual termino, o corre la idle y hay tareas por correr, corro la tarea que corresponda a continuacion
		return siggyPop();
	} else {
		if(hayTarea){
			// Competencia! Chequeo si debe seguir o hay una tarea mas prioritaria a correr
			return dameLaPosta(pidActual, pidPrioritaria);
		} else {
			// Sigue corriendo la tarea actual
			return pidActual;
		}
	}
}

/** Retorna el pid de la tarea mas prioritaria en espera **/
int SchedPSJF::sig(){
	for (int i = 0; i < 5; i++){	// Recorro de mayor prioridad a menor las qs (0>>4)
		if (!(qs[i].empty())){
			return qs[i].top();
		}
	}

	return IDLE_TASK; 	// De no haber retorna IDLE
}

/** Retorna el pid de la tarea mas prioritaria en espera, eliminandola de las qs de ready **/
int SchedPSJF::siggyPop(){

	int siggy(IDLE_TASK);
	for (int i = 0; i < 5; i++){	// Recorro de mayor prioridad a menor las qs (0>>4)
		if (!qs[i].empty()){
			siggy = qs[i].top();
			qs[i].pop();
			return siggy;
		}
	}

	return siggy;	// De no haber retorna IDLE
}

/** Devuelve la prioridad de la tarea del argumento (1 a 5) **/
int SchedPSJF::prioridad(int pid) {
	vector<int>* v_pid = tsk_params(pid);
	return (*v_pid)[0];
}

/**
 *	Define el proceso a correr entre los dados. Quita de espera al que corre y deja al otro en qs
 *	Retorna el proceso más prioritario, que queda fuera de la estructura de qs de procesos en ready
 *	Ademas, deja en ready al proceso de menor prioridad segun corresponda
 **/
int SchedPSJF::dameLaPosta(int pidActual, int pidPrioritaria){
	/** tsk_params -> params[0]==prioridad | params[1]==tiempo_cpu **/
	vector<int>* v_act = tsk_params(pidActual);
	vector<int>* v_pri = tsk_params(pidPrioritaria);

	int prioridad_actual = (*v_act)[0];
	int prioridad_prioritaria = (*v_pri)[0];
	int tiempos_actual = (*v_act)[1];
	int tiempos_prioritaria = (*v_pri)[1];

	int pidWinner = pidActual;	// Si pidWinner es el actual, sigue corriendo y nada hay que hacer

	/** Si en cambio el pidWinner es la tarea prioritaria en ready, hay que desalojarla y guardar al pidActual para una futura ejecucion **/
	if(prioridad_actual > prioridad_prioritaria || (prioridad_actual == prioridad_prioritaria && tiempos_actual > tiempos_prioritaria)){
		
		// Actualizo el nuevo pid
		pidWinner = pidPrioritaria;

		// Desalojo de ready
		qs[prioridad_prioritaria-1].pop();

		// Guardo el anterior
		qs[prioridad_actual-1].push(pidActual);
	}

	return pidWinner;
}
