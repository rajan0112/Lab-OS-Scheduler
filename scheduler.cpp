#include<vector>
#include<string>
#include<fstream>
#include<iostream>
#include<stdio.h>
#include<unistd.h>

#include"EventSimulator.h"
#include"Process.h"
#include"FCFSScheduler.h"
#include"LCFSScheduler.h"
#include"SRTFScheduler.h"
#include"RRScheduler.h"
#include"PRIOScheduler.h"
#include"PREPRIOScheduler.h"

int verbose = 0;

void init_process(std::vector<Process*>& processList,
	EventSimulator* DES,
	std::string inputFilePath, int maxPriority) {
	std::ifstream inputFile(inputFilePath);
	if (!inputFile.is_open()) {
		std::cerr << inputFilePath << " Open Failed!\n";
		exit(-3);
	}
	ID_t pid = 0;
	timestamp_t AT, TC, CB, IO;
	while (inputFile >> AT >> TC >> CB >> IO) {
		Process* ptrP = new Process(pid, AT, TC, CB, IO, DES->myrandom(maxPriority));
		processList.push_back(ptrP);
		DES->add_event(new Event(TRANS_TO_READY, ptrP,AT));
		pid++;
	}
	inputFile.close();
}

void print_summary(std::vector<Process*>& processList,
	EventSimulator* DES) {
	//print thread message:
	timestamp_t totalTT = 0;
	timestamp_t totalCW = 0;
	for (size_t id = 0; id < processList.size(); ++id) {
		Process* ptrP = processList[id];
		totalTT += ptrP->finishTime - ptrP->arrivalTime;
		totalCW += ptrP->cpuWatingTime;
		printf("%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
			(int)id, 
			ptrP->arrivalTime, 
			ptrP->totalCPUTime,
			ptrP->maxCPUBurst, 
			ptrP->maxIOBurst, 
			ptrP->static_prio,
			ptrP->finishTime,
			ptrP->finishTime - ptrP->arrivalTime, 
			ptrP->IOTime, 
			ptrP->cpuWatingTime);
	}
	printf("SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
		DES->lastEventFinish,
		(double)(100.00) * DES->CPUoccupation / DES->lastEventFinish,
		(double)(100.00) * DES->IOoccupation / DES->lastEventFinish,
		(double)(1.00) * totalTT / processList.size(),
		(double)(1.00) * totalCW / processList.size(),
		(double)(100.00) * processList.size() / DES->lastEventFinish);
}

void destroy_process(std::vector<Process*>& processList) {
	for (auto ptrP : processList) {
		delete ptrP;
	}
}

void display_help_message(){
	std::cout<<"Invalid Option.\nUsage: sched [-v] [-s<schedspec>] inputFile randFile\n";
}

int main(int argc, char* argv[]) {
	
	std::string SchedulerOption;
	std::string inputFilePath;
	std::string randFilePath;
	int c;
	//parsing command line options
	while((c = getopt(argc, argv, "hvs:")) != -1){
		switch(c){
			case 'h':
				display_help_message();
				break;
			case 'v':
				verbose = 1;
				break;
			case 's':
				SchedulerOption = std::string(optarg);
				break;
			default:
				display_help_message();
				exit(1);
		}
	}
	if(optind+2 != argc){
		display_help_message();
		exit(1);
	}
	inputFilePath = std::string(argv[optind]);
	randFilePath = std::string(argv[optind+1]);
	if(SchedulerOption.length() == 0){
		display_help_message();
		exit(1);
	}
	BasicScheduler* scheduler;
	timestamp_t q = MAX_TS;
	int maxPriority = 4;

	switch(SchedulerOption[0]){
		case 'F':
			scheduler = new FCFSScheduler();
			SchedulerOption = "FCFS\n";
			break;
		case 'L':
			scheduler = new LCFSScheduler();
			SchedulerOption = "LCFS\n";
			break;
		case 'S':
			scheduler = new SRTFScheduler();
			SchedulerOption = "SRTF\n";
			break;
		case 'R':
			scheduler = new RRScheduler();
			try{
				q = stoi(SchedulerOption.substr(1));
				if(q <= 0){
					display_help_message();
					exit(1);
				}
				SchedulerOption = "RR " + std::to_string(q) + "\n";
			}
			catch(...){
				display_help_message();
				exit(1);
			}
			break;
		case 'P':
			// Scanf will return number of parameters read
			if(sscanf(SchedulerOption.substr(1).c_str(),"%d:%d",&q,&maxPriority) < 2)
			{
				maxPriority = 4;
			}
			else {
				if(q <= 0 || maxPriority <= 0) {
					display_help_message();
					exit(1);
				}
			}

			SchedulerOption = "PRIO " + std::to_string(q) + "\n";		
			scheduler = new PRIOScheduler(maxPriority);

			break;

		case 'E':
			// Scanf will return number of parameters read
			if(sscanf(SchedulerOption.substr(1).c_str(),"%d:%d",&q,&maxPriority) < 2)
				maxPriority = 4;
			else {
				if(q <= 0 || maxPriority <= 0) {
					display_help_message();
					exit(1);
				}
			}

			SchedulerOption = "PREPRIO " + std::to_string(q) + "\n";		
			scheduler = new PREPRIOScheduler(maxPriority);

			break;

		default:
			display_help_message();
			exit(1);
	}

	EventSimulator* DES = new EventSimulator(scheduler,randFilePath,q);

	//initialize processes
	std::vector<Process*> processList;
	init_process(processList, DES, inputFilePath, maxPriority);

	//start simulation
	DES->Simulation();

	//print summary messages
	std::cout<<SchedulerOption;
	print_summary(processList, DES);

	//clean up
	destroy_process(processList);
	delete scheduler;
	delete DES;
	return 0;
}
