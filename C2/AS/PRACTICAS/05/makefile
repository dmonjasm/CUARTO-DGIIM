###############################################################################
# makefile
###############################################################################

SHELL = /bin/bash
.ONESHELL:

ATT = $(EXE:=.att)
DAT = $(EXE:=.dat)
EXE = $(basename $(SRC))
LOG = sort.log
PNG = $(SVG:.svg=.png)
SRC = $(wildcard *.c *.cc)
SVG = sort.svg sort.l.svg

CFLAGS = -march=native -O3 -pthread -Wall 
CXXFLAGS = $(CFLAGS) -std=c++17

###############################################################################

all: att $(PNG)

att: $(ATT)

bug: fib-vector2
	@echo "for unsigned long long..."
	for ((i = 80; i < 100; ++i)); do ./$< $$i; done

clean:
	-rm -rf $(ATT) $(DAT) $(EXE) $(LOG) $(PNG) $(SVG) {vg,}core.* perf.* *.old *~

exe: $(EXE)

fastest: fib-array fib-array2 fib-hash fib-hash2 fib-iter fib-map # $(sort $(filter fib-%,$(EXE)))
	@for i in $^; do
		echo -en "$$i:\t"
		sudo perf stat -r 33 -- ./$$i 99999 |& grep elapsed | tr -s ' '
	done

tabla: medir-usuario medir-nucleo medir-proceso sw-f sw-t sw-p
	@echo -e "\t\tusuario\tnucleo\tproceso"
	echo -en "null fork\t"
	for i in medir-usuario medir-nucleo medir-proceso; do
		./$$i | grep minimo | sed 's/minimo : \([[:digit:].,]*\)µs/\1/'
	done | tr '\n' '\t'
	echo -en "\nsignal/wait\t"
	for i in sw-f sw-t sw-p; do
		 perf stat -r 256 -- ./$$i 2> /dev/null | sort -h | head -1 | sed 's/.* ≈ \([[:digit:].,]*\)µs/\1/'
	done | tr '\n' '\t'
	echo

###############################################################################

bench-fib: LDLIBS+=-lbenchmark

bench-fib fiber fib-u id medir medir-usuario sw-f: LDLIBS+=-lboost_fiber -lboost_context -lboost_system

medir-gb: LDLIBS+=-lbenchmark -lboost_fiber -lboost_context -lboost_system

medir-proceso: | nulo

sort: CFLAGS+=-fopenmp

%.att: %
	objdump -CdS $< > $@

%.dat: %
	perf record -o $@ -- ./$< # use "-F max" to improve precision

%.log: %
	@./$< | tee $@

%.perf: %.dat
	perf report -i $<

%.png: %.svg
	convert -density 400 $< $@

%.svg: %.log makefile
	@gnuplot <<< "set terminal svg enhanced; \
	              set output '$@'; \
	              set key autotitle columnheader left top noenhanced; \
	              set grid; \
	              set xlabel 'size'; \
	              set logscale x 2; \
	              set ylabel 'time'; \
	              set xtics rotate; \
	              set xtics (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, '1k' 1024, '2k' 2048, '4k' 4096, '8k' 8192, '16k' 16384, '32k' 32768, '64k' 65526, '128k' 131072, '256k' 262144, '512k' 524288, '1M' 1048576, '2M' 2097152, '4M' 4194304, '8M' 8388608, '16M' 16777216, '32M' 33554432, '64M' 67108864, '128M' 134217728, '256M' 268435456, '512M' 536870912, '1G' 1073741824); \
	              set ytics ('5ms' 5e6, '10ms' 10e6, '15ms' 15e6, '20ms' 20e6, '25ms' 25e6, '30ms' 30e6, '35ms' 35e6, '40ms' 40e6, '45ms' 45e6, '50ms' 50e6, '55ms' 55e6, '60ms' 60e6, '65ms' 65e6, '70ms' 70e6, '75ms' 75e6, '80ms' 80e6, '85ms' 85e6, '90ms' 90e6, '95ms' 95e6, '100ms' 100e6, '105ms' 105e6, '110ms' 110e6, '115ms' 115e6); \
	              plot '$<' u 1:2 w lp lc -1 ps 0.66 pt 5, \
	                   '$<' u 1:3 w lp lc 1 ps 0.66 pt 7, \
	                   '$<' u 1:4 w lp lc 2 ps 0.66 pt 9, \
	                   '$<' u 1:5 w lp lc 3 ps 0.66 pt 13"

%.l.svg: %.log makefile
	@gnuplot <<< "set terminal svg enhanced; \
	              set output '$@'; \
	              set key autotitle columnheader bottom noenhanced; \
	              set grid; \
	              set xlabel 'size'; \
	              set ylabel 'time'; \
	              set logscale x 2; \
	              set logscale y 10; \
	              set xtics rotate; \
	              set xtics (1, 2, 4, 8, 16, 32, 64, 128, 256, 512, '1k' 1024, '2k' 2048, '4k' 4096, '8k' 8192, '16k' 16384, '32k' 32768, '64k' 65526, '128k' 131072, '256k' 262144, '512k' 524288, '1M' 1048576, '2M' 2097152, '4M' 4194304, '8M' 8388608, '16M' 16777216, '32M' 33554432, '64M' 67108864, '128M' 134217728, '256M' 268435456, '512M' 536870912, '1G' 1073741824); \
	              set ytics ('10ns' 1e1, '100ns' 1e2, '1us' 1e3, '10us' 1e4, '100us' 1e5, '1ms' 1e6, '10ms' 1e7, '100ms' 1e8, '1s' 1e9, '10s' 1e10, '100s' 1e11); \
	              plot '$<' u 1:2 w lp lc -1 ps 0.66 pt 5, \
	                   '$<' u 1:3 w lp lc 1 ps 0.66 pt 7, \
	                   '$<' u 1:4 w lp lc 2 ps 0.66 pt 9, \
	                   '$<' u 1:5 w lp lc 3 ps 0.66 pt 13"

###############################################################################

.PHONY: all att bug clean exe fastest tabla
.PRECIOUS: $(DAT) $(LOG) $(SVG)
.NOEXPORT:

###############################################################################

