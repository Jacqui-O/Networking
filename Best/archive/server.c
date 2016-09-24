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
#define PORT1 2000
#define PORT2 1234  
#define CSE01 "129.120.151.94"

void readFile(int);
void list_file(char *);
int checksum(unsigned short int arr[], int size);

int checksum(unsigned short int arr[], int size)
 {
    // Checksum function accepts and int cksum_arr 
    // Compute checksum 
    unsigned int i, sum=0, cksum; 
    for(i=0; i<size; i++)
        sum = sum+arr[i]; 

    //  Fold once  
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
    int listenfd, connfd, addrlen, n;
    char recvBuff[5000];
    struct sockaddr_in serv_addr, cli_addr;
    
     struct tcpheader tcp;  
     unsigned short int cksum_arr[12]; 
     unsigned int cksum;  

     listenfd = socket(AF_INET, SOCK_STREAM, 0);
     printf("Socket retrieve success.\n");
     
 
     if (listenfd < 0)
        printf("Failed to listen. \n");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = inet_addr(CSE01);
     serv_addr.sin_port = htons(PORT1);
    
     // Error when server is busy 
     if (bind(listenfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
              printf("Binding Error. \n");
              
     listen(listenfd,5);
     addrlen = sizeof(cli_addr);
     connfd = accept(listenfd, (struct sockaddr *) &cli_addr, &addrlen);
     
     bzero(recvBuff,5000);
           
     /* Reads message from client TCP header   */
     //n = read(connfd,recvBuff,4999); 
     /* Copy buffer into a struct  */
     //memcpy(&tcp, recvBuff, 4999);

     // Connection Granted
     // Copy 24 bytes for 1st cksum calculation
     tcp.srcport = htons(PORT2); 
     tcp.destport = htons(PORT1); 
     tcp.seqnum = 0;  tcp.acknum = 1; // client seq + 1 = 0+1 
     tcp.syn = 1;     tcp.ack = 1;
     tcp.fin = 0;     tcp.rst = 0;     
     tcp.psh = 0;     tcp.urg = 0;
     tcp.opt = 0;     tcp.urgptr = 0;
     tcp.recvWin = 0;
     memcpy(cksum_arr, &tcp, 24); 
     cksum = checksum(cksum_arr, 12);
     printf("2. Connection Granted TCP\n");
     printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
     printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
     printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
     printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     
     snprintf(recvBuff, 5000, "2.Granted\nSource:0x%04X\nDest:0x%04X\nSequence:0x%08X\nAckNum:0x%08X\n" 
     "FinBit:0x%04X\nSinBit:0x%04X\nRst:0x%04X\nPsh:0x%04X\nAck:0x%04X\nUrg:0x%04X\nRecvWin:0x%04X\n" 
     "UrgPtr:0x%04X\nChecksum:0x%04X", tcp.srcport, tcp.destport, tcp.seqnum,tcp.acknum, tcp.fin, tcp.syn,tcp.rst, tcp.psh, tcp.ack, tcp.urg,tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     list_file(recvBuff);   
    /* Writes on Client Side message received */
    //send(connfd, recvBuff, sizeof(recvBuff), 0);
   
    // Receive 1kb file
    readFile(connfd);
   
    //memcpy struct, receive
    // Acknowledge TCP close
    // Copy 24 bytes for 2nd cksum calculation
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1); 
    tcp.seqnum = 0; // Initial
    tcp.acknum = 1; 
    tcp.ack = 1;
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);
    printf("5. Acknowledge TCP Close\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
   
    snprintf(recvBuff, 5000, "5.Close\nSource:0x%04X\nDest:0x%04X\nSequence:0x%08X\nAckNum:0x%08X\n" 
     "FinBit:0x%04X\nSinBit:0x%04X\nRst:0x%04X\nPsh:0x%04X\nAck:0x%04X\nUrg:0x%04X\nRecvWin:0x%04X\n" 
     "UrgPtr:0x%04X\nChecksum:0x%04X", tcp.srcport, tcp.destport, tcp.seqnum,tcp.acknum, tcp.fin, tcp.syn,tcp.rst, tcp.psh, tcp.ack, tcp.urg,tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     list_file(recvBuff);  
    // Last TCP Close
    // Copy 24 bytes for 3rd cksum calculation  
    tcp.acknum = 1; // Initial +1
    tcp.fin = 1;
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);
    printf("6. Another TCP Close\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
    
    snprintf(recvBuff, 5000, "6.Final\nSource:0x%04X\nDest:0x%04X\nSequence:0x%08X\nAckNum:0x%08X\n" 
     "FinBit:0x%04X\nSinBit:0x%04X\nRst:0x%04X\nPsh:0x%04X\nAck:0x%04X\nUrg:0x%04X\nRecvWin:0x%04X\n" 
     "UrgPtr:0x%04X\nChecksum:0x%04X", tcp.srcport, tcp.destport, tcp.seqnum,tcp.acknum, tcp.fin, tcp.syn,tcp.rst, tcp.psh, tcp.ack, tcp.urg,tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
     list_file(recvBuff);  
return 0; 
}

void readFile(int connfd)
{ 
    // Open the file that we wish to transfer
    struct tcpheader tcp;
    unsigned short int cksum_arr[12]; 
    unsigned int cksum; 
    FILE *fp = fopen("Sample.txt","rb");
        if(fp==NULL)
        {
            printf("File opern error");  
        }   
        tcp.seqnum = 0;
        tcp.acknum = 0;

        // Read data from file and send it 
        while(1)
        {
            // First read file in chunks of 128 bytes 
            unsigned char buff[128]={0};
            int nread = fread(buff,1,128,fp);       

            // If read was success, send data. 
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread);
                printf("Sequence: 0x%08X\nAckNum: 0x%08X\n", tcp.seqnum, tcp.acknum); //
                printf("Checksum: 0x%04X\n\n", (0xFFF^cksum));//
                memcpy(cksum_arr, &tcp, 24); 
                cksum = checksum(cksum_arr, 12);
                tcp.seqnum = tcp.seqnum + tcp.acknum;
                tcp.acknum = tcp.acknum+128;
            }

            if (nread < 128)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }


        }
}

void list_file(char *data)
{
  //  Function saves segments into file
    FILE *f = fopen("list.txt", "a+");
    if(!f)
          return;

  // Put data into f  
  fprintf(f, "%s\n", data);
 
  return;
  fclose(f);
} /*  End list_file  */