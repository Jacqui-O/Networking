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
 
void recvFile(int); 

/* Checksum function accepts and int cksum_arr  */
int checksum(unsigned short int arr[], int size);

int checksum(unsigned short int arr[], int size)
 {
    unsigned int i, sum=0, cksum;
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
     int listenfd, connfd, addrlen, n;
     char recvBuff[5000];
     struct sockaddr_in serv_addr, cli_addr;
     
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
     //start while(1)
     connfd = accept(listenfd, (struct sockaddr *) &cli_addr, &addrlen);              
//-----------------------------End of Connect to Client--------------------------------------------------//
     
     struct tcpheader tcp;  
     unsigned short int cksum_arr[12]; 
     unsigned int cksum;  

     /*  1st Read  */
     /*  Reading 1st struct sent from client; memcpy buffer to struct  */
     bzero(recvBuff,5000);
     n = read(connfd,recvBuff,4999); 
     memcpy(&tcp, recvBuff, 4999);  
     tcp.seqnum = 0;
     tcp.acknum = 1; // client seq + 1 = 0+1 
     tcp.syn = 1;
     tcp.ack = 1; 
     
     /* Copy 24 bytes for 1st cksum calculation  */
     memcpy(cksum_arr, &tcp, 24); 
     cksum = checksum(cksum_arr, 12);

     /*  Print Connection Granted; can sprintf to save all data in a buffer and send to client, and send to writeToFile */
     printf("2. Connection Granted TCP\n");
     printf("Src: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
     printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
     printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
     printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
     printf("Chksum: 0x%04X\n", (0xFFF^cksum));

    /*  1st Send  */
    /*  memcpy structure to buffer */
    bzero(recvBuff,5000);
    memcpy(recvBuff, &tcp, sizeof(tcp)); 
    send(connfd, recvBuff, sizeof(recvBuff), 0); //doesnt send until press Enter

//---------------------------------Receive 1KB file--------------------------------------------
      FILE *fp = fopen("Sample.txt", "ab");//a+ append, ab open and create file
    int bytesReceived = 0;
    char fileBuff[128];
    bzero(fileBuff, 128);
    
    if(fp == NULL)
    {
        printf("File not found\n");
        return ;  
    }
    /* Receive data in chunks of 128 bytes */
    while((bytesReceived = read(connfd, fileBuff, 128 )) > 0)
    {
        //for testing
        printf("bytes received %d\n", bytesReceived);
        printf("%s", fileBuff); //print file that is received
        //only receiving 1 byte at a time, not writing to server
       // fwrite(fileBuff, 1, bytesReceived, fp);//sizeof(char) instead of 1
        //here is where is send ack
    }

    if(bytesReceived < 0)
    {
        printf("\n Read Error\n");
    }

//--------------------------------End of Receive File----------------------------------------
//receiving only 1 byte at a time, not writing here


    /* 2nd Read Request Close */
    bzero(recvBuff, 5000); 
    n = read(connfd,recvBuff,4999); 
    memcpy(&tcp, recvBuff, 4999);


    /* Acknowledge TCP close */
    tcp.seqnum = 0; // initial
    tcp.acknum = 1; 
    tcp.ack = 1;
    /*  copy 24 bytes for 2nd cksum calculation  */
     memcpy(cksum_arr, &tcp, 24); 
     cksum = checksum(cksum_arr, 12);

    printf("5. Acknowledge TCP Close\n");
    printf("Src: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Chksum: 0x%04X\n\n", (0xFFF^cksum));

    /* Another Close ACK */
    tcp.fin = 1;
    /*  copy 24 bytes for 2nd cksum calculation  */
     memcpy(cksum_arr, &tcp, 24); 
     cksum = checksum(cksum_arr, 12);
     printf("6. Another Close ACK\n");
    printf("Src: 0x%04X  Dest: 0x%04X\n", tcp.srcport, tcp.destport); 
    printf("Sequence: 0x%08X\nAckNum: 0x%08X\nFinBit: 0x%04X\n", tcp.seqnum, tcp.acknum, tcp.fin);
    printf("SinBit: 0x%04X\nRst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.syn, tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\n", tcp.recvWin, tcp.urgptr);
    printf("Chksum: 0x%04X\n\n", (0xFFF^cksum));

    /* 2nd Send */
    bzero(recvBuff,5000);
    memcpy(recvBuff, &tcp, sizeof(tcp));
    //writeTofile(recvBuff); //copy 2nd server segment
    send(connfd, recvBuff, sizeof(recvBuff), 0);

return 0; 
}

/*{
    FILE *fp = fopen("Sample.txt", "ab");//a+ append, ab open and create file
    int bytesReceived = 0;
    char fileBuff[128];
    bzero(fileBuff, 128);
    
    if(fp == NULL)
    {
        printf("File not found\n");
        return ;  
    }
    /* Receive data in chunks of 128 bytes */
   /* while((bytesReceived = read(connfd, fileBuff, 128 )) > 0)*/
    //{
        //for testing
      //  printf("bytes received %d\n", bytesReceived);
        //printf("%s", fileBuff); //print file that is received
        //only receiving 1 byte at a time, not writing to server
       // fwrite(fileBuff, 1, bytesReceived, fp);//sizeof(char) instead of 1
        //here is where is send ack
 //   }

 //   if(bytesReceived < 0)
//    {
    //    printf("\n Read Error\n");
  //  }
  //  fclose(fp);//
//}
