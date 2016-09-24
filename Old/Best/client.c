
/*------------------------------------------------------------- 
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./client serverhostname "
--------------------------------------------------------------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#define PORT 2000


int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in servAddr;
    struct hostent *server;
    char buffer[256];
    char response[2000];

    /*------User must enter ./client cse01------*/
    if (argc < 1) {
       fprintf(stderr,"usage %s hostname\n", argv[0]);
       exit(0);
    }
  
    /*-----SOCK_STREAM denotes TCP connection-----*/  
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 1)
        printf("Error opening socket");

    /*-------gets ip of cse01 and stores in struct server---*/
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Error, no such host\n");
        exit(0);
    }

        bzero((char *) &servAddr, sizeof(servAddr));
        servAddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *)&servAddr.sin_addr.s_addr,server->h_length);
        servAddr.sin_port = htons(PORT);
    
    if (connect(sockfd,(struct sockaddr *)&servAddr,sizeof(servAddr)) < 0)
        printf("Error connecting");

    printf("Please enter URL: ");
    bzero(buffer,256);

    /*--Receives URL--*/
    fgets(buffer,255,stdin);

    /*--Writes URL to server--*/
    n = write(sockfd,buffer,strlen(buffer));//
    
    if (n < 0)//del
         printf("Error writing to socket");//del
    bzero(buffer,256);

    if (n < 0)//del
         printf("Error reading from socket");//del
    printf("%s\n",buffer);

    if( recv(sockfd, response , 2000 , 0) < 0)
    {
        printf("Receive failed.\n");//from puts
    }
    
    /*---Response from server----*/
    puts(response);


    return 0;
}
