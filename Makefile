CXX=g++
CXXFLAGS=-std=c++11 -O2 -Wall -Wno-reorder -I.
DEPS = Common.h \
	   Process.h \
	   Event.h \
	   BasicScheduler.h \
	   FCFSScheduler.h \
	   LCFSScheduler.h \
	   SRTFScheduler.h \
	   RRScheduler.h \
	   PRIOScheduler.h \
	   PREPRIOScheduler.h \
	   EventSimulator.h
OBJ = scheduler.o \
	  EventSimulator.o \
	  Event.o \
	  FCFSScheduler.o \
	  LCFSScheduler.o \
	  SRTFScheduler.o \
	  RRScheduler.o \
	  PRIOScheduler.o \
	  PREPRIOScheduler.o \

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

sched: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f *.o sched
