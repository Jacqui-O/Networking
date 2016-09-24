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
#include "header.h"
#define PORT 2000

int main(int argc, char *argv[])
{
     int listenfd, connfd, addrlen, n;
     char recvBuff[5000];
     struct sockaddr_in serv_addr, cli_addr;
     
     listenfd = socket(AF_INET, SOCK_STREAM, 0);
     printf("Socket retrieve success. \n");
     
     if (listenfd < 0)
        printf("Failed to listen. \n");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = (INADDR_ANY);
     serv_addr.sin_port = htons(PORT);
    
     /*   Error when server is busy   */
     if (bind(listenfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              printf("Binding Error. \n");
              
     listen(listenfd,5);
     
     addrlen = sizeof(cli_addr);
     //start while(1)
     connfd = accept(listenfd, (struct sockaddr *) &cli_addr, &addrlen);              //
     //-------------------------------------------------------------------------------//
     
     struct tcpheader tcp; // from *tcp  
     unsigned short int cksum_arr[12]; 
     unsigned int i, sum=0, cksum;

     /*   Reads struct   */
     bzero(recvBuff,5000);
     //n = read(connfd, cksum_arr, 23); // if receiving as int
     //or maybe, send as buffer, receive as buffer, then copy into cksum arr to calculate, then copy back to buffer
     n = read(connfd,recvBuff,4999); //what size? maybe 24
     memcpy(&tcp, recvBuff, 4999);
     tcp.syn = 0;//
     printf("Syn: 0x%04X\n", tcp.syn);
     //memcpy(cksum_arr, &tcp, 24);//copy 24 bytes 
 
     
    /*   Connection Granted  */
 //     printf("Connection Granted\nServer Sequence Num: %d \nAckNum: %d \nSynbit: %d \nAckBit: %d \nChecksum: %d \n", tcp->tcph_seqnum, tcp->tcph_acknum, tcp->tcph_syn, tcp->tcph_ack, tcp->tcph_chksum);
     //n = write recvBuff
     n = write(connfd,"Message Received \n",18);

     //clear buff to receive header // Write back to client //
     bzero(recvBuff, 2000); //
    // n = read(connfd, recvBuff, 1999); //accepts tcp fields
     //printf("%s\n", recvBuff); // print tcp field request

return 0; 
}