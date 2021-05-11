#pragma once
#include "BasicScheduler.h"
#include<queue>
#include<array>
//priority based scheduler
//constant time to find next process, insertion and deletion
//This approach is simpler since we only have 4 priority levels
//will need a bit-map approch if we have more priority levels
//otherwise the searching time will be a factor of priority levels
class PRIOScheduler : public BasicScheduler {
public:
	PRIOScheduler(int maxPriority) {
//		ready = new std::array<std::queue<Process*>, 3>;
//		expired = new std::array<std::queue<Process*>, 3>;
		this->maxPriority = maxPriority;
		ready = new std::vector<std::queue<Process*>>(maxPriority);
		expired = new std::vector<std::queue<Process*>>(maxPriority);
	}

	~PRIOScheduler() {
		delete ready;
		delete expired;
	}

	PRIOScheduler(const PRIOScheduler&) = delete;

	void add_process(Process *p);
	int get_scheduler_algo () { return 4; } // FCFS=0, LCFS=1, SRTF=2, RR=3, PRIO=4, PREPRIO=5
	Process* get_next_process();
	int check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose);
private:
	//2 set of queues
	//each set of *maxPriority* differnt priority levels
	//std::array<std::queue<Process*>, 3>* ready;
	//std::array<std::queue<Process*>, 3>* expired;
	std::vector<std::queue<Process*>>* ready;
	std::vector<std::queue<Process*>>* expired;
	int maxPriority;
};
