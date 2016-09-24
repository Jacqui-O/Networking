// May create separate header file (.h) for all
// headers' structures

/* Structure of a TCP header */
struct tcpheader 
{
  unsigned short int tcph_srcport; //16 bit
  unsigned short int tcph_destport; //16 bit
  unsigned int tcph_seqnum; //32 bit
  unsigned int tcph_acknum; //32 bit
  unsigned char tcph_reserved:4, tcph_offset:4; //4 bit
  // unsigned char tcph_flags;
  unsigned int
      tcp_res1:4, /*little-endian*/
      tcph_hlen:4, /*length of tcp header in 32-bit words*/

   /*  Flags combined == 6 bit  */
      tcph_fin:1, /*Finish flag "fin"*/
      tcph_syn:1, /*Synchronize sequence numbers to start a connection*/
      tcph_rst:1, /*Reset flag */
      tcph_psh:1, /*Push, sends data to the application*/
      tcph_ack:1, /*acknowledge*/
      tcph_urg:1, /*urgent pointer*/

      tcph_res2:2;

  unsigned short int tcph_win; // 16 bit, set to zero
  unsigned short int tcph_chksum; // 16 bit, computed after header data is populated
  unsigned short int tcph_urgptr; // 16 bit, set to zero
  //unsigned int tcp_opt 32 bit options, set to zero
  };

  //for client2.c
  //typedef
  struct pseudo_h
{
    u_int32_t source_address;
    u_int32_t dest_address;
    u_int8_t placeholder;
    u_int8_t protocol;
    u_int16_t tcp_length;
};