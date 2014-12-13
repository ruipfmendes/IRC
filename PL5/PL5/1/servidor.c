	
/*******************************************************************
* SERVIDOR no porto 9000, � escuta de novos clientes. Quando surjem
* novos clientes os dados por eles enviados s�o lidos e descarregados no ecran.
*******************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#define SERVER_PORT 9000
#define BUF_SIZE 1024
void process_client(int fd);
void erro(char *msg);
int main() {
    int fd, client;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);
    if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        erro("na funcao socket");
    if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
        erro("na funcao bind");
    if( listen(fd, 5) < 0)
        erro("na funcao listen");
    while (1) {
        client_addr_size = sizeof(client_addr);
        client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
        if (client > 0) {
            if (fork() == 0) {
                close(fd);
                process_client(client);
                exit(0);
            }
            close(client);
        }
    }
    return 0;
}	
void process_client(int client_fd)
{
	int nread = 0;
	char buffer[BUF_SIZE];
	nread = read(client_fd, buffer, BUF_SIZE-1);
	buffer[nread] = '\0';
	printf("%s", buffer);
	int i;
	for(i=0;i<nread;i++){
		if(buffer[i] == 65 || buffer[i] == 97 || buffer[i] == 69 || buffer[i] == 101 || buffer[i] == 73 || buffer[i] == 105 || buffer[i] == 79 || buffer[i] == 111 || buffer[i] == 85 || buffer[i] == 117){
			buffer[i] = ' ';
		}
	}
	fflush(stdout);
	close(client_fd);
}

void erro(char *msg)
{
	printf("Erro: %s\n", msg);
	exit(-1);
}
