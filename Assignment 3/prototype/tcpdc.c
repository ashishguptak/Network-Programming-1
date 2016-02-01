/*
 * Program totroll.c
 *
 * Testing program to test the "troll" (q.v.)
 * Sends messages via the troll to a another process and prints whatever
 * messages come back.  The other process is supposed to be fromtroll.c,
 * which just echos what it gets.
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>

#define TIME_DELAY 1
#define MAX_PORT_NUMBER 65535
#define FTPC_PORT 2000
#define MSG_SIZE 1000

//extern int errno;

typedef struct MyMessage {
	struct sockaddr_in msg_header;
	char contents[MSG_SIZE];
} MyMessage;

/* interval between message sends */
struct timeval timeout = {
	0L, /* seconds */
	0L, /* microseconds */
};

struct timeval timeout_original;

int qflag=0;  /* quiet */

/* for lint */
//void bzero(), bcopy(), exit(), perror();
//double atof();
#define Printf if (!qflag) (void)printf
#define Fprintf (void)fprintf

//main(argc,argv)
//int argc;
//char *argv[];
int main (int argc, char**argv){

	int troll_socket;	/* a socket for sending messages and receiving responses */
	MyMessage message;
	struct hostent *host;
	u_short port;
	struct sockaddr_in trolladdr, destaddr, localaddr, fromaddr;
	fd_set selectmask;
	int counter, n;
	//ftp
	struct sockaddr_in dummy_socket;
	int ftpc_socket;
	char ftp_message[MSG_SIZE];
	struct sockaddr_in ftp_addr;
	int byte_count_sent = 0;
	int byte_count_rec = 0;
	//

	/* process arguments */

	if(argc < 5){
 		printf("Incorrect number of arguments. Please run the program in the following format.\n");
 		printf("\ttcpdc <troll-IP> <troll-port> <dest-ip> <dest-port>\n");
 		return -1;
	}
	
	//check ip address format.
    if(!(inet_pton(AF_INET,argv[1],&(trolladdr.sin_addr)))){
		printf("Invalid troll IP Address.\n");
		return -1;
	}
	if(!(inet_pton(AF_INET,argv[3],&(destaddr.sin_addr)))){
		printf("Invalid destination IP Address.\n");
		return -1;
	}

	//check port number limit.
	if(atoi(argv[2]) > MAX_PORT_NUMBER){
		printf("troll Port number out of range.\n");
		return -1;
	}
	if(atoi(argv[4]) > MAX_PORT_NUMBER){
		printf("destination Port number out of range.\n");
		return -1;
	}

	host = gethostbyname(argv[1]);
	bzero ((char *)&trolladdr, sizeof trolladdr);
	trolladdr.sin_family = AF_INET;
	bcopy(host->h_addr, (char*)&trolladdr.sin_addr, host->h_length);
	trolladdr.sin_port = htons(atoi(argv[2]));

	/* destination ... */
	host = gethostbyname(argv[3]);
	bzero ((char *)&destaddr, sizeof destaddr);
	destaddr.sin_family = htons(AF_INET);
	bcopy(host->h_addr, (char*)&destaddr.sin_addr, host->h_length);
	destaddr.sin_port = htons(atoi(argv[4]));
	//ftp
	bzero ((char *)&ftp_addr, sizeof ftp_addr);
	ftp_addr.sin_family = htons(AF_INET);
	bcopy(host->h_addr, (char*)&ftp_addr.sin_addr, host->h_length);
	ftp_addr.sin_port = htons(FTPC_PORT);
	//
	/* create a socket... */

	if ((troll_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error in troll socket.\n");
		return -1;
	}
	//ftp
	if ((ftpc_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error in ftp client socket.\n");
		return -1;
	}
	//
	FD_ZERO(&selectmask);
	FD_SET(troll_socket, &selectmask);
	FD_SET(ftpc_socket, &selectmask);

	/* ... and bind its local address */
	bzero((char *)&localaddr, sizeof localaddr);
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = INADDR_ANY; /* let the kernel fill this in */
	localaddr.sin_port = 0;					/* let the kernel choose a port */
	if (bind(troll_socket, (struct sockaddr *)&localaddr, sizeof localaddr) < 0) {
		printf("error in binding troll socket.\n");
		return -1;
	}

	//ftp
	if (bind(ftpc_socket, (struct sockaddr *)&ftp_addr, sizeof ftp_addr) < 0) {
		printf("error in binding client socket.\n");
		return -1;
	}
	//
	/* Main loop */

	counter = 0;
	message.msg_header = destaddr; /* structure copy */
	errno = 0;

	timeout_original = timeout;
	for(;;) {
		socklen_t len = sizeof fromaddr;
		//ftp
		socklen_t ftp_len = sizeof ftp_addr;
		//
		fd_set rmask;
		rmask = selectmask;
		n = select(ftpc_socket+1, &rmask, 0, 0, &timeout);
		if (n < 0) {
			printf("error in select mask.\n");
			return -1;
		}
		
		// if (FD_ISSET(troll_socket, &rmask)) {
		// 	/* read in one message from the troll */
		// 	n = recvfrom(troll_socket, (char *)&message, sizeof message, MSG_WAITALL,
		// 			(struct sockaddr *)&fromaddr, &len);
		// 	if (n<0) {
		// 		perror("totroll recvfrom");
		// 		exit(1);
		// 	}
		// 	//Printf("<<< %d %s\n",message.contents,message.contents==counter-1 ? "" : "???");
		// 	continue;
		// }
		//ftp
		if (FD_ISSET(ftpc_socket, &rmask)) {
			/* read in one message from the troll */
			n = recvfrom(ftpc_socket, (char *)&message.contents, sizeof ftp_message, MSG_WAITALL,
					(struct sockaddr *)&ftp_addr, &ftp_len);
			byte_count_rec += n;
			printf("byte count received so far: %d\n",byte_count_rec);
			if (n<0) {
				printf("error in receiving from ftp client.\n");
				return -1;
			}

			n = sendto(troll_socket, (char *)&message, sizeof message, 0,
					(struct sockaddr *)&trolladdr, sizeof trolladdr);
			byte_count_sent += n;
			bzero((char *)&message.contents,sizeof (message.contents));
			if (n<0) {
				printf("error in sending to troll.\n");
				return -1;
			}
			printf("byte count sent so far: %d\n",byte_count_sent);
			continue;
		}
		//
		if (timeout.tv_sec == 0 &&  timeout.tv_usec==0){
			timeout = timeout_original;
			//printf("time out occurred.\n" );
		}
	}
} /* main */


