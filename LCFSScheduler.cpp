#include"LCFSScheduler.h"

void
LCFSScheduler::add_process(Process* p) {
	q.push(p);
}

Process*
LCFSScheduler::get_next_process() {
	if (q.empty())return nullptr;
	else {
		Process* ret = q.top();
		q.pop();
		return ret;
	}
}

int 
LCFSScheduler::check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose) {
	return 0;
}