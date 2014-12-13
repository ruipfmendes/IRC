#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#define BUF_SIZE 1024

void erro(char *msg);

int main  (int argc, char *argv[]) {
    char endServer[100];
    int fd;
    struct hostent *hostPtr;
	struct sockaddr_in addr, serv_addr;

    if (argc != 4) {
        printf("cliente <host> <port> <string>\n");
        exit(-1);
    }
    strcpy(endServer, argv[1]);
    if ((hostPtr = gethostbyname(endServer)) == 0)
        erro("Nao consegui obter endereço");
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));
    if((fd = socket(AF_INET,SOCK_DGRAM,0)) == -1) //SOCK_DGRAM em vez de SOCK_STREAM
        erro("socket");
    if(sendto(fd, argv[3], strlen(argv[3])+1, 0,(struct sockaddr*)&addr,sizeof(addr))<0){
        erro("sendto");}
	close(fd);
	exit(0);
}
void erro(char *msg)
{
    printf("Erro: %s\n", msg);
    exit(-1);
}
