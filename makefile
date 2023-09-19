all: BDHM

CC=gcc
OPT1=-Wall -std=c99 -O3 -g -flto
LIBS3= -lgsl -lgslcblas -lm

BDHM: BDHM.c
	${CC} ${OPT1} -c lists.c
	${CC} ${OPT1} -c BDHM.c
	${CC} ${OPT1} -c inputdata.c
	${CC} ${OPT1} -c init.c
	${CC} ${OPT1} -c distance.c
	${CC} ${OPT1} -c printdata.c
	${CC} ${OPT1} -c hi.c
	${CC} ${OPT1} -c bda.c
	${CC} ${OPT1} -c force.c
	${CC} ${OPT1} -c magnetics.c
	${CC} ${OPT1} -c verletlist.c
	${CC} ${OPT1} -c printdisplay.c
	${CC} ${OPT1} -c flow.c
	${CC} ${OPT1} -c partmanagement.c
	${CC} ${OPT1} -c calc.c
	${CC} ${OPT1} lists.o inputdata.o distance.o init.o printdata.o hi.o bda.o force.o magnetics.o verletlist.o printdisplay.o flow.o partmanagement.o calc.o BDHM.o -o BDHM3.out ${LIBS3}

clean:
	rm -f lists.o
	rm -f BDHM.o
	rm -f inputdata.o
	rm -f distance.o
	rm -f init.o
	rm -f printdata.o
	rm -f hi.o
	rm -f bda.o
	rm -f force.o
	rm -f magnetics.o
	rm -f verletlist.o
	rm -f printdata.o
	rm -f flow.o
	rm -f partmanagement.o
	rm -f calc.o
	rm -f BDHM3.out
