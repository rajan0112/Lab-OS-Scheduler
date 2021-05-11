#include"PREPRIOScheduler.h"
#include<iostream>
void
PREPRIOScheduler::add_process(Process* p) {
	//if a process have -1 priority
	//put it into expired queue
	if (p->prio == -1) {
		p->prio = p->static_prio - 1;
		(*expired)[p->prio].push(p);
	}
	else {
		(*ready)[p->prio].push(p);
	}
}

Process*
PREPRIOScheduler::get_next_process() {
	//first try to find a process in ready queue
	//if ready queue is empty, swap ready and expired
	//otherwise, it means we dont have any process in the queue left
	for (int i = maxPriority-1; i >= 0; --i) {
		if (!(*ready)[i].empty()) {
			Process* ret = (*ready)[i].front();
			(*ready)[i].pop();
			return ret;
		}
	}
	std::swap(ready, expired);
	for (int i = maxPriority - 1; i >= 0; --i) {
		if (!(*ready)[i].empty()) {
			Process* ret = (*ready)[i].front();
			(*ready)[i].pop();
			return ret;
		}
	}
	return nullptr;
}

int
PREPRIOScheduler::check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose) {
	
	if(incoming_proc == 0 || running_process == 0)	return false;
	if(incoming_proc->prio > running_process->prio)	return true; else return false;
	//return true;
}
