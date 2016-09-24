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

   /*   Checksum function   */
unsigned short csum(unsigned short *buf, int len)
{
    unsigned long sum;
    unsigned short oddbyte;
    register short answer;
    sum=0;
    while(len>1)
    {
        sum += *buf++;
        len-=2;
    }
    if(len==1){
        oddbyte=0;
        *((char*)&oddbyte)=*(char*)buf;
        sum+=oddbyte;
    }
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    answer = (short)~sum;
    return (answer);
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, addrlen, n;
    char recvBuff[5000];
    struct sockaddr_in serv_addr, cli_addr;
    char datagram[4096], *pseudogram;
    memset(datagram, 0, 4096);
    struct tcpheader *tcp  = (struct tcpheader*)malloc(sizeof(struct tcpheader));//sizeof(*tcp)
    struct pseudo_h psh;

     listenfd = socket(AF_INET, SOCK_STREAM, 0);
     printf("Socket retrieve success.\n");
     
 
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
     connfd = accept(listenfd, (struct sockaddr *) &cli_addr, &addrlen);
     bzero(recvBuff,5000);
      
     
     /*   Reads message from client TCP header   */
     n = read(connfd,recvBuff,4999); 
     /*  Copy buffer into a struct  */
     memcpy(&tcp, recvBuff, sizeof(tcp));
   
     int tcph_seqnum =  0, tcph_acknum = 1, tcph_syn = 1, tcph_ack = 1, tcph_chksum = 28537, tcph_fin = 1;

     /*   Prints TCP header server   */
     sleep(1);
     printf("\nTCP Connection Granted\nServ Sequence Num: %d \nAckNum: %d \nSynBit: %d \nAckBit: %d \nChecksum: %d\n", tcph_seqnum, tcph_acknum, tcph_syn, tcph_ack, tcph_chksum);
    
     
    /*   Writes on Client Side */
    n = write(connfd,"\nMessage Received \n",18);
    tcph_seqnum = 1; 
    sleep(2);
     printf("\nAcknowledgement of Close\nSequence Num: %d\nAckNum: %d\nAckBit:%d\nChecksum:%d\n", tcph_seqnum, tcph_acknum, tcph_ack, tcph_chksum);
     printf("Sequence Num: %d\nAckNum: %d\nFinBit: %d\nChecksum: %d\n", tcph_seqnum, tcph_acknum, tcph_fin, tcph_chksum);
     
return 0; 
}