#pragma once
#include "Process.h"
//a general interface for Schedulers
//only provide two method
//1. add a process to runqueue
//2. get next process from runqueue to run
class BasicScheduler {
public:
	virtual ~BasicScheduler() {}
	virtual int get_scheduler_algo() = 0;
	virtual void add_process(Process* p) = 0;
	virtual Process* get_next_process() = 0;
	virtual int check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose) = 0;
	
};
