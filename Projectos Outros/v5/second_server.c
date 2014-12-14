#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>

#define buffer_size 300;
int main()
{
    int sd;
    int port;
    char host [60];
    struct hostent *hostPtr;

    struct sockaddr_in addr, serv_addr;
    int servidor;
    int serv_addr_size;

    strcpy(host, "localhost");
    port = 8000;

    if ((hostPtr = gethostbyname(host)) == 0){
        perror("While getting host name");
    }

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) port);

    if((sd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("While creating socket");
        return 1;
    }

    if( connect(sd,(struct sockaddr *)&addr,sizeof (addr)) < 0){
        perror("While connecting");
        return 1;
    }
    while(1){
        process_server(sd);
    }
    return 0;
}

void process_server(int sd){

    int nread;
    char buffer[buffer_size];
    nread=read(sd, buffer, buffer_size+1)
    fflush(stdout);
    buffer[nread]='\0';
}