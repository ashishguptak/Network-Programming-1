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
    if(argc!=2) {
        printf("*****Incorrect Arguments,Enter port number for Server*****\n");
        exit(0);
    }
    int tcpdc_sock, server_sock;                     /* initial socket descriptor */
                
    int rval=1;                   /* returned value from a read */
    struct sockaddr_in sock_addr, tcdpc_sock_addr; /* structure for socket name setup */
    char buf[MAX_MES_LEN];               /* buffer for holding read data */
    char buf2[MAX_MES_LEN] = "*****Hello back in TCP from server*****";
    int port = atoi(argv[1]);       /*Post number changed to integer*/
    int bufferLen = MAX_MES_LEN;
    printf("*****TCPD Server started at Port Number: %d *****\n", port);
    printf("*****TCPD server waiting for remote connection from clients*****\n");

    /*initialize socket connection in unix domain*/
    if((tcpdc_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("*****error openting datagram socket*****");
        exit(1);
    }
/*initialize socket connection in unix domain*/
    if((server_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("*****error openting datagram socket*****");
        exit(1);
    }

    /* construct name of socket to receive data from tcpdc. */
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sock_addr.sin_port = htons(6000);
	//to send data to ftps.c this will be a client
    tcdpc_sock_addr.sin_family = AF_INET;
    tcdpc_sock_addr.sin_addr.s_addr = INADDR_ANY;
    tcdpc_sock_addr.sin_port = htons(port);   //port address of tcpdc to receive the data.
printf("Before BInding\n");
fflush(stdout);
    /* bind socket name to socket tcpdc to get the data, thsi will be a serevr for tcpdc client */
    if(bind(tcpdc_sock, (struct sockaddr *)&tcdpc_sock_addr, sizeof(tcdpc_sock_addr)) < 0) {
        perror("*****error binding stream socket*****");
        exit(1);
    }
printf("After BInding\n");
fflush(stdout);
   
	char readBuffer[MAX_MES_LEN];	
    //int rev = RECV(sock, buf, buflen, 0,(struct sockaddr *)&sock_addr, &sock_addr);    

    //printf("Server receives: %s\n", buf);
printf("Before ReadFrom\n");
fflush(stdout);
int buflen = MAX_MES_LEN;
int rec = recvfrom(tcpdc_sock, readBuffer, buflen, 0,(struct sockaddr *)&tcdpc_sock_addr, &tcdpc_sock_addr);
printf("rec: %d", rec);
if(rec<0)				//Read the contents from Troll
		{
			error("ERROR reading packet from Troll");
			exit(1);
		}
printf("%s\n", readBuffer);
bzero(readBuffer, bufferLen);
    while(1){
		if(recvfrom(tcpdc_sock, (char*)&readBuffer, MAX_MES_LEN, 0,(struct sockaddr *)&tcdpc_sock_addr, &tcdpc_sock_addr)<0)				//Read the contents from Troll
		{
			error("ERROR reading packet from Troll");
			exit(1);
		}
		printf("Received at TCP-deamon Client: %s\n",readBuffer);
		
		if(sendto(server_sock, readBuffer, buflen, 0, (struct sockaddr *)&sock_addr, sizeof(sock_addr))<0) //Send contents received from troll on port 6000 to Server port 4000
		{
			error("Unable to send received packet from Troll to Server");
			exit(1);
		}
bzero(readBuffer, bufferLen);
	}
	
	close(tcpdc_sock);
	close(server_sock);
}
