TGTDIR=		build

all:
	cd kern && make install clean && cd -

run:
	qemu-system-i386 -hda ${TGTDIR}/os.bin -serial stdio

clean:
	rm -rf ${TGTDIR}
	cd kern && make clean && cd -
