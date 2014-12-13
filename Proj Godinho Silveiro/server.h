#include <fcntl.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>

#define BUF_SIZE	   1024
#define USER_SIZE    20
#define SERVER_PORT  9005

void process_client(int client_fd);

typedef struct{
    char msg[BUF_SIZE];
} sendbox;



typedef struct{
	char user[60];
	char password[60];
}credencial;



void lerFicheiro();
int verifica(char user[60], char password[60]);
void autenticacao_server(int fd);

struct sockaddr_in addr, client_addr;
int client_addr_size;
credencial credenciais[100];
int shmid;
sendbox *sb, *serverb;
int credencial_size=0;
