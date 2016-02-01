/*
 * Program fromtroll.c
 *
 * Testing program to test the "troll" (q.v.)
 * Listens for messages and attempts to echo them back to the sender via
 * the troll.  See also totroll.c.
 */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

extern int errno;

typedef struct MyMessage {
	struct sockaddr_in msg_header;
	char contents[1000];
} MyMessage;

/* for lint */
//void bzero(), bcopy(), exit(), perror();
#define Printf (void)printf
#define Fprintf (void)fprintf
#define FTPS_PORT 2001
#define MAX_PORT_NUMBER 65535

int main (int argc, char**argv){
	int troll_socket;	/* a socket for sending messages and receiving responses */
	MyMessage message;
	u_short port;
	struct sockaddr_in trolladdr, localaddr;
	int n;
	int lastseq = -1;
	//ftp
	int ftps_socket;
	struct sockaddr_in ftps_addr;
	int byte_count_received = 0;
	int byte_count_sent = 0;
	//
	/* process arguments */
	if(argc != 2){
 		printf("Incorrect number of arguments. Please run the program in the following format.\n");
 		printf("\ttcpds <tcpd-server-port>\n");
 		return -1;
	}

	//check port number limit.
	if(atoi(argv[1]) > MAX_PORT_NUMBER){
		printf("troll Port number out of range.\n");
		return -1;
	}

	/* create a socket... */

	if ((troll_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error in troll socket.\n");
		return -1;
	}
	//ftp
	if ((ftps_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("error in ftps socket.\n");
		return -1;
	}
	//

	/* ... and bind its local address */
	bzero((char *)&localaddr, sizeof localaddr);
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = INADDR_ANY; /* let the kernel fill this in */
	localaddr.sin_port = htons(atoi(argv[1]));
	//ftp
	ftps_addr.sin_family = AF_INET;
	ftps_addr.sin_addr.s_addr = INADDR_ANY;
	ftps_addr.sin_port = htons(FTPS_PORT);
	//
	if (bind(troll_socket, (struct sockaddr *)&localaddr, sizeof localaddr) < 0) {
		printf("error in binding to server socket.\n");
		return -1;
	}

	/* Main loop */

	for(;;) {
		socklen_t len = sizeof trolladdr;

		/* read in one message from the troll */
		n = recvfrom(troll_socket, (char *)&message, sizeof message, 0,
				(struct sockaddr *)&trolladdr, &len);
		if (n<0) {
			printf("error in receiving from troll.\n");
			return -1;
		}
		byte_count_received += n;
		printf("number of bytes received so far: %d\n", byte_count_received);
		//n = message.contents - (lastseq+1);
		//Printf(">>> message content=%s\n", &message.contents);
		n = sendto(ftps_socket, &message.contents, sizeof (message.contents), 0, 
				(struct sockaddr *)&ftps_addr, sizeof(struct sockaddr_in));
		if (n<0) {
			printf("error in sending to ftps server.\n");
			return -1;
		}
		byte_count_sent += n;
		printf("number of bytes sent so far: %d\n", byte_count_sent);
		bzero((char *)&message.contents,sizeof (message.contents));
		/*
		if (n == 0)
			Printf(">>> %d\n", message.contents);
		else if (n > 0)
			Printf(">>> %d (%d missing)\n", message.contents, n);
		else
			Printf(">>> %d (duplicate)\n", message.contents);
		lastseq = message.contents;
		*/
		errno = 0;
		//n = sendto(sock, (char *)&message, sizeof message, 0, (struct sockaddr *)&trolladdr, len);
		/*
		if (n!=sizeof message) {
			perror("fromtroll sendto");
			exit(1);
		}*/
	}
} /* main */


