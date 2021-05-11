#include"FCFSScheduler.h"

void
FCFSScheduler::add_process(Process* p) {
	q.push(p);
}

Process*
FCFSScheduler::get_next_process() {
	if (q.empty())return nullptr;
	else {
		Process* ret = q.front();
		q.pop();
		return ret;
	}
}

int 
FCFSScheduler::check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose) {
	return 0;
}