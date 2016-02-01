/* fptc.c using UDP */

/* Client for connecting to Internet stream server waiting on port 1040 */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "socket_helper.h"
#define MAX_MES_LEN 1000

/*Strucutre to store file size and file name*/
struct file_information{
   int size;
   char filename[20];
}file_info;

/* client program called with host name and port number where server is run, file to be transfered is also given as an argument */
main(int argc, char *argv[]) {

    int sock;                     /* initial socket descriptor */
    int  rval;                    /* returned value from a read */
    struct sockaddr_in client_addr; /* structure for socket name
                                 * setup */
    char buf[MAX_MES_LEN] = "*****Hello in TCP from client*****";     /* message to set to server */
    struct hostent *hp;
    int bufferLen = MAX_MES_LEN;
    /*If incorrect arguements then exit the program.*/
    if(argc!=3) {
        printf("*****Enter Correct Arguements.*****");
        exit(1);
    }
    //convert port numebr to integer.
    int port = atoi(argv[2]);

    /* initialize socket connection in unix domain */
    if((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("*****error openting datagram socket*****");
        exit(1);
    }

    hp = gethostbyname(argv[1]);
    if(hp == 0) {
        fprintf(stderr, "%s: unknown host\n", argv[1]);
        exit(2);
    }

    /* construct name of socket to send to */
    bcopy((void *)hp->h_addr, (void *)&client_addr.sin_addr, hp->h_length);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port); /* fixed by adding htons() */

    //int send = SEND(sock, buf, MAX_MES_LEN, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    //printf("Client sends:    %s\n", buf);
    FILE *fp = fopen("File", "rb");
    char buffer[MAX_MES_LEN];
    int fs_block_sz = 0;
    //int success = 0;
    printf("\n*****Start reading*****");
    fflush(stdout);
    //Send the file 1000 bytes at a time to the server.
    while((fs_block_sz = fread(buffer, sizeof(char), MAX_MES_LEN, fp))>0) {
	printf("Sent bytes: %d\n", fs_block_sz);
        if(sendto(sock, buffer, fs_block_sz, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
            printf("*****ERROR: Failed to send file *****\n");
            exit(0);
        }
        bzero(buffer, bufferLen);
	usleep(10);
    }
    printf("\n*****File sent*****\n");
    fclose(fp);
    close(sock);
    exit(1);
}

