BIN = os.bin
BINDIR = ../build

CC = cc
ASM = nasm
LD = ld
CFLAGS = -g -m32 -ffreestanding -Wall -Wextra -std=c99 -O2
LDFLAGS = -Ttext 0x1000 --oformat binary

BOOTFILE = boot.asm
BOOT_BIN = boot.bin
KERNEL_BIN = kernel.bin
SRC = *.c *.asm
OBJ = loader.o \
      kernel.o \
      string.o \
      tty.o \
      idt.o \
      int.o \
      kbd.o \
      timer.o

all: options ${BIN}

options:
	@echo ${BIN} build options
	@echo CC	= ${CC}
	@echo ASM	= ${ASM}
	@echo LD	= ${LD}
	@echo CFLAGS	= ${CFLAGS}
	@echo LDFLAGS	= ${LDFLAGS}

${BIN}: ${OBJ}
	mkdir -p ${BINDIR}
	${ASM} -fbin ${BOOTFILE} -o ${BOOT_BIN}
	${LD} ${LDFLAGS} ${OBJ} -o ${KERNEL_BIN}
	dd if=/dev/zero bs=1000000 count=1 >> ${KERNEL_BIN}
	cat ${BOOT_BIN} ${KERNEL_BIN} > $@

.c.o:
	${CC} -c ${CFLAGS} $<

.asm.o:
	${ASM} -felf $<

install: all
	mv ${BIN} ${BINDIR}

run: all
	qemu-system-i386 -hdd ${BINDIR}/${BIN}

clean:
	rm -rf *.bin *.o

.PHONY: options all install run clean
