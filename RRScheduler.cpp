#include"RRScheduler.h"

void
RRScheduler::add_process(Process* p) {
	p->prio = p->static_prio - 1;
	q.push(p);
}

Process*
RRScheduler::get_next_process() {
	if (q.empty())return nullptr;
	else {
		Process* ret = q.front();
		q.pop();
		return ret;
	}
}

int 
RRScheduler::check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose) {
	return 0;
}