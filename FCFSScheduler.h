#pragma once
#include "BasicScheduler.h"
#include<queue>
//first-come-first-server scheduler
//As simple as it is :)
class FCFSScheduler : public BasicScheduler {
public:
	FCFSScheduler() {}

	~FCFSScheduler() {}

	FCFSScheduler(const FCFSScheduler&) = delete;

	void add_process(Process *p);
	int get_scheduler_algo() {return 0; }// FCFS=0, LCFS=1, SRTF=2, RR=3, PRIO=4, PREPRIO=5 
	Process* get_next_process();
	int check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose);
private:
	std::queue<Process*> q;
};
