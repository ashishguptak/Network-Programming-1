//client code
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(void)
{
    int sockfd = 0;
    int bytesReceived = 0;
    char recvBuff[256];
    memset(recvBuff, '0', sizeof(recvBuff));
    struct sockaddr_in serv_addr;

    /* Create a socket first */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0))< 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    /* Initialize sockaddr_in data structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); // port
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* Attempt a connection */
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

    /* Create file where data will be stored */
    FILE *fp;
    fp = fopen("sample_file.txt", "ab"); 
    if(NULL == fp)
    {
        printf("Error opening file");
        return 1;
    }

    /* Receive data in chunks of 256 bytes */
    while((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
    {
        printf("Bytes received %d\n",bytesReceived);    
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1,bytesReceived,fp);
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }


    return 0;
}


//server code.
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr;
    char sendBuff[1025];
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listen(listenfd, 10) == -1)
    {
        printf("Failed to listen\n");
        return -1;
    }


    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);

        /* Open the file that we wish to transfer */
        FILE *fp = fopen("sample_file.txt","rb");
        if(fp==NULL)
        {
            printf("File opern error");
            return 1;   
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[256]={0};
            int nread = fread(buff,1,256,fp);
            printf("Bytes read %d \n", nread);        

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread);
            }

            /*
             * There is something tricky going on with read .. 
             * Either there was error, or we reached end of file.
             */
            if (nread < 256)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }


        }

        close(connfd);
        sleep(1);
    }


    return 0;
}


//help
::read()  returns the number of characters read.
          Just because you write in chunks of 256 does not mean they
          will arrive in chunks of.

::write() Same thing goes here.
          You may try and write in chunks of 256 does not mean
          that all 256 will be written.

//using write
std::size_t    bytesWritten = 0;
std::size_t    bytesToWrite = <Some Size>;

while (bytesWritten != bytesToWrite)
{
    std::size_t    writtenThisTime;

    do
    {
         writtenThisTime = ::write(fd, buf + bytesWritten, (bytesToWrite - bytesWritten));
    }
    while((writtenThisTime == -1) && (errno == EINTR));

    if (writtenThisTime == -1)
    {
        /* Real error. Do something appropriate. */
        return;
    }
    bytesWritten += writtenThisTime;
}

//using read
std::size_t    bytesRead = 0;
std::size_t    bytesToRead = <Some Size>;

while (bytesToRead != bytesRead)
{
    std::size_t    readThisTime;

    do
    {
         readThisTime = ::read(fd, buf + bytesRead, (bytesToRead - bytesRead));
    }
    while((readThisTime == -1) && (errno == EINTR));

    if (readThisTime == -1)
    {
        /* Real error. Do something appropriate. */
        return;
    }
    bytesRead += readThisTime;
}

//other unchecked calls.
fwrite(recvBuff, 1,bytesReceived,fp);
listenfd = socket(AF_INET, SOCK_STREAM, 0);
bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));
connfd = accept(listenfd, (struct sockaddr*)NULL ,NULL);
int nread = fread(buff,1,256,fp);             // You may not get all 256 items.
