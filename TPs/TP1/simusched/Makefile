export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_fcfs.o sched_rr.o sched_no_mistery.o
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
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 1 2 5 $(RR) 1 5 | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-1.png
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 2 2 5 $(RR) 5 2 | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-2.png


ejercicio5: $(MAIN)
		mkdir -p $(OUT_FOLDER)
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@-1.tsk 1 0 0 SchedNoMistery | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-1.png
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@-2.tsk 2 0 0 SchedNoMistery | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-2.png

ejercicio7: $(MAIN)
		mkdir -p $(OUT_FOLDER)

		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 1 1 1 $(RR) 5 | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-rr-core1.png
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 1 1 1 SchedMistery | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-mistery-core1.png
		# FALTA AGREGAR PSJF

		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 2 1 1 $(RR) 5 5 | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-rr-core2.png
		$(HERE)$(MAIN) $(TASKS_FOLDER)/$@.tsk 2 1 1 SchedMistery | $(HERE)$(GRAPH) > $(OUT_FOLDER)/$@-mistery-core2.png
		# FALTA AGREGAR PSJF

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h sched_rr.h sched_no_mistery.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)

new: clean all



