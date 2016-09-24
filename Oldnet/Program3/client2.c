/*------------------------------------------------------------- 
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./client cse01 cse02"
   Client is run on cse02
--------------------------------------------------------------*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netdb.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT1 2000
#define PORT2 1234   
#include "header.h"
#define PCKT_LEN 8192

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
    int sockfd, n, one = 1;
    struct sockaddr_in serv_addr;
    const *val = &one;//
    struct hostent *server;//
    char buffer[5000];
    char datagram[4096], *pseudogram;
    memset(datagram, 0, 4096);
    struct tcpheader *tcp = (struct tcpheader *)(datagram);
    struct pseudo_h psh;
    
    /*  Find source/dest port, and header length and populate all fields of TCP segment  */
    /* A. Connection request TCP segment below */

    tcp->tcph_srcport = htons(PORT2); 
    tcp->tcph_destport = htons(PORT1); 
    /*  Add these 12 values to get checksum  */
    tcp->tcph_seqnum = 0;
    tcp->tcph_acknum = 0;
    tcp->tcph_offset = 5;
    tcp->tcph_reserved = 0; 
    tcp->tcph_fin = 0;
    tcp->tcph_syn = 1;
    tcp->tcph_rst = 0;
    tcp->tcph_psh = 0; 
    tcp->tcph_ack = 0;
    tcp->tcph_urg = 0;
    tcp->tcph_win = htons(5840); 
    tcp->tcph_urgptr = 0;
    tcp->tcph_chksum = 0; 

    /*   TCP Checksum   */
    psh.source_address = inet_addr(argv[1]);
    psh.dest_address = inet_addr(argv[2]);
    psh.placeholder = 0;
    psh.protocol = IPPROTO_TCP;
    psh.tcp_length = htons(sizeof(struct tcpheader));
    
    int psize = sizeof(struct pseudo_h) + sizeof(struct tcpheader);
    pseudogram = malloc(psize);

//size pseudo_h = 12
//size tcpheader = 24
//psize = 36

    memcpy(pseudogram, (char*)&psh, sizeof(struct pseudo_h));
    memcpy(pseudogram + sizeof(struct pseudo_h), tcp, sizeof(struct tcpheader));

    /*  Populate checksum  */
    tcp->tcph_chksum = csum((unsigned short*) pseudogram, psize); 


    /*   User must enter ./client cse01 cse02 */
    if (argc < 3) {
       printf("Must use the form: %s servname clientname\n", argv[0]);
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
    serv_addr.sin_port = htons(PORT1);// 80?
   
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        printf("Connection Failed. \n");
        return 1;
    }
    
    /*   Prints to client   */
    printf("Connection Successful\n\n");
    
       if (setsockopt (sockfd, IPPROTO_IP, IP_OPTIONS, val, sizeof (one)) < 0)
    {
        perror("setsockopt() error");
        exit(0);
    }
     
    printf("Request TCP Connection\nSequence Num: %d\nAckNum: %d\nSynBit: %d\nChecksum: %d\n", tcp->tcph_seqnum, tcp->tcph_acknum, tcp->tcph_syn, tcp->tcph_chksum);//
    /*   Request TCP from server   */
    bzero(buffer,5000);
    /*  Copy struct into a buffer and send to server  */
    memcpy(buffer, &tcp, sizeof(tcp));
    send(sockfd, buffer, sizeof(buffer), 0);
    bzero(buffer, 5000);
    
    /*   Reads reply from server, Message Received   */
    n = read(sockfd,buffer,4999);
    tcp->tcph_seqnum = 1;
    tcp->tcph_acknum = 1;
    tcp->tcph_ack = 1;
    
    /*  Populate checksum  */
    tcp->tcph_chksum = csum((unsigned short*) pseudogram, psize); 

    /*   Prints reply from server, Message Received   */
    sleep(1);//
    printf("%s\nAcknowledgement TCP\nSequence Num: %d\nAckNum: %d\nAckBit: %d\nChecksum: %d\n",buffer, tcp->tcph_seqnum, tcp->tcph_acknum, tcp->tcph_ack, tcp->tcph_chksum);


    /*  TCP Close Request  */
    tcp->tcph_fin = 1;
    tcp->tcph_chksum = csum((unsigned short*) pseudogram, psize); 

    printf("\nClose Request\nSequence Num: %d\nAckNum: %d\nFinBit: %d\nChecksum: %d\n", tcp->tcph_seqnum, tcp->tcph_acknum, tcp->tcph_fin, tcp->tcph_chksum);

return 0;
}