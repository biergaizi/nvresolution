CC = gcc
CFLAGS = -O3 -Wall -Wextra -std=c89 -g
DEFINES = -D_GNU_SOURCE

all:
	${CC} nvresolution.c -o nvresolution ${CFLAGS} ${DEFINES}
static:
	${CC} -static nvresolution.c -o nvresolution ${CFLAGS} ${DEFINES}
clean:
	rm -rf nvresolution
