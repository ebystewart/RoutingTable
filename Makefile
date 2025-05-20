CC=gcc
CFLAGS=-g
OBJS=routing.o	\
main.o


routing.bin:${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o routing.bin

routing.o:routing.c
	${CC} ${CFLAGS} -c routing.c -I . -o routing.o

main.o:main.c
	${CC} ${CFLAGS} -c main.c -I . -o main.o

all:
	make

clean:
	rm *.o
	rm *.bin