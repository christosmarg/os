BINDIR=		build

all:
	cd kern && make install clean && cd -

run:
	qemu-system-i386 -hda ${BINDIR}/os.img -d cpu -monitor stdio

clean:
	rm -rf ${BINDIR}
	cd kern && make clean && cd -
