/*------------------------------------------------------------- 
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./client servname "
   Client is run on cse02
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
#define PORT 2000
#include "header.h"
#define PCKT_LEN 8192

/*   Checksum function   */
unsigned short csum(unsigned short *buf, int len)
{
    unsigned long sum;
    for(sum=0; len>0; len--)
        sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

int main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr, din;
    struct hostent *server, *website; 
    char buffer[256];
    char other[PCKT_LEN];
    
    /*   Sizeof Headers   */
    struct ipheader *ip = (struct ipheader *) other;
    struct tcpheader *tcp = (struct tcpheader *) (other + sizeof(struct ipheader));
    int one = 1;
    const int *val = &one;
    memset(other, 0, PCKT_LEN);

    /*   User must enter ./client cse01  */
    if (argc < 2) {
       printf("Must use the form: %s servname\n", argv[0]);
       exit(0);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket. \n");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        printf("No such host. \n");
        exit(0);
    }
 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(PORT);
   
    //din port
    //din addr
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        printf("Connection Failed. \n");
        return 1;
    }
    
    /*   Prints to client   */
    printf("Connection Successful \n ");
    
    // IP structure
    
    ip->iph_ihl = 5;
    ip->iph_ver = 4;
    ip->iph_tos = 16;//
    ip->iph_len = sizeof(struct ipheader) + sizeof(struct tcpheader); 
    ip->iph_ident = htons(54321);
    ip->iph_offset = 0;
    ip->iph_ttl = 64;
    ip->iph_protocol = 6; // TCP
    ip->iph_chksum = 0; // Done by kernel
    // Source IP, modify as needed, spoofed, we accept through
    // command line argument
    ip->iph_sourceip = inet_addr(argv[1]); //
    // Destination IP, modify as needed, but here we accept through
    // command line argument
  //  ip->iph_destip = inet_addr(argv[3]);
    
    // TCP structure
    // The source port, spoofed, we accept through the command line
   
 //   tcp->tcph_srcport = htons(atoi(argv[2]));
    // The destination port, we accept through command line
  //  tcp->tcph_destport = htons(atoi(argv[4]));
    tcp->tcph_seqnum = htonl(1);
    tcp->tcph_acknum = 0;
    tcp->tcph_offset = 5;
    tcp->tcph_syn = 1;
    tcp->tcph_ack = 0;
    tcp->tcph_win = htons(32767);//
    tcp->tcph_chksum = 0; // Done by kernel
    tcp->tcph_urgptr = 0;
    // IP checksum calculation
    ip->iph_chksum = csum((unsigned short *) other, (sizeof(struct ipheader) + sizeof(struct tcpheader)));
    //csum in ip->iph_chksum
    
    //   Print initial condition here  //
    //   Do calculation   //
    //   Send to Server   //
    
    /*   Message sent to print on server side.   */
    sprintf(buffer,"Seq #: %d \nAck: %d \nAckNum: %d \nSyn: %d \nUrg: %d \nTCPCheck: %d \nIPCheck: %d\n", tcp->tcph_seqnum, tcp->tcph_ack, tcp->tcph_acknum, tcp->tcph_syn, tcp->tcph_urgptr, tcp->tcph_chksum, ip->iph_chksum);
    send(sockfd, buffer, sizeof(buffer), 0);

    bzero(buffer,256);
    
    /*   Reads reply from server, Message Received   */
    n = read(sockfd,buffer,255);

    /*   Prints reply from server, Message Received   */
    printf("%s\n",buffer);
    
return 0;
}