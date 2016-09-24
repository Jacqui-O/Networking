/*------------------------------------------------------------- 
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./client cse01 cse02"
   Client is run on cse02
   networking->program3->archive

--------------------------------------------------------------*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#define PORT1 2000
#define PORT2 1234   
#include "header.h"

void recvFile(int);

// Checksum function accepts an int cksum_arr[] 
int checksum(unsigned short int arr[], int size);

int checksum(unsigned short int arr[], int size)
 {
    unsigned int i, sum = 0, cksum;
    //  Compute checksum  
    for(i=0; i<size; i++)
     	sum = sum+arr[i]; 

    // Fold once  
    cksum = sum >> 16; 
    sum  = sum & 0x0000FFFF;
    sum = cksum + sum; 

    //  Fold once more  
    cksum = sum >> 16; 
    sum  = sum & 0x0000FFFF;
    cksum = cksum + sum;
    return cksum;
    // Returns cksum total 
 }

int main(int argc, char *argv[])
{
    int sockfd = 0, n;
    struct hostent *server;
    char buffer[1024];//recvBuff[256];
    memset(buffer, '0', sizeof(buffer));
    struct sockaddr_in serv_addr;

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

    // Attempt a connection 
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
//***************************************************************************    
	struct tcpheader tcp;
    unsigned short int cksum_arr[12];
    unsigned int cksum; 

    //   Find source/dest port, and header length and populate all fields of TCP segment
    //   Connection request TCP segment below 
    //   Add these 12 values to get checksum
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1);  
    tcp.seqnum = 0;  tcp.acknum = 0;
    tcp.fin = 0;     tcp.syn = 1;
    tcp.rst = 0;     tcp.psh = 0;
    tcp.ack = 0;     tcp.urg = 0;
    tcp.opt = 0;     tcp.urgptr = 0;
    tcp.chksum = 0;  tcp.recvWin = 0;
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12); 
    //  TCP Request Header 
    printf("1. Connection Request TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
    // Copy struct into a buffer and send to server  
    //bzero(buffer,1024);
    //memcpy(buffer, &tcp, sizeof(tcp));
    //send(sockfd, buffer, sizeof(buffer), 0);

    /* Create file where data will be stored */
    //call createFile

    recvFile(sockfd);


    return 0;
}

void recvFile(int sockfd)
{
	int bytesReceived = 0;
    char recvBuff[128];
    struct tcpheader tcp;
    FILE *fp;
    fp = fopen("recipe.txt", "ab"); 
    if(NULL == fp)
    {
        printf("Error opening file");
    }

    /* Receive data in chunks of 128bytes */
    while((bytesReceived = read(sockfd, recvBuff, 128)) > 0)
    {
        printf("Bytes received %d\n",bytesReceived);   
        //print ack 
        // recvBuff[n] = 0;
        fwrite(recvBuff, 1,bytesReceived,fp);
        // printf("%s \n", recvBuff);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");//get read error when adding tcp
    }
}
