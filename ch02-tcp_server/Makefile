SRC := src
CC := gcc
BUILD := build
CFLAGS := -g -pthread

simple_server:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/$@.c -o ${BUILD}/$@
	${BUILD}/$@

multiprocess_server:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/$@.c -o ${BUILD}/$@
	${BUILD}/$@

multithread_server:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/$@.c -o ${BUILD}/$@
	${BUILD}/$@

select_server:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/$@.c -o ${BUILD}/$@
	${BUILD}/$@

# listen on tcp/8892
epoll_server:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/tcp_server.c ${SRC}/srv_utilities.c -o ${BUILD}/$@
	${BUILD}/$@

bst:
	mkdir -p ${BUILD}
	${CC} ${CFLAGS} ${SRC}/$@.c ${SRC}/btree.c -o ${BUILD}/$@
	${BUILD}/$@

clean:
	rm -rf ${BUILD}
