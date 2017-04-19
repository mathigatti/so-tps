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
		std::priority_queue<int, std::vector<int>, PSJF> queue;
		queues.push_back(queue);
	}
}

SchedPSJF::~SchedPSJF() {
	// Nada por aqui!
}

void SchedPSJF::load(int pid) {
	/** llego una tarea nueva -> chequeo su prioridad e inserto en la queue correspondiente **/
	int prioridad = prioridad(pid);
	queues[prioridad-1].push(pid);
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

/** Devuelve la prioridad de la tarea del argumento (1 a 5) **/
int prioridad(int pid){
	vector<int>* v_pid = tsk_params(pid);
	return v_pid[0];
}

/** Retorna el pid de la tarea mas prioritaria en espera **/
int sig(){
	for (int i = 0; i < 5; i++){	// Recorro de mayor prioridad a menor las queues (0>>4)
		if (!queues[i].empty()){
			return queues[i].top();
		}
	}

	return IDLE_TASK; 	// De no haber retorna IDLE
}

/** Retorna el pid de la tarea mas prioritaria en espera, eliminandola de las queues de ready **/
int siggyPop(){

	int siggy(IDLE_TASK);
	for (int i = 0; i < 5; i++){	// Recorro de mayor prioridad a menor las queues (0>>4)
		if (!queues[i].empty()){
			siggy = queues[i].top();
			queues[i].pop();
			return siggy;
		}
	}

	return siggy;	// De no haber retorna IDLE
}

/**
 *	Define el proceso a correr entre los dados. Quita de espera al que corre y deja al otro en queues
 *	Retorna el proceso más prioritario, que queda fuera de la estructura de queues de procesos en ready
 *	Ademas, deja en ready al proceso de menor prioridad segun corresponda
 **/
int dameLaPosta(int pidActual, int pidPrioritaria){
	/** tsk_params -> params[0]==prioridad | params[1]==tiempo_cpu **/
	vector<int>* v_act = tsk_params(pidActual);
	vector<int>* v_pri = tsk_params(pidPrioritaria);

	int prioridad_actual = v_act[0];
	int prioridad_prioritaria = v_pri[0];
	int tiempos_actual = v_act[1];
	int tiempos_prioritaria = v_pri[1];

	int pidWinner = pidActual;	// Si pidWinner es el actual, sigue corriendo y nada hay que hacer

	/** Si en cambio el pidWinner es la tarea prioritaria en ready, hay que desalojarla y guardar al pidActual para una futura ejecucion **/
	if(prioridad_actual < prioridad_prioritaria || (prioridad_actual == prioridad_prioritaria && tiempos_actual > tiempos_prioritaria)){
		
		// Actualizo el nuevo pid
		pidWinner = pidPrioritaria;

		// Desalojo de ready
		queues[prioridad_prioritaria-1].pop();

		// Guardo el anterior
		queues[prioridad_actual-1].push(pidActual);
	}

	return pidWinner;
}
