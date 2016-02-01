/* ftps.c using UDP */

/* Server for accepting an Internet stream connection on port 1040 */
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

/*Strucutre to store file size and file name*/
struct file_information{
   int size;
   char filename[20];
}file_info;

/* server program called with port number as argument */
main(int argc, char *argv[]) {
    if(argc!=2) {
        printf("*****Incorrect Arguments,Enter port number for Server*****\n");
        exit(0);
    }
    int sock;                     /* initial socket descriptor */
    int msgsock;                  /* accepted socket descriptor,
                                 * each client connection has a
                                 * unique socket descriptor*/
    int rval=1;                   /* returned value from a read */
    struct sockaddr_in sock_addr; /* structure for socket name setup */
    char buf[MAX_MES_LEN];               /* buffer for holding read data */
    char buf2[MAX_MES_LEN] = "*****Hello back in TCP from server*****";
    int port = atoi(argv[1]);       /*Post number changed to integer*/
    int bufferLen = MAX_MES_LEN;
    printf("*****Server started at Port Number: %d *****\n", port);
    printf("*****server waiting for remote connection from clients*****\n");

    /*initialize socket connection in unix domain*/
    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("*****error openting datagram socket*****");
        exit(1);
    }

    /* construct name of socket to send to */
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = INADDR_ANY;
    sock_addr.sin_port = htons(port);

    /* bind socket name to socket */
    if(bind(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        perror("*****error binding stream socket*****");
        exit(1);
    }

    int buflen = MAX_MES_LEN;

    //int rev = RECV(sock, buf, buflen, 0,(struct sockaddr *)&sock_addr, &sock_addr);    

    //printf("Server receives: %s\n", buf);
    FILE *fp = fopen("File_server", "wb");
    if(fp == NULL) {
        printf("******ERROR OPENING OUTPUT FILE******");
        printf("\n******Press Enter to Exit******");
        getchar();
        exit(0);
    }
    char readBuffer[MAX_MES_LEN];
    int fr_block_sz = 0;

    bzero(readBuffer, bufferLen);
    /*Read the file 1000 bytes at a time.*/
    while(fr_block_sz = RECV(sock, readBuffer, buflen, 0,(struct sockaddr *)&sock_addr, &sock_addr)) {

        if(fr_block_sz < 0) {
            error("*****Error receiving file from client to server.\n*****");
            exit(1);
        }
	printf("fr_size: %d, read = %s\n", fr_block_sz, readBuffer);
        int write_sz = fwrite(readBuffer, 1, fr_block_sz, fp);
	if(fr_block_sz<1000) {
		break;
	}
	printf("write_size: %d\n", write_sz);
        if(write_sz < fr_block_sz) {
            error("*****File write failed on server.*****\n");
            exit(1);
        }
        bzero(readBuffer, bufferLen);
		
    }
    
    printf("*****Received from client!*****\n");
    fclose(fp);
    close(sock);
}

