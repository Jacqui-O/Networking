/* -----------------------------------------
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./server "
-------------------------------------------*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include "header.h"
#define PORT 2000
#define PORT2 1234 

void readFile(int);
 /* Checksum function accepts and int cksum_arr */
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

int main(void)
{
    int listenfd = 0;
    int connfd = 0;
    struct sockaddr_in serv_addr, cli_addr;
    char sendBuff[1024];//1024
    int numrv;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    printf("Socket retrieve success\n");

    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    bzero((char *) &serv_addr, sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = (INADDR_ANY);
     serv_addr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr));

    if(listenfd < 0)
        printf("Failed to listen\n");
    listen(listenfd,5);
    numrv = sizeof(cli_addr);     


        connfd = accept(listenfd, (struct sockaddr*)&cli_addr, &numrv);
//*******************************************************************
  struct tcpheader tcp;  
     	unsigned short int cksum_arr[12]; 
     	unsigned int cksum;  
        
     	// Reads message from client TCP header 
     	// Copy buffer into a struct   
     	//read(connfd,sendBuff,1023); //n=
     	//memcpy(&tcp, sendBuff, 1023);
     	tcp.seqnum = 0;
     	tcp.acknum = 1; // client seq + 1 = 0+1 
     	tcp.syn = 1;
     	tcp.ack = 1;
     	// Copy 24 bytes for 1st cksum calculation
    	memcpy(cksum_arr, &tcp, 24); 
     	cksum = checksum(cksum_arr, 12);
     
     	// Print Connection Granted 
     	printf("2. Connection Granted TCP\n");
    	printf("Source: 0x%04X  Dest: 0x%04X\nSequence: 0x%08X\n", tcp.srcport, tcp.destport, tcp.seqnum); 
    	printf("AckNum: 0x%08X\nFinBit: 0x%04X\nSinBit: 0x%04X\n", tcp.acknum, tcp.fin, tcp.syn);
    	printf("Rst: 0x%04X\nPsh: 0x%04X\nAck: 0x%04X\nUrg: 0x%04X\n", tcp.rst, tcp.psh, tcp.ack, tcp.urg);
    	printf("RecvWin: 0x%04X\nUrgPtr: 0x%04X\nChecksum: 0x%04X\n\n", tcp.recvWin, tcp.urgptr, (0xFFF^cksum));
        
        /* Open the file that we wish to transfer; put in client */
      //call recvFIle here
    	//sleep(1);
        readFile(connfd);
   	 	 
        close(connfd);
        //sleep(1);


    return 0;
}

void readFile(int connfd)//pass buffer, or create new buffer
{
	FILE *fp = fopen("Sample.txt","rb");
        if(fp==NULL)
        {
            printf("File opern error");  
        }   

        /* Read data from file and send it */
        while(1)
        {
            /* First read file in chunks of 128 bytes */
            unsigned char buff[128]={0};
            int nread = fread(buff,1,128,fp);
            printf("Bytes read %d \n", nread);        

            /* If read was success, send data. */
            if(nread > 0)
            {
                printf("Sending \n");
                write(connfd, buff, nread);
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

