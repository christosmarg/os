TGTDIR = build

all:
	cd boot && make install clean && cd ..

run:
	qemu-system-i386 -hdd ${TGTDIR}/os.bin

clean:
	rm -rf ${TGTDIR}
