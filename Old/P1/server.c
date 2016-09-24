/* -----------------------------------------
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./server "
-------------------------------------------*/

#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 2000

int main(int argc, char *argv[])
{
     int listenfd, connfd, addrlen, n;
     char recvBuff[256];
     struct sockaddr_in serv_addr, cli_addr;

     
     /*   SOCK_STREAM denotes TCP connection   */
     listenfd = socket(AF_INET, SOCK_STREAM, 0);
        printf("Socket retrieve success. \n");
     
     
    /*-----listenfd must == 0 for success-----*/
    if (listenfd < 0) 
        printf("Failed to listen. \n");
     
     /*------Initialize serv_addr to zero------*/
     bzero((char *) &serv_addr, sizeof(serv_addr));
    

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = (INADDR_ANY);
     serv_addr.sin_port = htons(PORT);
    
    /*   Error when server is busy   */
    if (bind(listenfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
              printf("Binding Error. \n");
              
     listen(listenfd,5);
     addrlen = sizeof(cli_addr);
     connfd = accept(listenfd, (struct sockaddr *) &cli_addr, &addrlen);
    
     /*----Set recvBuff to zero-----*/
     bzero(recvBuff,256);
     
     /*------Reads message from client, GET...-----*/
     n = read(connfd,recvBuff,255);
     
     /*------Prints GET... to server-----*/
     printf("%s\n",recvBuff);
     
    /*   Writes on Client Side   */
     n = write(connfd,"Message Received \n",18);
     
return 0; 
}