#include <vector>
#include <queue>
#include <list>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {
	// FCFS recibe la cantidad de cores.
}

SchedNoMistery::~SchedNoMistery() {

}


void SchedNoMistery::load(int pid) {
	q.push(pid); // llegó una tarea nueva
//	cerr << "Cargando tarea de primer parametro " << (*tsk_params(pid))[0];
}

void SchedNoMistery::unblock(int pid) {
	// Uy! unblock!... bueno, ya seguir'a en el próximo tick
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {
	if (m == EXIT) {
		// Si el pid actual terminó, sigue el próximo.
		if (q.empty()) return IDLE_TASK;
		else {
			int sig = q.top(); q.pop();
			return sig;
		}
	} else {
		// Siempre sigue el pid actual mientras no termine.
		if (current_pid(cpu) == IDLE_TASK && !q.empty()) {
			int sig = q.top(); q.pop();
			return sig;
		} else {
			return current_pid(cpu);
		}
	}
}
