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

int main(int argc, char *argv[])
{    
    //char datagram[4096],  *data, *pseudogram;
    //memset(datagram, 0, 4096);
    struct tcpheader tcp;// from *tcp
    unsigned short int cksum_arr[12];//12 values to add in checksum
    unsigned int i, sum = 0, cksum; 

//data can be 128kb
   // data = datagram + sizeof(struct tcpheader);//
   // strcpy(data, "stuff"); //
    
    /*  Find source/dest port, and header length and populate all fields of TCP segment  */
    /* A. Connection request TCP segment below */
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1);  
    /*  Add these 12 values to get checksum  */
    tcp.seqnum = 0;
    tcp.acknum = 0;
    tcp.fin = 0;
    tcp.syn = 1;
    tcp.rst = 0;
    tcp.psh = 0; 
    tcp.ack = 0;
    tcp.urg = 0;
    tcp.recvWin = 0; //htons(5840)
    tcp.urgptr = 0;
    tcp.chksum = 0; 
    tcp.opt = 0; 

    //------------------------------------------------------------------//
    int sockfd, n, one = 1;
    const  *val = &one; 
    struct sockaddr_in serv_addr;
    struct hostent *server; 
    char buffer[5000];

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
    serv_addr.sin_port = htons(PORT1);
   
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        printf("Connection Failed. \n");
        return 1;
    }
                                                              
    printf("Connection Successful\n\n");                       
    //---------------------------------------------------------------------//
    
       if (setsockopt (sockfd, IPPROTO_IP, IP_OPTIONS, val, sizeof (one)) < 0)
    {
        perror("setsockopt() error");
        exit(0);
    }
    
    /*  copy 24 bytes for cksum calculation  */
    memcpy(cksum_arr, &tcp, 24); 

    /* Compute sum  */
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

    /*  TCP Request Header */
    printf("1. Connection Request TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Checksum: 0x%04X\n\n", (0xFFF^cksum));

    /*  First Send  */
    /*  memcpy structure to buffer */
    bzero(buffer,5000);
    memcpy(buffer, &tcp, sizeof(tcp)); 
    send(sockfd, buffer, sizeof(buffer), 0); 

    /*  First Read  */
    /*  Read struct from Server */
    bzero(cksum_arr, 12); //clear cksum_arr
    bzero(buffer, 5000);
    n = read(sockfd,buffer,4999);
    memcpy(&tcp, buffer, 4999);
    tcp.seqnum = 1; //initial client + 1 = 0+1
    tcp.acknum = 1; //initial server + 1 = 0+1
    tcp.ack = 1; 
    /* Copy 24 bytes for cksum calculation  */
    memcpy(cksum_arr, &tcp, 24); 
    /* Compute checksum */
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

    /*  Print Connection Granted  */
    printf("3. Acknowledge TCP Segment\n");
    printf("Source: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Checksum: 0x%04X\n", (0xFFF^cksum));
    //printf("Server Reply: %s\n",buffer);

return 0;
}