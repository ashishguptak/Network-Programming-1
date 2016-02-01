#Makefile

CC = g++
OFILES = client.o \
	server.o 
CFILECLT = ftpc.c
CFILESRV = ftps.c
CFILETCP = to.c

EXESRV = ftps
EXECLT = ftpc
EXETCP = tcpd
CFLAGS = -Wall -O3

default : ${EXESRV} ${EXECLT} ${EXETCP}

${EXESRV} :
	${CC} ${CFLAGS} -o ${EXESRV} ${CFILESRV}

${EXECLT} :
	${CC} ${CFLAGS} -o ${EXECLT} ${CFILECLT}

${EXETCP} :
	${CC} ${CFLAGS} -o ${EXETCP} ${CFILETCP}

clean :
	-rm -f ${OFILES} ${EXESRV} ${EXECLT} ${EXETCP} *~
                 
full : clean default

.PHONY : default clean full
