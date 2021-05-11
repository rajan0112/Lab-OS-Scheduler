#include"SRTFScheduler.h"

void
SRTFScheduler::add_process(Process* p) {
	q.push(std::make_pair(scheID++,p));
}

Process*
SRTFScheduler::get_next_process() {
	if (q.empty())return nullptr;
	else {
		Process* ret = q.top().second;
		q.pop();
		return ret;
	}
}

int 
SRTFScheduler::check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose) {
	return 0;
}