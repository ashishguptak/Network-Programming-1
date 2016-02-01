#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "socket_helper.h"
#define MAX_MES_LEN 1000

main(int argc, char *argv[]) {
    
    int sock, tcpds_sock;                     /* initial socket descriptor */
                
    int rval=1;                   /* returned value from a read */
    struct sockaddr_in sock_addr, tcpds_sock_addr; /* structure for socket name setup */
    char buf[MAX_MES_LEN];               /* buffer for holding read data */
    char buf2[MAX_MES_LEN] = "*****Hello back in TCP from server*****";

    int bufferLen = MAX_MES_LEN;
    
    printf("*****TCPD server waiting for remote connection from clients*****\n");

    /*initialize socket connection in unix domain*/
    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("*****error openting datagram socket*****");
        exit(1);
    }

    /*initialize socket connection in unix domain*/
    if((tcpds_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("*****error openting datagram socket*****");
        exit(1);
    }

    /* construct name of socket to send to */
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(atoi(argv[1]));  // this port number will be input for client. it will be a server for ftpc so bind it.

    tcpds_sock_addr.sin_family = AF_INET;
    tcpds_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcpds_sock_addr.sin_port = htons(7000);//Convert the port number to network byte order from host byte order
	printf("Before BInding\n");
fflush(stdout);
    /* bind socket name to socket */
    if(bind(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        perror("*****error binding stream socket*****");
        exit(1);
    }

    int buflen = MAX_MES_LEN;
	char readBuffer[MAX_MES_LEN] = "Hello From tcpdccccccc";	
    //int rev = RECV(sock, buf, buflen, 0,(struct sockaddr *)&sock_addr, &sock_addr);    
	
   if(sendto(tcpds_sock, readBuffer, buflen, 0, (struct sockaddr *)&tcpds_sock_addr, sizeof(tcpds_sock_addr))<0) //Send contents received from troll on port 6000 to Server port 4000
		{
			error("Unable to send received packet from Troll to Server");
			exit(1);
		}
printf("Sent by tcpc: %s\n", readBuffer);
bzero(readBuffer, bufferLen);
    while(1){
		if(recvfrom(sock, readBuffer, buflen, 0,(struct sockaddr *)&sock_addr, &sock_addr)<0)				//Read the contents from Troll
		{
			error("ERROR reading packet from Troll");
			exit(1);
		}
		printf("%s\n", readBuffer);
		
		if(sendto(tcpds_sock, readBuffer, sizeof readBuffer, 0, (struct sockaddr *)&tcpds_sock_addr, sizeof(tcpds_sock_addr))<0) //Send contents received from troll on port 6000 to Server port 4000
		{
			error("Unable to send received packet from Troll to Server");
			exit(1);
		}
bzero(readBuffer, bufferLen);
	}
	
	close(sock);
	close(tcpds_sock);
}
