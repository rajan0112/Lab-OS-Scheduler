#pragma once
#include"Common.h"
#include"Process.h"
enum EVENT_ACTION {
	TRANS_TO_READY,
	TRANS_TO_RUN,
	TRANS_TO_PREEMPT,
	TRANS_TO_BLOCK,
	TRANS_TO_FINISH
};
class Event {
public:
	Event() = delete;

	Event(EVENT_ACTION _T,
		Process* _p,
		timestamp_t _t):transitionType(_T),evtProcess(_p),evtTimestamp(_t) {
		//whenever create a new event, the ID increase 1
		evtId = globalEventId;
		globalEventId++;
	}

	//remove copy construction, avoid affecting id counting
	Event(const Event&) = delete;

	//used for compare two events
	bool operator < (const Event& e) {
		if (evtTimestamp != e.evtTimestamp) return evtTimestamp < e.evtTimestamp;
		return this->evtId < e.evtId;
	}

	//The state that the process will be transfer to
	EVENT_ACTION transitionType;

	//the process that event handles
	Process* evtProcess;

	timestamp_t evtTimestamp;

private:
	ID_t evtId;
	
	//whenever create a new event, the ID increase 1
	//initialized as 0
	static ID_t globalEventId;
};

//used for compare two events
struct evtPtrLess {
	bool operator() (Event* e1, Event* e2) {
		return (*e1) < (*e2);
	}
};

struct evtPtrGreater {
	bool operator()(Event* e1, Event* e2) {
		return (*e2) < (*e1);
	}
};
