/********************************
 * Lab Assignment 2.
 * -----------------
 * File : server.c
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
#define HEADER_FILENAME_SIZE 20
#define HEADER_FILESIZE_SIZE 4
#define HEADER_SIZE HEADER_FILENAME_SIZE+HEADER_FILESIZE_SIZE
#define OUTPUT_FILENAME_SIZE 32
#define BACKLOG 1

int configure_server_socket (int port);

struct sockaddr_in socket_server,socket_client;

struct header{
    uint32_t file_size;
    char file_name[HEADER_FILENAME_SIZE];
};

int main(int argc,char** argv){
    int socket_file_directory,output_file_directory;
    struct header file_header;
    unsigned int length=sizeof(struct sockaddr_in);
    long int n, m,count=0;
    unsigned int file_directory;
    ushort client_port_number;
    char buffer[BUFFER_SIZE],output_file_name[OUTPUT_FILENAME_SIZE];
    char dst[INET_ADDRSTRLEN];
    
    if(argc < 2){
        printf("Incorrect number of arguments. Please run the program in the following format.\n");
        printf("\tftpc  <local-port>\n");
        return -1;
    }
    
    socket_file_directory = configure_server_socket(atoi(argv[1]));
    
    memset(buffer,'\0',BUFFER_SIZE);
    listen(socket_file_directory,BACKLOG);
    
    file_directory=accept(socket_file_directory,(struct sockaddr*)&socket_client,&length);
    if(file_directory==-1){
        printf("Error while accepting the connection.\n");
        return -1;
    }
    else {
        if(inet_ntop(AF_INET,&socket_client.sin_addr,dst,INET_ADDRSTRLEN)==NULL){
            printf("Error with remote socket address.\n");
            return -1;
        }
        client_port_number=ntohs(socket_client.sin_port);

        n=recv(file_directory,&file_header,HEADER_SIZE,0);
        printf("Input file name is %s\n",file_header.file_name);
        printf("Input file size is %d\n",file_header.file_size);
        memset(output_file_name,'\0',OUTPUT_FILENAME_SIZE);
        strcpy(output_file_name,file_header.file_name);
        strcat(output_file_name,"_out");
        printf("Creating the copied output file : %s\n",output_file_name);
        
        if ((output_file_directory=open(output_file_name,O_CREAT|O_WRONLY,0600))==-1){
            printf("Error while creating output file.\n");
            return -1;
        }
        memset(buffer,'\0',BUFFER_SIZE);
        n=recv(file_directory,buffer,BUFFER_SIZE,0);
        //TODO: verify the number of bytes read.
        while(n) {
            if(n==-1){
                printf("Error while receiving data.\n");
                return -1;
            }
            if((m=write(output_file_directory,buffer,n))==-1){
                printf("Error while writing into output file.\n");
                return -1;
            }
            count=count+m;
            memset(buffer,'\0',BUFFER_SIZE);
            n=recv(file_directory,buffer,BUFFER_SIZE,0);
            //TODO: verify the number of bytes read.
        }
        close(socket_file_directory);
        close(output_file_directory);
    }
    close(file_directory);
    
    //printf("Address of the input machine %s.%d\n",dst,client_port_number);
    printf("Output File Size : %ld \n",count);
    
    return 0;
}


int configure_server_socket (int port){
    int l;
	int socket_fd;
    int yes=1;
    
	socket_fd = socket(PF_INET,SOCK_STREAM,0);
	if (socket_fd == -1){
        printf("Error while creating socket.\n");
        return -1;
	}

    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int)) == -1 ) {
        printf("Error while setting up socket option.\n");
        return -1;
    }
    
    l=sizeof(struct sockaddr_in);
	bzero(&socket_server,l);
	
	socket_server.sin_family=AF_INET;
	socket_server.sin_port=htons(port);
	socket_server.sin_addr.s_addr=htonl(INADDR_ANY);
    
	if(bind(socket_fd,(struct sockaddr*)&socket_server,l)==-1){
		printf("Error while binding the socket.\n");
        return -1;
	}
    
    return socket_fd;
}