CC= 		cc
AS= 		nasm
LD= 		ld
ARCH= 		i386 # FIXME: make this automatic (?)
ARCHINCDIR+= 	${PREFIX}/${ARCH}
INCDIR+=	${PREFIX}/include
CFLAGS+= 	-g -m32 -Wall -Wextra -Werror -pedantic -std=c99 -O2 \
		-nostdlib -nodefaultlibs \
		-ffreestanding -fno-stack-protector -fno-builtin \
		-I${ARCHINCDIR} -I${INCDIR}
LDFLAGS+= 	-T${LINKSCRIPT} --oformat binary -melf_${ARCH} # FIXME: elf sig osdev

all: options ${BIN}

options:
	@echo ${BIN} build options
	@echo "ARCH	= ${ARCH}"
	@echo "CC	= ${CC}"
	@echo "AS	= ${AS}"
	@echo "LD	= ${LD}"
	@echo "CFLAGS	= ${CFLAGS}"
	@echo "LDFLAGS	= ${LDFLAGS}"

# TODO: boot, dd, kern, cat | start esp when 0x1000 | ld -T x2 | 2mb >>
${BIN}: ${OBJ}
	mkdir -p ${BINDIR}
	${AS} -fbin ${BOOT_FILE} -o ${BOOT_BIN}
	${LD} ${LDFLAGS} ${OBJ} -o ${KERNEL_BIN}
	dd if=/dev/zero bs=1000000 count=1 >> ${KERNEL_BIN}
	cat ${BOOT_BIN} ${KERNEL_BIN} > $@

.c.o:
	${CC} -c ${CFLAGS} $<

.s.o:
	${AS} -felf $<

install: all
	mv ${BIN} ${BINDIR}

clean:
	rm -rf *.bin *.img *.o

.PHONY: all options install clean
