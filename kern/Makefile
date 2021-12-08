BIN= 		os.bin
BINDIR= 	../build

CC= 		cc
ASM= 		nasm
LD= 		ld
ARCH= 		i386 # FIXME: make this automatic
ARCHINCDIR= 	../${ARCH}
INCDIR=		../include
LINKSCRIPT=	link.ld
CFLAGS= 	-g -m32 -Wall -Wextra -Werror -std=c99 -O2 \
		-nostdlib -nodefaultlibs \
		-ffreestanding -fno-stack-protector -fno-builtin \
		-I${ARCHINCDIR} -I${INCDIR}
LDFLAGS= 	-T${LINKSCRIPT} --oformat binary -melf_${ARCH}

BOOTFILE= 	boot.s
BOOT_BIN= 	boot.bin
KERNEL_BIN= 	kernel.bin
SRC= 		*.c *.s
OBJ= 		kern_main.o \
		libk.o \
		idt.o \
		intr.o \
		vga.o \
		kbd.o \
		timer.o \
		vm_page.o

all: options ${BIN}

options:
	@echo ${BIN} build options
	@echo ARCH	= ${ARCH}
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
