BIN = os.bin
BINDIR = ../build

CC = cc
ASM = nasm
LD = ld
INCDIR = ../include
CFLAGS = -g -m32 -nostdlib -ffreestanding -Wall -Wextra -std=c99 -O2 -I${INCDIR}
LDFLAGS = -Ttext 0x1000 --oformat binary

BOOTFILE = boot.s
BOOT_BIN = boot.bin
KERNEL_BIN = kernel.bin
SRC = *.c *.s
OBJ = kern_main.o \
      libk.o \
      idt.o \
      intr.o \
      vga.o \
      kbd.o \
      timer.o \
      page.o

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

.s.o:
	${ASM} -felf $<

install: all
	mv ${BIN} ${BINDIR}

clean:
	rm -rf *.bin *.o

.PHONY: all options install clean
