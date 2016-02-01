#Makefile

CC = g++
OFILES = client.o \
	server.o 
OUTFILES = ./*_out
CFILECLT = ftpc.c
CFILESRV = ftps.c
CFILETCPC = tcpdc.c
CFILETCPS = tcpds.c

EXESRV = ftps
EXECLT = ftpc
EXETCPC = tcpdc
EXETCPS = tcpds

CFLAGS = -Wall -O3 -w
LIB = -lm -lc
RCSDIFFFLAGS = -c

SRC = Makefile ftpc.c ftps.c tcpdc.c tcpds.c troll.h

default: ${EXESRV} ${EXECLT} ${EXETCPC} ${EXETCPS}

troll: troll.o
	${CC} ${CFLAGS} -o troll troll.o ${LIB}

${EXESRV}:
	${CC} ${CFLAGS} -o ${EXESRV} ${CFILESRV}

${EXECLT}:
	${CC} ${CFLAGS} -o ${EXECLT} ${CFILECLT}

${EXETCPC}:
	${CC} ${CFLAGS} -o ${EXETCPC} ${CFILETCPC} ${LIB}

${EXETCPS}:
	${CC} ${CFLAGS} -o ${EXETCPS} ${CFILETCPS} ${LIB}

ftp: ${EXESRV} ${EXECLT}

tcpd: ${EXETCPC} ${EXETCPS}

clean:
	-rm -f ${OFILES} ${OUTFILES} ${EXESRV} ${EXECLT} ${EXETCPC} ${EXETCPS} *~
                 
full: clean default

.PHONY: default clean full 