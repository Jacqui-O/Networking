/*------------------------------------------------------------- 
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./client cse01 cse02"
   Client is run on cse02
   networking->program3->archive
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
   //define cse01 and cse02

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
    struct sockaddr_in serv_addr;
    const *val = &one;//
    struct hostent *server;//
    char buffer[5000];

    struct tcpheader tcp;
    unsigned short int cksum_arr[12];
    unsigned int cksum; 
    
   /*  Find source/dest port, and header length and populate all fields of TCP segment
        A. Connection request TCP segment below */
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
    /*  Add these 12 values to get checksum  */

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

    printf("Connection Successful\n\n");
    
       if (setsockopt (sockfd, IPPROTO_IP, IP_OPTIONS, val, sizeof (one)) < 0)
    {
        perror("setsockopt() error");
        exit(0);
    }

    /*  TCP Request Header */
    printf("1. Connection Request TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));


    bzero(buffer,5000);
    /*  Copy struct into a buffer and send to server  */
    memcpy(buffer, &tcp, sizeof(tcp));
    send(sockfd, buffer, sizeof(buffer), 0);
    bzero(buffer, 5000);
    



    /*   Reads reply from server, Message Received   */
    n = read(sockfd,buffer,4999);
    memcpy(&tcp, buffer, 4999);
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1); 
    tcp.seqnum = 1; // initial client + 1 = 0+1
    tcp.acknum = 1; // initial server + 1 = 0+1
    tcp.ack = 1;
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);
    /* Copy 24 bytes for 2nd cksum calculation  */

    /*  Print Connection Granted  */
    printf("3. Acknowledge TCP Segment\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));  
  
  //send 1kb file
    /* Create file where data will be stored; server instead? */
    FILE *fp = fopen("Sample.txt","rb");
        if(fp==NULL)
        {
            printf("File open error");//error
            return 1;   
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 256 bytes */
            unsigned char buff[128]={0};
            //memcpy()
            int nread = fread(buff,1,128,fp);
            printf("Bytes read %d \n", nread);        

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(sockfd, buff, nread);//prints out on client
            }

            /*
             * There is something tricky going on with read .. 
             * Either there was error, or we reached end of file.
             */
            if (nread < 256)
            {
                if (feof(fp))
                    printf("End of file\n");
                if (ferror(fp))
                    printf("Error reading\n");
                break;
            }

        }
        close(sockfd);

////////////////////////////////////////////////////////////////
    /*  TCP Close Request  */
   //Copy 24 bytes for 3rd cksum calculation
    tcp.srcport = htons(PORT2); 
    tcp.destport = htons(PORT1);  
    tcp.seqnum = 0;
    tcp.acknum = 0;
    tcp.ack = 0; 
    tcp.fin = 1;
    memcpy(cksum_arr, &tcp, 24); 
    cksum = checksum(cksum_arr, 12);

    printf("4. Request close TCP\n");
    printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
    //send struct
  
          


return 0;
}