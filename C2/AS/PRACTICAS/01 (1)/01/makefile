##########################################################################
# makefile
##########################################################################

A32 = $(wildcard *32*.s)
E32 = $(basename $(A32))

A64 = $(wildcard *64*.s)
E64 = $(basename $(A64))

ASM = $(wildcard *.s)
SRC = $(wildcard *.c *.cc)
EXE = $(basename $(ASM) $(SRC))
ATT = $(EXE:=.att)

##########################################################################

CFLAGS = -g -no-pie -Wall
CXXFLAGS = $(CFLAGS)

##########################################################################

all: att

att: $(ATT)

clean:
	-rm -fv $(ATT) $(EXE) core.* *~

exe: $(EXE)

.PHONY: all att clean exe

##########################################################################

$(E32): CFLAGS+=-m32
$(E64): CFLAGS+=-m64

addable: CXXFLAGS+=-fconcepts
sum0: CXXFLAGS+=-O0
sum1: CXXFLAGS+=-O1
sum2: CXXFLAGS+=-O2
sum3: CXXFLAGS+=-O3
sums: CXXFLAGS+=-Os
sumg: CXXFLAGS+=-Og
variadic: CXXFLAGS+=-O3
zmm: CXXFLAGS+=-march=knl -O3

##########################################################################

%: %.s
	$(CC) $(CFLAGS) -nostartfiles $< -o $@

%: %.c
	$(CC) $(CFLAGS) $< -o $@

%: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

%.att: %
	objdump -C -d $< > $@

##########################################################################
