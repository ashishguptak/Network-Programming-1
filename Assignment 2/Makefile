#Makefile

CC = g++
OFILES = client.o \
	server.o 
CFILECLT = client.c
CFILESRV = server.c
EXESRV = ftps
EXECLT = ftpc
CFLAGS=-Wall -O3

but : ${EXESRV} ${EXECLT}

${EXESRV} :
	${CC} ${CFLAGS} -o ${EXESRV} ${CFILESRV}

${EXECLT} :
	${CC} ${CFLAGS} -o ${EXECLT} ${CFILECLT}

clean :
	-rm -f ${OFILES} ${EXESRV} ${EXECLT} *~
                 
mrproper : clean but

.PHONY : but clean mrproper




