#include"EventSimulator.h"
#include<string>
#include<iostream>
#include<algorithm>
#include<fstream>

//functions below:
//used for priting help messages
namespace {
	std::string PROC_STATE_S[5] = {
		"CREATED",
		"READY",
		"RUNNG",
		"BLOCK",
		"FINISH"
	};

	void msgTransitCreatedToReady(
		const timestamp_t currTime,
		const ID_t procId,
		const timestamp_t timeInPrevState) {
		std::cout << currTime << " " << procId << " " << timeInPrevState << ": "
			<< PROC_STATE_S[CREATED] << " -> " << PROC_STATE_S[READY] << std::endl;
	}

	void msgDone(const timestamp_t currTime,
		const ID_t procId,
		const timestamp_t timeInPrevState) {
		std::cout << currTime << " " << procId << " " << timeInPrevState << ": Done\n";
	}

	void msgTransitReadyToRun(
		const timestamp_t currTime,
		const ID_t procId,
		const timestamp_t timeInPrevState,
		const timestamp_t cb,
		const timestamp_t rem,
		const priority_t prio
	) {
		std::cout << currTime << " " << procId << " " << timeInPrevState << ": "
			<< PROC_STATE_S[READY] << " -> " << PROC_STATE_S[RUNNING]
			<< " cb=" << cb << " rem=" << rem << " prio=" << prio << std::endl;
	}

	void msgTransitRunToBlock(
		const timestamp_t currTime,
		const ID_t procId,
		const timestamp_t timeInPrevState,
		const timestamp_t ib,
		const timestamp_t rem
	) {
		std::cout << currTime << " " << procId << " " << timeInPrevState << ": "
			<< PROC_STATE_S[RUNNING] << " -> " << PROC_STATE_S[BLOCK]
			<< "  ib=" << ib << " rem=" << rem << std::endl;
	}

	void msgTransitRunToReady(
		const timestamp_t currTime,
		const ID_t procId,
		const timestamp_t timeInPrevState,
		const timestamp_t cb,
		const timestamp_t rem,
		const priority_t prio
	) {
		std::cout << currTime << " " << procId << " " << timeInPrevState << ": "
			<< PROC_STATE_S[RUNNING] << " -> " << PROC_STATE_S[READY]
			<< "  cb=" << cb << " rem=" << rem << " prio=" << prio << std::endl;
	}

	void msgTransitBlockToReady(
		const timestamp_t currTime,
		const ID_t procId,
		const timestamp_t timeInPrevState
	) {
		std::cout << currTime << " " << procId << " " << timeInPrevState << ": "
			<< PROC_STATE_S[BLOCK] << " -> " << PROC_STATE_S[READY] << std::endl;
	}

	void msgTransitToPreempt(
		const timestamp_t currTime,
		const timestamp_t nextEventTime,
		const ID_t runningProcId,
		const ID_t incomingProcId,
		const bool preempt
	) {
	std::cout << "---> PRIO preemption "
		<< runningProcId
		<< " by " << incomingProcId 
		<< " ? 0 "
		<< " TS=" << nextEventTime << " now=" << currTime 
		<< ((preempt) ? ") --> YES" : ") --> NO" )
		<< std::endl;
	}
}//anonymous namespace

extern int verbose;

//DES process
void
EventSimulator::Simulation() {

	Event* evt;
	bool CALL_SCHEDULER = false;
	Process* CURR_RUNNING_PROCESS = nullptr;

	while ((evt = get_event())) {
		Process *proc = evt->evtProcess;
		timestamp_t currTime = evt->evtTimestamp;
		timestamp_t timeInPrevState = currTime - proc->stateTS;

		switch (evt->transitionType) {

		case TRANS_TO_READY: {
			// must come from BLOCKED or from PREEMPTION or from CREATED
			// must add to run queue

			if (proc->state == RUNNING) {
				//is being preempted
				proc->cbRemain -= timeInPrevState;
				proc->cpuRemain -= timeInPrevState;

				if (verbose)msgTransitRunToReady(currTime,
					proc->procId,
					timeInPrevState,
					proc->cbRemain,
					proc->cpuRemain,
					proc->prio);
				CPUoccupation += timeInPrevState;
				CURR_RUNNING_PROCESS = nullptr;

				//deal with priority
				proc->prio -= 1;
			}
			else if (proc->state == BLOCK) {
				proc->ibRemain = 0;
				proc->IOTime += timeInPrevState;
				proc->prio = proc->static_prio - 1;
				if (verbose)
					msgTransitBlockToReady(currTime,
						proc->procId,
						timeInPrevState);
				
				///////////////////////////////////
				
				bool preempt = scheduler->check_preemption(proc,CURR_RUNNING_PROCESS,currTime,verbose);
				Event *nextEvent = peek_event();
				timestamp_t nextEvtTimestamp = currTime;
				if(nextEvent != NULL)
					nextEvtTimestamp = nextEvent->evtTimestamp;

				if( preempt ) 
				{
					if(!anyMoreEventsForTS(CURR_RUNNING_PROCESS,currTime)) {
						del_future_events(CURR_RUNNING_PROCESS, currTime);
						add_event(new Event(TRANS_TO_PREEMPT, CURR_RUNNING_PROCESS, currTime));
					}
					else {
						preempt = false;
					}
				}
				
				if(verbose && CURR_RUNNING_PROCESS) {
					msgTransitToPreempt(currTime,nextEvtTimestamp,
						CURR_RUNNING_PROCESS->procId, proc->procId, preempt);

				}
				//////////////////////////////////////////////////////////////						
			}
			else if (proc->state == CREATED) {
				if (verbose)
					msgTransitCreatedToReady(currTime, proc->procId, timeInPrevState);

				///////////////////////////////////
				
				bool preempt = scheduler->check_preemption(proc,CURR_RUNNING_PROCESS,currTime,verbose);
				Event *nextEvent = peek_event();
				timestamp_t nextEvtTimestamp = currTime;
				if(nextEvent != NULL)
					nextEvtTimestamp = nextEvent->evtTimestamp;

				if( preempt ) 
				{

					if(!anyMoreEventsForTS(CURR_RUNNING_PROCESS,currTime)) {
						del_future_events(CURR_RUNNING_PROCESS, currTime);
						add_event(new Event(TRANS_TO_PREEMPT, CURR_RUNNING_PROCESS, currTime));
					}
					else {
						preempt = false;
					}
				}
				
				if(verbose && CURR_RUNNING_PROCESS) {
					msgTransitToPreempt(currTime,nextEvtTimestamp,
						CURR_RUNNING_PROCESS->procId, proc->procId, preempt);

				}
				//////////////////////////////////////////////////////////////						
			}
			else {
				std::cerr << "Illigal Transit_1! From " << PROC_STATE_S[proc->state] << " to READY" << std::endl;
				exit(-1);
			}
			proc->state = READY;
			scheduler->add_process(proc);
			CALL_SCHEDULER = true;
			break;
		}

		case TRANS_TO_RUN: {
			// must from ready
			// create event for either preemption or blocking
			if (proc->state != READY) {
				std::cerr << "Illigal Transit_2! From " << PROC_STATE_S[proc->state] << " to RUN\n";
				exit(-1);
			}

			if (proc->cbRemain == 0) {
				//create a new cpu burst
				timestamp_t newcb = std::min(myrandom(proc->maxCPUBurst), proc->cpuRemain);
				proc->cbRemain = newcb;
			}
			if (proc->cbRemain <= quantum) {
				if (proc->cbRemain >= proc->cpuRemain  ) {
					//will finish after this run
					add_event(new Event(TRANS_TO_FINISH, evt->evtProcess, currTime + proc->cpuRemain));
				}
				else {
					//will block by io after this run
					add_event(new Event(TRANS_TO_BLOCK, evt->evtProcess, currTime + proc->cbRemain));
				}
			}
			else {
				//will be preempted after this run
				add_event(new Event(TRANS_TO_READY, evt->evtProcess, currTime + quantum));
			}
			if (verbose) {
				msgTransitReadyToRun(currTime,
					proc->procId,
					timeInPrevState,
					proc->cbRemain,
					proc->cpuRemain,
					proc->prio);
			}
			proc->cpuWatingTime += timeInPrevState;
			proc->state = RUNNING;
			break;
		}

		case TRANS_TO_BLOCK: {
			//must come from running
			//create an event for when process becomes READY again
			timestamp_t newib = myrandom(proc->maxIOBurst);
			proc->cpuRemain -= proc->cbRemain;
			proc->cbRemain = 0;
			proc->ibRemain = newib;

			//deal with cpu occupation
			CPUoccupation += timeInPrevState;

			//deal with io occupation
			if (nextIOEnd < currTime + newib) {
				if (nextIOEnd >= currTime)
					IOoccupation += currTime + newib - nextIOEnd;
				else
					IOoccupation += newib;
				nextIOEnd = currTime + newib;
			}


			add_event(new Event(TRANS_TO_READY, evt->evtProcess, currTime + newib));
			if (verbose) {
				msgTransitRunToBlock(currTime,
					proc->procId,
					timeInPrevState,
					proc->ibRemain,
					proc->cpuRemain);
			}

			proc->state = BLOCK;
			CALL_SCHEDULER = true;
			CURR_RUNNING_PROCESS = nullptr;
			break;
		}

		case TRANS_TO_PREEMPT:
			// add to runqueue(no event is generated)
			//if(CURR_RUNNING_PROCESS != 0)
			//	CURR_RUNNING_PROCESS->state = READY;
			if(proc->state != RUNNING) {
				std::cerr << "Illegal state _3\n";
				exit(-1);
			}
			proc->cbRemain -= timeInPrevState;
			proc->cpuRemain -= timeInPrevState;
			CPUoccupation += timeInPrevState;

			//deal with priority
			proc->prio -= 1;
			proc->state=READY;			

			scheduler->add_process(proc);
			CALL_SCHEDULER = true;
			CURR_RUNNING_PROCESS = nullptr;
			break;

		case TRANS_TO_FINISH: {
			//must come from running
			if (verbose) {
				msgDone(currTime,
					proc->procId,
					timeInPrevState);
			}
			CPUoccupation += timeInPrevState;
			proc->finishTime = currTime;
			proc->state = FINISH;
			CURR_RUNNING_PROCESS = nullptr;
			CALL_SCHEDULER = true;
			break;
		}

		default: {
			std::cerr << "Illigal State!\n";
			exit(-2);
		}
		}

		//////////////////////////////////////////////
		proc->stateTS = currTime;
		lastEventFinish = currTime;
		delete evt;
		evt = nullptr;

		if (CALL_SCHEDULER) {
			if (get_next_event_time() == currTime) {
				continue;//process next event from Event queue
			}
			CALL_SCHEDULER = false;
			if (CURR_RUNNING_PROCESS == nullptr) {
				CURR_RUNNING_PROCESS = scheduler->get_next_process();
				if (CURR_RUNNING_PROCESS == nullptr) {
					continue;
				}
				add_event(new Event(TRANS_TO_RUN, CURR_RUNNING_PROCESS, currTime));
			}
			// create event to make process runnable for same time.
		}
	}
}

void
EventSimulator::add_event(Event* e) {
	eventQ.push(e);
}

// Delete all future events of a process
void 
EventSimulator::del_future_events(Process *proc, timestamp_t curTime) {
	std::vector<Event*> tempQ;
	if (proc == NULL) return;
	while (!eventQ.empty()) { 

		Event *evt = eventQ.top();
		if(evt->evtProcess->procId == proc->procId && evt->evtTimestamp > curTime)
			delete evt;
		else {
		    tempQ.push_back(evt);
		}
		eventQ.pop(); 
    } 
	
	if(!tempQ.empty()) { 
		for(Event *t : tempQ) {
			eventQ.push(t);
		}
	}
}

Event*
EventSimulator::get_event() {
	if (eventQ.empty())return nullptr;
	else {
		Event* ret = eventQ.top();
		eventQ.pop();
		return ret;
	}
}


Event*
EventSimulator::peek_event() {
	if (eventQ.empty())return nullptr;
	else {
		Event* ret = eventQ.top();
		return ret;
	}
}
Event* 
EventSimulator::anyMoreEventsForTS(Process *p, timestamp_t ts)
{
	std::vector<Event*> tempQ;
	Event *nextEvt  = nullptr;
	if (p == NULL) return nullptr;

	while (!eventQ.empty()) { 

		Event *evt = eventQ.top();
		if(evt->evtProcess->procId == p->procId && evt->evtTimestamp == ts)
			nextEvt = evt;
		tempQ.push_back(evt);
		eventQ.pop(); 
	}
	
	if(!tempQ.empty()) { 
		for(Event *t : tempQ) {
			eventQ.push(t);
		}
	}

	return nextEvt;
}

timestamp_t
EventSimulator::get_next_event_time() {
	if (eventQ.empty())return MAX_TS;
	else return eventQ.top()->evtTimestamp;
}

void
EventSimulator::init_rand_array(std::string randNumFile) {
	std::ifstream numFile(randNumFile);
	if (!numFile.is_open()) {
		std::cerr << randNumFile << "Open Failed.\n";
		exit(-3);
	}
	int num;
	numFile >> num;
	randArr.resize(num);
	for (int i = 0; i < num; ++i) {
		numFile >> randArr[i];
	}
	numFile.close();
}

timestamp_t
EventSimulator::myrandom(timestamp_t burst) {
	int burstNum = burst;
	int ret = 1 + (randArr[randNumPos] % burstNum);
	randNumPos = (randNumPos + 1) % randArr.size();
	return ret;
}
