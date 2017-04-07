#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include <list>
#include "basesched.h"

class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		std::list<int> cola_ready;
		std::vector<int> cpu_quantum;
		std::vector<int> quantums_restantes;
		int next(int cpu);
		void reset_quantum(int cpu);
		int get_next_from_queue();
};

#endif
