.ONESHELL:

ASM  = $(wildcard *.s)
ATT  = $(ASM:.s=.att)
BIN  = $(basename $(ASM))
OBJ  = $(ASM:.s=.o)

all: $(ATT) qemu

clean:
	-killall -q qemu-system-i386 || true
	-rm -fv $(ATT) $(BIN) $(OBJ) *~

debug: clean $(BIN) # https://github.com/cloudius-systems/osv/wiki/OSv-early-boot-(MBR)
	qemu-system-i386 -drive file=$(BIN),format=raw -s -S &> /dev/null &
	gdb -ex 'target remote localhost:1234' -ex 'set architecture i8086' -ex 'hbr *0x7c00' -ex 'c'

qemu: $(BIN)
	qemu-system-i386 -drive file=$(BIN),format=raw &> /dev/null &

$(BIN): $(OBJ)
	ld --oformat binary -Ttext 0x7c00 $< -o $@

%.att: %
	objdump -b binary -D -M i8086 -m i386 $< > $@


.PHONY: all clean debug qemu

