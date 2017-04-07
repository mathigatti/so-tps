export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_fcfs.o sched_rr.o
MAIN=simusched
GRAPH=graphsched.py
HERE=./
FCFS=SchedFCFS
RR=SchedRR
OUT_FOLDER=out/$@
TASKS_FOLDER=tasks/$@

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

ejercicio1: $(MAIN)
	mkdir -p $(OUT_FOLDER)
	$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 1 0 5 $(FCFS) | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@.png

ejercicio2: $(MAIN)
	mkdir -p $(OUT_FOLDER)
	$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 1 2 5 $(FCFS) | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-1_core.png
	$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 2 2 5 $(FCFS) | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-2_core.png
	$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 3 2 5 $(FCFS) | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-4_core.png

ejercicio3: $(MAIN)
	mkdir -p $(OUT_FOLDER)
	$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 2 2 5 $(FCFS) | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-2_core.png
	$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 3 2 5 $(FCFS) | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-4_core.png

ejercicio4: $(MAIN)
		mkdir -p $(OUT_FOLDER)
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 1 2 5 $(RR) 5 | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-1.png
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 2 2 5 $(RR) 5 2 | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-2.png

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h sched_rr.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)

new: clean all



