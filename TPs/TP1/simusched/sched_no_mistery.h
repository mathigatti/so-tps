#ifndef __SCHED_No_Mistery__
#define __SCHED_No_Mistery__

#include <vector>
#include <queue>
#include <algorithm>
#include "basesched.h"

using namespace std;

struct ShortestJob{

	bool  operator()(const int& pid1, const int& pid2){
		vector<int>* v1 = tsk_params(pid1);
		vector<int>* v2 = tsk_params(pid2);
		
		if(v1[0] == v2[0]){
			// Si duran lo mismo desempato por pid mas pequenio
			return pid1 > pid2;
		} 

		return v1[0] > v2[0];
	};

};

class SchedNoMistery : public SchedBase {
	public:
		SchedNoMistery(std::vector<int> argn);
        ~SchedNoMistery();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		std::priority_queue<int, std::vector<int>, ShortestJob > q;

};

#endif
