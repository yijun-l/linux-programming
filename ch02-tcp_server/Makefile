SRC := src
CC := gcc
BUILD := build
CFLAGS := -g

simple_server:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/$@.c -o ${BUILD}/$@
	${BUILD}/$@

clean:
	rm -rf ${BUILD}
