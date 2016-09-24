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
     printf("Socket retrieve success.\n\n");
     
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
     
     struct tcpheader tcp;  
     unsigned short int cksum_arr[12]; 
     unsigned int i, sum=0, cksum;

     /*  First Read  */
     /*  Reads struct  */
     bzero(recvBuff,5000);
     n = read(connfd,recvBuff,4999); 
     memcpy(&tcp, recvBuff, 4999);  
     tcp.seqnum = 0;
     tcp.acknum = 1; //client seq + 1 = 0+1 
     tcp.syn = 1;
     tcp.ack = 1; 
   //  tcp.chksum = 0; //Do I set checksum to zero each time before calclulating 
     
     /*  copy 24 bytes for cksum calculation  */
     memcpy(cksum_arr, &tcp, 24); 
     /*  Compute checksum  */
     for(i=0; i<12; i++)
     sum = sum+cksum_arr[i]; 

    /*  Fold once  */
    cksum = sum >> 16; 
    sum  = sum & 0x0000FFFF;
    sum = cksum + sum; 

    /*  Fold once more  */
    cksum = sum >> 16; 
    sum  = sum & 0x0000FFFF;
    cksum = cksum + sum; 
    /*   Print Connection Granted  */
    printf("2. Connection Granted TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Checksum: 0x%04X\n", (0xFFF^cksum));
     
    /*  First Send  */
    /*  memcpy structure to buffer */
    bzero(recvBuff,5000);
    memcpy(recvBuff, &tcp, sizeof(tcp)); 
    send(connfd, recvBuff, sizeof(recvBuff), 0); 

    /* Clear struct  */
    bzero(cksum_arr, 12);
    /*   Connection Granted  */
    // n = write(connfd,"Message Received \n",18);

     //clear buff to receive header 
     bzero(recvBuff, 5000); //
    // n = read(connfd, recvBuff, 1999); //accepts tcp fields
     //printf("%s\n", recvBuff); // print tcp field request

return 0; 
}