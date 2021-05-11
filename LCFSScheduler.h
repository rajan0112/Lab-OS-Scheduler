#pragma once
#include "BasicScheduler.h"
#include<stack>
//last-come-first-serve scheduler
//As simple as it is too...
class LCFSScheduler : public BasicScheduler {
public:
	LCFSScheduler() {}

	~LCFSScheduler() {}

	LCFSScheduler(const LCFSScheduler&) = delete;

	void add_process(Process *p);
	int get_scheduler_algo () { return 1 ; } // FCFS=0, LCFS=1, SRTF=2, RR=3, PRIO=4, PREPRIO=5
	Process* get_next_process();
	int check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose);
private:
	std::stack<Process*> q;
};
