#pragma once
#include"Event.h"
#include "BasicScheduler.h"
#include<queue>
#include<vector>
#include<string>
class EventSimulator {
public:
	EventSimulator() = delete;

	EventSimulator(BasicScheduler* _s,
		std::string randNumFile,
		const timestamp_t _q = MAX_TS) :scheduler(_s), quantum(_q) {
		lastEventFinish = 0;
		CPUoccupation = 0;
		IOoccupation = 0;
		nextIOEnd = 0;
		randNumPos = 0;
		init_rand_array(randNumFile);
	}

	//provide a "fake random number" in [1,burst]
	//the fake random numbers are stored in randArr
	//and will wrap around if exausted
	timestamp_t myrandom(timestamp_t burst);

	EventSimulator(const EventSimulator& _e) = delete;

	void add_event(Event*);
	void del_future_events(Process *proc, timestamp_t curTime);
	void Simulation();

	~EventSimulator() {}

	//summaries for the whole simulation
	timestamp_t lastEventFinish;

	timestamp_t CPUoccupation;

	timestamp_t IOoccupation;

private:
	//event queue
	//has O(1) for geting next event, O(log(n)) for insertion and deletion
	std::priority_queue<Event*, std::vector<Event*>, evtPtrGreater> eventQ;

	//used for random vector
	//provide "fake" random numbers
	size_t randNumPos;
	std::vector<long long> randArr;

	void init_rand_array(std::string randNumFile);

	Event* get_event();
	Event* peek_event();
	Event* anyMoreEventsForTS(Process *p, timestamp_t ts);
	timestamp_t get_next_event_time();

	BasicScheduler* scheduler;

	//time slice
	timestamp_t quantum;

	//help for the summary

	timestamp_t nextIOEnd;
};
