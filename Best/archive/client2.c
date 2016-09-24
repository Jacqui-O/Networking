/*------------------------------------------------------------- 
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./client"
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
#define CSE01 "129.120.151.94"

void recvFile(int);
void list_file(char *);
int checksum(unsigned short int arr[], int size);

int checksum(unsigned short int arr[], int size)
 {
    // Checksum function accepts an int cksum_arr[]
    // Computes checksum 
    unsigned int i, sum = 0, cksum;
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

int main(void)
{
    int sockfd, n, one = 1;
    struct sockaddr_in serv_addr;
    const *val = &one;
    struct hostent *server;
    char buffer[5000];

    struct tcpheader tcp;
    unsigned short int cksum_arr[12];
    unsigned int cksum; 
    
    // Populate all fields of TCP segment
    // Connection request TCP segment below 
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
    // Add these 12 values to get checksum  

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket. \n");
 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(CSE01);
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

    //  TCP Request Header Segment
    printf("1. Connection Request TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));

snprintf(buffer, 5000, "1.Request\nSource:0x%04X\nDest:0x%04X\nSequence:0x%08X\nAckNum:0x%08X\n" 
     "FinBit:0x%04X\nSinBit:0x%04X\nRst:0x%04X\nPsh:0x%04X\nAck:0x%04X\nUrg:0x%04X\nRecvWin:0x%04X\n" 
     "UrgPtr:0x%04X\nChecksum:0x%04X", tcp.srcport, tcp.destport, tcp.seqnum,tcp.acknum, tcp.fin, tcp.syn,tcp.rst, tcp.psh, tcp.ack, tcp.urg,tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     list_file(buffer);  
    /*  Copy struct into a buffer and send to server  */
    //bzero(buffer,5000);
    //memcpy(buffer, &tcp, sizeof(tcp));
    //send(sockfd, buffer, sizeof(buffer), 0);

    /*   Reads reply from server, Message Received   */
    //bzero(buffer, 5000);
    //n = read(sockfd,buffer,4999);
    //memcpy(&tcp, buffer, 4999);

    // Connection Granted Segment
    // Copy 24 bytes for 2nd cksum calculation
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1); 
    tcp.seqnum = 1; // initial client + 1 = 0+1
    tcp.acknum = 1; // initial server + 1 = 0+1
    tcp.ack = 1;     tcp.syn = 1; 
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);
    printf("3. Acknowledge TCP Segment\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));  
 
  snprintf(buffer, 5000, "3.Acknowledge\nSource:0x%04X\nDest:0x%04X\nSequence:0x%08X\nAckNum:0x%08X\n" 
     "FinBit:0x%04X\nSinBit:0x%04X\nRst:0x%04X\nPsh:0x%04X\nAck:0x%04X\nUrg:0x%04X\nRecvWin:0x%04X\n" 
     "UrgPtr:0x%04X\nChecksum:0x%04X", tcp.srcport, tcp.destport, tcp.seqnum,tcp.acknum, tcp.fin, tcp.syn,tcp.rst, tcp.psh, tcp.ack, tcp.urg,tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     list_file(buffer);  
    // Receive 1kb file
    recvFile(sockfd);

    // TCP Close Request  
    // Copy 24 bytes for 3rd cksum calculation
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1);  
    tcp.seqnum = 0;
    tcp.acknum = 0;
    tcp.fin = 1;
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);
    printf("\n4. Request close TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
   
   snprintf(buffer, 5000, "4.Close\nSource:0x%04X\nDest:0x%04X\nSequence:0x%08X\nAckNum:0x%08X\n" 
     "FinBit:0x%04X\nSinBit:0x%04X\nRst:0x%04X\nPsh:0x%04X\nAck:0x%04X\nUrg:0x%04X\nRecvWin:0x%04X\n" 
     "UrgPtr:0x%04X\nChecksum:0x%04X", tcp.srcport, tcp.destport, tcp.seqnum,tcp.acknum, tcp.fin, tcp.syn,tcp.rst, tcp.psh, tcp.ack, tcp.urg,tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     list_file(buffer);  
    // Final Close Segment
    tcp.seqnum = 1;
    tcp.acknum = 1;
    tcp.ack = 1; 
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);
    printf("7. Final Close\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
   
   snprintf(buffer, 5000, "7.Grant Close\nSource:0x%04X\nDest:0x%04X\nSequence:0x%08X\nAckNum:0x%08X\n" 
     "FinBit:0x%04X\nSinBit:0x%04X\nRst:0x%04X\nPsh:0x%04X\nAck:0x%04X\nUrg:0x%04X\nRecvWin:0x%04X\n" 
     "UrgPtr:0x%04X\nChecksum:0x%04X", tcp.srcport, tcp.destport, tcp.seqnum,tcp.acknum, tcp.fin, tcp.syn,tcp.rst, tcp.psh, tcp.ack, tcp.urg,tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     list_file(buffer);  
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

    tcp.seqnum = 0;

    // Receive data in chunks of 128bytes 
    // Sequence number of zero and AckNum equal to data payload
    while((bytesReceived = read(sockfd, recvBuff, 128)) > 0)
    {
        printf("Receiving\n");   
        printf("Sequence: %d\nAckNum: %d\n\n", tcp.seqnum, bytesReceived);
        fwrite(recvBuff, 1,bytesReceived,fp);
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error \n");
    }
}

void list_file(char *data)
{
  //  Function saves segments into file
    FILE *f = fopen("list.txt", "a+");
    if(!f)
          return;

  // Put data into f  
  fprintf(f, "%s\n", data);//arr?
 
  return;
  fclose(f);
} /*  End list_file  */