#pragma once
#include "BasicScheduler.h"
#include<queue>
//round robin scheduler
//well, it's just a tiny improved version of FCFS
class RRScheduler : public BasicScheduler {
public:
	RRScheduler() {}

	~RRScheduler() {}

	RRScheduler(const RRScheduler&) = delete;

	void add_process(Process *p);
	int get_scheduler_algo () { return 3; } // FCFS=0, LCFS=1, SRTF=2, RR=3, PRIO=4, PREPRIO=5
	Process* get_next_process();
	int check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose);
private:
	std::queue<Process*> q;
};
