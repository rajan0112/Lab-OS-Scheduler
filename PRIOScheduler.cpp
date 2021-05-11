#include"PRIOScheduler.h"

void
PRIOScheduler::add_process(Process* p) {
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
PRIOScheduler::get_next_process() {
	//first try to find a process in ready queue
	//if ready queue is empty, swap ready and expired
	//otherwise, means we donnot have any process in the queue left
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
PRIOScheduler::check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose) {
	return 0;
}