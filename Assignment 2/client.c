/********************************
 * Lab Assignment 2.
 * -----------------
 * File : client.c
 *
 * Description : FTP using TCP.
 *
 * @author : Jhansi Lakshmi Kolla, Rakshith Kunchum
 * @version : 1.0.0
 * @email : kolla.4@osu.edu, kunchum.1@osu.edu
 * @date : 20/01/2016
 */
 
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

#define BUFFER_SIZE 512
#define INPUT_FILENAME_SIZE 20
#define HEADER_FILENAME_SIZE INPUT_FILENAME_SIZE
#define HEADER_FILESIZE 4
#define HEADER_SIZE HEADER_FILENAME_SIZE+HEADER_FILESIZE
#define MAX_PORT_NUMBER 65535

int configure_client_socket (int port, char* ipaddr);

struct sockaddr_in socket_server;

struct header{
	uint32_t file_size;
	char file_name[HEADER_FILENAME_SIZE];
};

int main (int argc, char**argv){

	int socket_file_directory, input_file_directory;
    char buffer[BUFFER_SIZE];
    struct header file_header;
    off_t count=0, m;//long
	long int n;
    int l=sizeof(struct sockaddr_in);
    
	//check for number of arguments.
	if(argc < 4){
 		printf("Incorrect number of arguments. Please run the program in the following format.\n");
 		printf("\tftpc  <remote-IP>  <remote-port>  <local-file-to-tranfer>\n");
 		return -1;
	}
    
    //check ip address format.
    if(!(inet_pton(AF_INET,argv[1],&(socket_server.sin_addr)))){
		printf("Invalid IP Address.\n");
		return -1;
	}
	
	//check port number limit.
	if(atoi(argv[2]) > MAX_PORT_NUMBER){
		printf("Port number out of range.\n");
		return -1;
	}
    
    //check the size of the input filename.
    if(strlen(argv[3]) > INPUT_FILENAME_SIZE-1){
    	printf("Input filename should be less than 20 characters.\n");
    	printf("Otherwise the filename will be truncated and sent.\n");
		//return -1;
    }
    //configure the socket for tcp and bind it to the specified ip-address and port.
    socket_file_directory=configure_client_socket(atoi(argv[2]), argv[1]);
    
	if ((input_file_directory = open(argv[3],O_RDONLY))==-1){
		printf("Error while opening the file.");
		return -1;
	}
    
    if(connect(socket_file_directory,(struct sockaddr*)&socket_server,l)==-1){
        printf("Connection Refused.\n");
		return -1;
    }

    //determining file size.
    FILE* input_file = fopen(argv[3], "r");
	fseek(input_file, 0, SEEK_END); // seek to end of file 	
	int file_size = ftell(input_file); 
	fseek(input_file, 0, SEEK_SET); 
	fclose(input_file);


    strcpy(file_header.file_name,argv[3]);
    file_header.file_size = file_size;
    n = send(socket_file_directory, &file_header, sizeof(file_header),0);
    //TODO: Add code to verify sending of header information.

	n = read(input_file_directory,buffer,BUFFER_SIZE);
	if(n == 0){
		printf("Input file is empty.\n");
	}
	while(n){
		if(n==-1){
			printf("Error while reading the file.\n");
			return -1;
		}
		m = send(socket_file_directory,buffer,n,0);
		//TODO: Add code to verify sending packet information.
		if(m==-1){
			printf("Error while sending the file.\n");
			return -1;
		}
		count+=m;
		//fprintf(stdout,"----\n%s\n----\n",buffer);
        n = read(input_file_directory,buffer,BUFFER_SIZE);
	}

	//m=sendto(socket_file_directory,buffer,0,0,(struct sockaddr*)&socket_server,l);
	
	printf("Input file size : %d\n",count);
	
	close(input_file_directory);
    close(socket_file_directory);
	return EXIT_SUCCESS;
}

int configure_client_socket (int port, char* ip_address){
    int socket_fd;
    
	socket_fd = socket(PF_INET,SOCK_STREAM,0);
	if (socket_fd == -1){
        printf("Error while creating a socket.\n");
		return -1;
	}
    
	socket_server.sin_family=AF_INET;
	socket_server.sin_port=htons(port);
    return socket_fd;
}