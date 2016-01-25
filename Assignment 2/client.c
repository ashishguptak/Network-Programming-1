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
 * @date : 24/01/2016
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
#define HEADER_FILENAME_SIZE 20
#define HEADER_FILESIZE_SIZE 4
int configure_client_socket (int port, char* ipaddr);

struct sockaddr_in socket_server, free_socket;

struct header{
	char filename[HEADER_FILENAME_SIZE];
    uint32_t filesize;
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
    
    if(!(inet_pton(AF_INET,argv[1],&(free_socket.sin_addr)))){
		printf("Invalid IP Address.\n");
		return -1;
	}
	
	if(atoi(argv[2]) > 65535){
		printf("Port number out of range.\n");
		return -1;
	}
    
    socket_file_directory=configure_client_socket(atoi(argv[2]), argv[1]);
    
	if ((input_file_directory = open(argv[3],O_RDONLY))==-1){
		perror("Error while opening the file.");
		return EXIT_FAILURE;
	}
    
    if(connect(socket_file_directory,(struct sockaddr*)&socket_server,l)==-1){
        printf("Connection Refused.\n");
		return -1;
    }
    //determining file size.
    FILE* input_file = fopen(argv[3], "r");
	fseek(input_file, 0, SEEK_END); // seek to end of file 	
	int file_size = ftell(input_file); 
	fseek(input_file, 0, SEEK_SET); // seek back to beginning of file
	fclose(input_file);
    strcpy(file_header.filename,argv[3]);
    file_header.filesize = file_size;
    write(socket_file_directory, &file_header, sizeof(file_header));
	n = read(input_file_directory,buffer,BUFFER_SIZE);
	if(n == 0){
		printf("Input file is empty.\n");
	}
	while(n){
		if(n==-1){
			printf("Error while reading the file.\n");
			return -1;
		}
		m = sendto(socket_file_directory,buffer,n,0,(struct sockaddr*)&socket_server,l);
		if(m==-1){
			printf("Error while sending the file.\n");
			return -1;
		}
		count+=m;
		//fprintf(stdout,"----\n%s\n----\n",buffer);
        n = read(input_file_directory,buffer,BUFFER_SIZE);
	}

	//m=sendto(socket_file_directory,buffer,0,0,(struct sockaddr*)&socket_server,l);
	
	printf("File Size : %d\n",count);
	
    close(socket_file_directory);
	return EXIT_SUCCESS;
}

int configure_client_socket (int port, char* ipaddr){
    int l;
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