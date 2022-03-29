###############################################################################

SHELL = /usr/bin/bash
.ONESHELL:

###############################################################################

CXXFLAGS = -march=native -O3 -pthread -Wall
LDLIBS = -lrt

###############################################################################

ATT = $(EXE:=.att)
SRC = $(sort $(wildcard *.cc))
EXE = $(basename $(SRC))
DAT = $(EXE:=.dat)
LOG = $(EXE:=.log)
VER = $(EXE:=.ver)

###############################################################################

all: clave ver
	@printf '%56s\n' | tr ' ' -

att: $(ATT)

clave: exe
	@printf '%56s\n' | tr ' ' -
	echo "clave: programa = tipo de paso de mensajes"
	echo "       mensajes = pings + pongs"
	echo "       tiempo   = tiempo de cpu total en ms"
	echo "       ratio    = mensajes / tiempo"
	printf '%56s\n' | tr ' ' -
	echo "            programa    mensajes      tiempo       ratio"
	printf '%56s\n' | tr ' ' -

clean:
	-rm -fv $(ATT) $(DAT) $(EXE) $(LOG) $(VER) {vg,}core.* perf.data *~

exe: $(EXE)

mensajes: clave
	@make -s ver | sort -h -k 2 -r
	printf '%56s\n' | tr ' ' -

ratio: clave
	@make -s ver | sort -h -k 4 -r
	printf '%56s\n' | tr ' ' -

tiempo: clave
	@make -s ver | sort -h -k 3
	printf '%56s\n' | tr ' ' -

ver: exe $(VER)

###############################################################################

%.att: %
	objdump -CdS $< > $@

%.dat %.log: %
	@LANG='en' sudo perf stat ./$< 1> $*.log 2> $*.dat

%.ver: %.dat %.log
	@min=$$(cat $*.log | cut -d' ' -f3 | sort | head -n1)
	max=$$(cat $*.log | cut -d' ' -f3 | sort | tail -n1)
	if ((min == max || min + 1 == max)); then
		lpad() { printf "%*s" "$$1" "$$2"; }
		lprint() { lpad "$$1" "$$2"; shift 2; (( $$# > 0 )) && lprint "$$@" || echo; }
		pings=`grep 'ping' $*.log | grep -o '[[:digit:]]*'`
		pongs=`grep 'pong' $*.log | grep -o '[[:digit:]]*'`
		(( msg = pings + pongs ))
		time=$$(grep 'task-clock' $*.dat | tr -s ' ' | cut -d ' ' -f 2)
		ratio=$$(bc -l <<< "scale=2; $$msg/$$time" 2> /dev/null)
		lprint 20 $* 12 $$msg 12 $$time 12 $$ratio
	else
		echo "fallo en $*.log: nº de pings != nº de pongs!!!"
	fi

###############################################################################

.PHONY: all att clave clean exe mensajes ratio tiempo ver
.PRECIOUS: $(LOG) $(DAT)
.NOEXPORT:

###############################################################################

