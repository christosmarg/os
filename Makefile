TGTDIR = build

all:
	cd kern && make install clean && cd -

run:
	qemu-system-i386 -hdd ${TGTDIR}/os.bin

clean:
	rm -rf ${TGTDIR}
	cd kern && make clean && cd -
