/* -----------------------------------------
   Jacqui Oquendo
   CSCE 3530
   To run, user must enter "./server "
   After program is run a few times, files will be saved in current directory.
   One will be list.txt, and others will be webData files with timestamps as 
   the naming convention YYYYMMDDhhmmss.

   To view contents of webData files type "joe YYYYMMDDhhmmss\" and Enter
-------------------------------------------*/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PORT 2000


void  fetch(int);
int   domain_to_ip(char *, char *);
void  list_file(char *, char *);
void  writeTofile(char *, char *);
int  blacklist(char *, char *);//from void
void  getTime(char *);


int main(int argc, char *argv[])
{
     struct sockaddr_in servAddr, clidAddr;

     int listenfd = socket(AF_INET, SOCK_STREAM, 0);
            printf("Socket retrieve success.\n");
     if (listenfd < 0)
            printf("Failed to listen.\n");//for testing
     
     bzero((char *) &servAddr, sizeof(servAddr));
     servAddr.sin_family = AF_INET;
     servAddr.sin_addr.s_addr = INADDR_ANY;
     servAddr.sin_port = htons(PORT);

     /*   Error when server is busy   */
     if (bind(listenfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
              printf("Binding error.\n");//for testing
     listen(listenfd,5);
     int length = sizeof(clidAddr);
        
        /*  While (1) keeps server on  */
        while (1) 
        {
           int connfd = accept(listenfd, (struct sockaddr *) &clidAddr, &length);
            if (connfd < 0)
                printf("Error on accept.\n");//for testing
           int pid = fork();
            if (pid < 0)
                printf("Error on fork.\n");//for testing
            if (pid == 0)  
            {
                close(listenfd);
                fetch(connfd);
                exit(0);
            }
            /*  End if  */
            else close(connfd);
        } 
        /*  End while loop  */
return 0; 
}

void fetch (int sock)
{
 /* Function connects proxy server to web server. Recieves domain from client and it
  * gets converted to an IP address through domain_to_ip function. 
  * sock is socket with client.
  * serv_sock is socket with web server.
  */
 
    int n;
    char buffer[256], address[16];
    int serv_sock;
    struct sockaddr_in server;
    char  response[2000], msg[2000], *domain, ip[16], ts[20];

   /*  Clears buffer and Reads URL from client  */
    bzero(buffer,256);
    n = read(sock,buffer,255);
      if (n < 0) 
          printf("Error reading from socket");//for testing
    /*  Get current time  */
    getTime(ts);
   
   /*  Remove newline from buffer; Error if domain not valid  */
    buffer[n-1] = '\0';
    domain = buffer;
      if(domain == NULL)
      {
          herror("gethostbyname");//for testing         
          return ;
      }

   /*  Converts domain to ip address  */
    domain_to_ip(domain, ip); 
    
      
    /*  Create socket to web server  */
    serv_sock = socket(AF_INET , SOCK_STREAM , 0);
      if (serv_sock <0)
      {
          printf("Could not create socket");//for testing
      }

    /*  Fill structure  */
    server.sin_addr.s_addr = inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons( 80 );

    /*  Connect to the web server  */
    connect(serv_sock , (struct sockaddr *)&server , sizeof(server));
    
    if(!blacklist(domain, ts))//if true
          printf("Website blocked.\n\n");
      
    else
    {
      /*  Do everything down here  
       *  Add url and timestamp to list.txt
       */
          list_file(domain, ts); 

      /*  Print GET request and send to the web server  */
          snprintf(msg, 2000, "GET  / HTTP/1.1\r\nHost: %s\r\n\r\n", domain);
          printf("%s", msg);
          send(serv_sock , msg , strlen(msg) , 0);

    /*  Check if receive a reply from the server  */
      if( recv(serv_sock, response , 2000 , 0) < 0)
      {
          printf("Receive failed.\n");
      }

      else 
      /*  Write a reply to client 
       *  Save reponse to file YYYY..      
       */
          n = write(sock, response, 2000);
          writeTofile(response, ts);//pass ts

      if (n < 0) 
          printf("Error writing to socket.\n");//for testing
    }/*  End else  */

} /*  End fetch  */


int domain_to_ip(char *domain , char *ip)
{
  /*   Function converts domain to IP address   */
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
    int rv;
 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; 
    hints.ai_socktype = SOCK_STREAM;
 
      if ( (rv = getaddrinfo( domain , "http" , &hints , &servinfo)) != 0) 
      {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
      }
 
      /*   Loop through all the results and connect to the first we can  */
      for(p = servinfo; p != NULL; p = p->ai_next) 
      {
        h = (struct sockaddr_in *) p->ai_addr;
        strcpy(ip , inet_ntoa( h->sin_addr ) );
      }
     
     /*   Clear out structure   */
  freeaddrinfo(servinfo); 
  return 0;

} /*  End domain_to_ip  */


void list_file(char *visitedSite, char *currentTime)
{
  /*  Function stores the URL and timestamp into list.txt  
   *  Let's user know if website is cached or not
   */
    char    buf[80], cache[1000], arr[200];
    int     result = -1, i=0;
  

    /*  Append data to this file  */
    FILE *listFile = fopen("list.txt", "a+");
    if(!listFile)
          return;

    /*  Need newline after visited site for cache to recognize  */
    snprintf(arr, 200, "%s\n", visitedSite);

      for(i = 0; fgets(cache, 1000, listFile) != NULL; i++)
      {
      /*  This wont happen if visitedSite doesn't have a new line  */
          result = strncmp(arr, cache, result);
            if(result == 0)
            {
              printf("Visited Already.\n"); 
              return;
            }

          result = -1;
      }
  /*  Put URL's and timestamps into listFile  */
  fprintf(listFile, "%s  %s\n", arr, currentTime);
  printf("Added to cache.\n");
  return;
  fclose(listFile);
} /*  End list_file  */


void writeTofile(char *response, char *currentTime)//pass time
{
    /*  Function stores the web data into a file YYYYMM..  */  
    FILE *webData = fopen(currentTime, "w");//write, from buf
    if(!webData)
          return;

    /* Put reponse into webData file YYYY.. */
    fprintf(webData, "%s", response);
        return;

    fclose(webData);

} /*  End writeTofile  */


int blacklist(char *website, char *currentTime)
{
  //do something with currentTime
  //convert to integer
  //add start and stop time comparison
  FILE *blacklistFile = fopen("blacklist.txt", "r");

  char buffer[1000];
  int i = 0, result = -1;

  /* Check if file is exists */
  if(!blacklistFile)
    return 0;

  /*  Go through file line by line */
  for(i = 0; fgets(buffer, 1000, blacklistFile) != NULL; i++)
  {
    /* Compare website to those in the list */
    result = strncmp(website, buffer, result);
    //convert string to int with atoi and compare to currentTime
    /* if websites match, state blacklisted website and skip print response */
    if(result == 0)
    {

      /*  Blacklisted  */
      return 1; 
    }
  }
  /*  Otherwise the website was valid  */
  fclose(blacklistFile);
  return 0;

} /* End of blacklist function  */


void getTime(char *currentTime)
{
  /*  Function gets current time  */  
    time_t  now;
    struct tm *ts;
    char buf[80];

    /* Get the current time */
    now = time(NULL);

    /*  Format the time YYYYMMDDhhmmss */
    ts = localtime(&now);
    strftime(buf, sizeof(buf), "%Y%m%d%H%M%S ", ts);//add newline
    strcpy(currentTime, buf);
}

/*
Need: 
      Separate domain from path (strtok, strchr, etc...)
      Refine Blacklist
*/