BIN= 		os.img
BINDIR= 	../build
PREFIX=		..	# base dir

LINKSCRIPT=	link.ld
BOOT_FILE= 	boot.s
BOOT_BIN= 	boot.bin
KERNEL_BIN= 	kernel.bin
OBJ= 		kern_main.o \
		libk.o \
		idt.o \
		pic.o \
		intr.o \
		vga.o \
		kbd.o \
		timer.o

include	../mk/kern.mk
