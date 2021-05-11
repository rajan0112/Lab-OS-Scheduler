#pragma once
#include"Common.h"
enum PROC_STATE
{
	CREATED,
	READY,
	RUNNING,
	BLOCK,
	FINISH
};

class Process {
public:

	//static attributes for the process
	const ID_t procId;
	const timestamp_t arrivalTime;
	const timestamp_t maxCPUBurst;
	const timestamp_t totalCPUTime;
	const timestamp_t maxIOBurst;
	
	priority_t static_prio;

	//dynamic informations
	timestamp_t cpuRemain;
	timestamp_t cbRemain;
	timestamp_t ibRemain;
	priority_t prio;
	timestamp_t stateTS;
	PROC_STATE state;

	//used for summarize
	timestamp_t finishTime;
	timestamp_t turnaroundTime;
	timestamp_t IOTime;
	timestamp_t cpuWatingTime;
 
	Process() = delete;

	Process(const ID_t _id,
		const timestamp_t _at,
		const timestamp_t _tc,
		const timestamp_t _cb,
		const timestamp_t _io,
		const priority_t _prio)
		:procId(_id), arrivalTime(_at), cpuRemain(_tc), maxCPUBurst(_cb), maxIOBurst(_io), totalCPUTime(_tc),
		cbRemain(0), ibRemain(0), stateTS(_at), state(CREATED),static_prio(_prio),
		IOTime(0), cpuWatingTime(0),prio(_prio - 1){}

	Process(const Process&) = delete;

private:
	
};
