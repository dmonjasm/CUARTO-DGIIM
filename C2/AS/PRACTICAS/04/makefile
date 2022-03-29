ATT = $(EXE:=.att)
DAT = $(EXE:=.data) $(EXE:=.data.old)
EXE = $(basename $(SRC))
LOG = $(EXE:=.log)
SRC = $(wildcard *.c *.cc)

CFLAGS = -march=native -O3 -pthread -Wall
CXXFLAGS = $(CFLAGS)

all: $(EXE)

att: $(ATT)

clean:
	-rm -fv $(ATT) $(DAT) $(EXE) $(LOG) core.* *~

exe: $(EXE)

medir.nulo.gb: CFLAGS+=-lbenchmark

%.att: %
	objdump -Cd $< > $@

%.log: %
	valgrind --tool=callgrind --branch-sim=yes --cache-sim=yes --callgrind-out-file=$*.log --collect-bus=yes --collect-jumps=yes --dump-instr=yes --dump-line=yes -- ./$<

%.cg: %.log
	kcachegrind $<

%.data: %
#	perf record -e cycles:ku -F max -g -o $@ -- ./$<
	perf record -g -o $@ -- ./$<

%.perf: %.data
	perf report -g 'graph,0.5,caller' -i $<

.PHONY: all att clean exe
.PRECIOUS: $(DAT) $(LOG)
