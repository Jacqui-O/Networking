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

 /* Checksum function accepts and int cksum_arr 
    checksum(cksum_arr, 12);
 */
int checksum(unsigned short int arr[], int size);

 int checksum(unsigned short int arr[], int size)
 {
    unsigned int i, sum = 0, cksum;
     /*  Compute checksum  */
     for(i=0; i<size; i++)
     sum = sum+arr[i]; 

    /*  Fold once  */
    cksum = sum >> 16; 
    sum  = sum & 0x0000FFFF;
    sum = cksum + sum; 

    /*  Fold once more  */
    cksum = sum >> 16; 
    sum  = sum & 0x0000FFFF;
    cksum = cksum + sum;
    return cksum;
    /* Returns cksum total */
 }

int main(int argc, char *argv[])
{    
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
   
    
       if (setsockopt (sockfd, IPPROTO_IP, IP_OPTIONS, val, sizeof (one)) < 0)
    {
        perror("setsockopt() error");
        exit(0);
    }
    //---------------------------------------------------------------------//
    
    struct tcpheader tcp;
    unsigned short int cksum_arr[12];
    unsigned int cksum; 
    
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
    tcp.recvWin = 0; 
    tcp.urgptr = 0;
    tcp.chksum = 0; 
    tcp.opt = 0; 
    //tcp.data[128];
    
    /*  copy 24 bytes for 1st cksum calculation  */
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);//right?

    /*  TCP Request Header; can sprintf to save all data in a buffer and send to client */
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
    bzero(buffer, 5000);
    n = read(sockfd,buffer,4999); // not reading until Enter on server side
    memcpy(&tcp, buffer, 4999);
    tcp.seqnum = 1; // initial client + 1 = 0+1
    tcp.acknum = 1; // initial server + 1 = 0+1
    tcp.ack = 1; 
    
    /* Copy 24 bytes for 2nd cksum calculation  */
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12); 

    /*  Print Connection Granted  */
    printf("3. Acknowledge TCP Segment\n");
    printf("Source: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Checksum: 0x%04X\n\n", (0xFFF^cksum));
    
    //Transfer a 1KB file from client to server

    //memset(&tcp, 0, sizeof(tcp)); // OK??
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1);  
    /*  Add these 12 values to get checksum  */
    tcp.seqnum = 0;
    tcp.acknum = 0;
    tcp.ack = 0; 
    tcp.fin = 1;
    /* Copy 24 bytes for 3rd cksum calculation  */
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);

    /* Print close request TCP Segment */
    printf("Request close TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Checksum: 0x%04X\n", (0xFFF^cksum));
    printf("Press Enter");
    /* Second send */
    bzero(buffer, 5000); //clear buffer for send
    memcpy(buffer, &tcp, sizeof(tcp)); 
    send(sockfd, buffer, sizeof(buffer), 0); 

    /* 2nd Read */
    bzero(buffer, 5000);
    n = read(sockfd,buffer,4999); //not reading until Enter on server side
    memcpy(&tcp, buffer, 4999);
    tcp.seqnum = 1; 
    tcp.acknum = 1;
    tcp.ack = 1;
    /* Copy 24 bytes for 4th cksum calculation  */
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);
    //tcp.chksum = cksum; 
    printf("ACK TCP Seg\n");
    printf("Source: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Checksum: 0x%04X\n\n", (0xFFF^cksum));
return 0;
}