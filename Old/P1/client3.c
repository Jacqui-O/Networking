/*------------------------------------------------------------- 
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./client serverhostname portno URL"
--------------------------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

/*----argument count, argument vector-----------
  ----argc - n number of strings in command line
  ----argv - a vector of size n---------------*/
int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server, *website; 
    char buffer[256];

    /*------User must enter ./client cse01 portnumber URL------*/
    if (argc < 3) {
       printf("Must use the form: %s serverhostname portnumber URL\n", argv[0]);
       exit(0);
    }
    
    /*----Convert portno provided by user to an integer-----*/
    portno = atoi(argv[2]);
   
    /*-----SOCK_STREAM denotes TCP connection-----*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /*-----sockfd<0 unsuccessful, sockfd==0 successful------*/
    if (sockfd < 0) 
        error("ERROR opening socket. \n");

    /*-------cse01 is server host name, entered by user-----*/
    /*-------gets ip of cse01 and stores in struct server---*/
    server = gethostbyname(argv[1]);
   
    /*-----Print message if hostname is invalid-----*/
    if (server == NULL) {
        printf("No such host. \n");
        exit(0);
    }
    
    /*------Set serv_addr to zero-----*/
    bzero((char *) &serv_addr, sizeof(serv_addr));


    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
   
    
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        printf("Connection Failed. \n");
        return 1;
    }
    
    /*----Prints to client------*/
    printf("Connection Successful \n ");
    
    /*-----Gets IP address of URL and stores in website struct-----*/
    website=gethostbyname(argv[3]);
    
    
    /*----Messaged sent to print on server side, GET...-----*/
    sprintf(buffer,"GET %s HTTP/1.1\n ",argv[3]);
    send(sockfd, buffer, sizeof(buffer), 0);

    /*----Reset buffer to zero----*/
    bzero(buffer,256);
    
    /*-----Reads reply from server, Message Received------*/
    n = read(sockfd,buffer,255);

    /*----Prints reply from server, Message Received------*/
    printf("%s\n",buffer);
    
return 0;
}