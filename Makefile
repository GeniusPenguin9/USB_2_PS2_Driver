CC=gcc
CFLAGS=-Iinc -g 
LIBS=-lX11 -lXext -lXi -lXrandr -lXinerama -lpthread
BUILDDIR=build
TARGETNAME=usb2ps2

build: src/*.c inc/*.h
	mkdir -p ${BUILDDIR}
	$(CC) src/* -o ${BUILDDIR}/${TARGETNAME} ${CFLAGS} ${LIBS}

build-test:
	mkdir -p ${BUILDDIR}
	$(CC) src/* -o ${BUILDDIR}/${TARGETNAME} ${CFLAGS} 

clean:
	rm -f -r build
