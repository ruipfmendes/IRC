#include "header.h"


void erro(char *msg);
void process_server(int servidor_fp);

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
    if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        erro("socket");
    if( connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0)
	erro("Connect");
    write(fd, argv[3], 1 + strlen(argv[3]));
    process_server(fd);
	close(fd);
	exit(0);
}

void process_server(int server_fd)
{
	
	int nread = 0;
	char buffer[BUF_SIZE];
	nread = read(server_fd,buffer,BUF_SIZE-1);
	buffer[nread]='\0';
	printf("%s\n",buffer);
	fflush(stdout);
	close(server_fd);
}

void erro(char *msg)
{
    printf("Erro: %s\n", msg);
    exit(-1);
}

