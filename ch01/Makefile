SRC := src
CC := gcc
BUILD := build
CFLAGS := -g

fork:
	${CC} ${CFLAGS} ${SRC}/$@.c -o ${BUILD}/$@
	@echo "==================== Program Started ===================="
	${BUILD}/$@

clean:
	rm -f build/*
