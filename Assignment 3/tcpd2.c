/*
 * Program fromtroll.c
 *
 * Testing program to test the "troll" (q.v.)
 * Listens for messages and attempts to echo them back to the sender via
 * the troll.  See also totroll.c.
 */
#ifndef lint
static char *rcsid = "$Header: /var/home/solomon/640/troll/RCS/fromtroll.c,v 3.2 1991/11/07 18:11:43 solomon Exp solomon $";
#endif

#include <sys/param.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

extern int errno;

typedef struct MyMessage {
	struct sockaddr_in msg_header;
	char contents[1000];
} MyMessage;

/* for lint */
void bzero(), bcopy(), exit(), perror();
#define Printf (void)printf
#define Fprintf (void)fprintf

main(argc,argv)
int argc;
char *argv[];
{

	int sock;	/* a socket for sending messages and receiving responses */
	MyMessage message;
	u_short port;
	struct sockaddr_in trolladdr, localaddr;
	int n;
	int lastseq = -1;
	//ftp
	int ftps_socket;
	struct sockaddr_in ftps_addr;
	//
	/* process arguments */
	if (argc != 2) {
		Fprintf(stderr, "usage: %s port\n", argv[0]);
		exit(1);
	}

	port = atoi(argv[1]);
	if (port < 1024 || port > 0xffff) {
		Fprintf(stderr, "%s: Bad troll port %d (must be between 1024 and %d)\n",
			argv[0], port, 0xffff);
		exit(1);
	}

	/* create a socket... */

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("fromtroll socket");
		exit(1);
	}
	//ftp
	if ((ftps_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("fromtroll socket");
		exit(1);
	}
	//

	/* ... and bind its local address */
	bzero((char *)&localaddr, sizeof localaddr);
	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = INADDR_ANY; /* let the kernel fill this in */
	localaddr.sin_port = htons(port);
	//ftp
	ftps_addr.sin_family = AF_INET;
	ftps_addr.sin_addr.s_addr = INADDR_ANY;
	ftps_addr.sin_port = htons(2001);
	//
	if (bind(sock, (struct sockaddr *)&localaddr, sizeof localaddr) < 0) {
		perror("client bind");
		exit(1);
	}

	/* Main loop */

	for(;;) {
		int len = sizeof trolladdr;

		/* read in one message from the troll */
		n = recvfrom(sock, (char *)&message, sizeof message, 0,
				(struct sockaddr *)&trolladdr, &len);
		if (n<0) {
			perror("fromtroll recvfrom");
			exit(1);
		}
		//n = message.contents - (lastseq+1);
		//Printf(">>> message content=%s\n", &message.contents);
		n = sendto(ftps_socket, &message.contents, sizeof (message.contents), 0, (struct sockaddr_in*)&ftps_addr, sizeof(struct sockaddr_in));
		printf("number of bytes sent : %d\n", n);
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


