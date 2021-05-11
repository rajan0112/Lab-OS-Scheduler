#pragma once
#include "BasicScheduler.h"
#include<queue>
//shortest job first scheduler
//a job will comes first if:
//1. it has smallest cpu running time remaining
//2. it goes in to queue eairlier if there is a tie in 1
//Efficiency:
//Constant time to find next process
//log(n) for insertion and deletion
class SRTFScheduler : public BasicScheduler {
public:
	SRTFScheduler() {
		scheID = 0;
	}

	~SRTFScheduler() {}

	SRTFScheduler(const SRTFScheduler&) = delete;

	void add_process(Process *p);
	int get_scheduler_algo() { return  2;} // FCFS=0, LCFS=1, SRTF=2, RR=3, PRIO=4, PREPRIO=5
	Process* get_next_process();
	int check_preemption(Process* incoming_proc, Process* running_process, int current_time, bool verbose);
private:
	ID_t scheID;
	struct SRTFGreater {
		bool operator()(std::pair<ID_t, Process*> p1, std::pair<ID_t, Process*> p2) {
			/*if (p1.second->cbRemain != p2.second->cbRemain){
				return p1.second->cbRemain > p2.second->cbRemain;
			}*/
			
			if (p1.second->cpuRemain != p2.second->cpuRemain){
				return p1.second->cpuRemain > p2.second->cpuRemain;
			}
			else{
				return p1.first > p2.first;
			}
		}
	};
	std::priority_queue<std::pair<ID_t, Process*>, 
		std::vector<std::pair<ID_t, Process*> >,
	   	SRTFGreater> q;
};
