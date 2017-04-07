#include <vector>
#include <queue>
#include <list>
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	int cant_cores = argn[0];

	cpu_quantum = std::vector<int>(cant_cores);
	quantums_restantes = std::vector<int>(cant_cores);

	for(int i = 0; i < cant_cores; i++){

		cpu_quantum[i] = argn[i + 1];
		quantums_restantes[i] = argn[i + 1];
	}

}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) {

	cola_ready.push_back(pid); //Entra en la cola ready

}

void SchedRR::unblock(int pid) {

	cola_ready.push_back(pid); //Vuelve a la cola ready

}

int SchedRR::tick(int cpu, const enum Motivo m) {

	int pid = current_pid(cpu);

	switch (m) {
		case TICK:
			if(pid == IDLE_TASK) {
				// No estabamos haciendo nada, 3legimos el proximo proceso
				pid = next(cpu);
			} else if(--quantums_restantes[cpu] == 0) {
				// Se acabo el quantum, reencolamos este proceso porque todavia no termino
				cola_ready.push_back(pid);
				// Elegimos el proximo proceso
				pid = next(cpu);
			}
			break;
		case EXIT:
			// Por si en el ciclo anterior paso a ready
			cola_ready.remove(pid);
			// Cambiamos el pid si termino el proceso
			pid = next(cpu);
			break;
		case BLOCK:
			// Bloqueo el proceso, vamos al proximo, no se reinserta en la cola porque eso ocurre en el unblock
			pid = next(cpu);
			break;
	}

return pid;

}

int SchedRR::next(int cpu) {

	reset_quantum(cpu);
	return get_next_from_queue();

}

void SchedRR::reset_quantum(int cpu) {

	quantums_restantes[cpu] = cpu_quantum[cpu];

}

int SchedRR::get_next_from_queue() {

	int next_pid = IDLE_TASK;

	if(!cola_ready.empty()) {

		next_pid = cola_ready.front();
	 	cola_ready.pop_front();

	}

	return next_pid;
}
