###############################################################################

.ONESHELL:

###############################################################################

ATT = $(EXE:=.att)
CAC = $(EXE:=.cache)
DAT = $(EXE:=.dat)
EXE = $(basename $(SRC))
IJK = ijk-s ijk-i ijk-l ijk-f ijk-d ijk-ld
LOG = $(EXE:=.log) $(EXE:=.log2)
OLD = $(DAT:=.old)
OUT = $(IJK:=.out)
SRC = $(wildcard *.cc)

###############################################################################

CXXFLAGS = -g -march=native -O3 -Wall -std=c++17

###############################################################################

all: $(ATT)

att: $(ATT)

clean:
	-rm -f $(ATT) $(CAC) $(DAT) $(EXE) $(LOG) $(OLD) $(OUT) {cache,call}grind.out.* {vg,}core.* gmon.out perf.data* *~

exe: $(EXE)

sm: selfmod
	gdb ./$<

tipos: $(OUT)
	@echo -e "    short   \t     int    \t    long    \t    float   \t   double   \t long double"
	 echo -e "------------\t------------\t------------\t------------\t------------\t------------"
	paste $(OUT)

###############################################################################

%.att: %
	objdump -Cd $< > $@

%.cga: %.log
	cg_annotate --auto=yes $<

%.dat: %
#	sudo sh -c 'echo -1 > /proc/sys/kernel/perf_event_paranoid'
	sudo perf record --call-graph dwarf -e cycles -F max -o $@ -- ./$<

%.kcg: %.log2
	kcachegrind $<

%.log: %
	valgrind --cachegrind-out-file=$@ --tool=cachegrind ./$<

%.log2: %
	valgrind --branch-sim=yes --cacheuse=yes --callgrind-out-file=$@ --cache-sim=yes --collect-jumps=yes --demangle=yes --dump-instr=yes --instr-atstart=yes -q --separate-threads=yes --simulate-hwpref=yes --tool=callgrind --trace-jump=yes ./$<

%.out: %
	./$< | tee $@

%.perf: %.dat
	sudo perf report -g 'graph,0.5,caller' -i $<

###############################################################################

facil2: CXXFLAGS+=-fno-inline
fibonacci: CXXFLAGS+=-pthread
ijk-bench: LDLIBS+=-lbenchmark
#selfmod:  CXXFLAGS+=-fno-inline
#%-etal: CXXFLAGS+=-fopt-info

###############################################################################

.PHONY: all att clean exe sm tipos
.PRECIOUS: $(DAT) $(LOG) $(OUT)
.NOEXPORT:

###############################################################################

