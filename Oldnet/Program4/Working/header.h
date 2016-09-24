// May create separate header file (.h) for all
// headers' structures

/* Structure of a TCP header */
struct tcpheader 
{
  short int srcport, destport; //16 bit
  int seqnum, acknum, opt; //32 bit, options set to zero
  char reserved:6, offset:4; 
  short int 
   /*  Flags combined == 6 bit  */
      fin:1, /*Finish flag "fin"*/
      syn:1, /*Synchronize sequence numbers to start a connection*/
      rst:1, /*Reset flag */
      psh:1, /*Push, sends data to the application*/
      ack:1, /*acknowledge*/
      urg:1; /*urgent pointer*/
  //char data[128]; //128byte

  short int recvWin, chksum, urgptr; // 16 bit, set to zero
 
  };

