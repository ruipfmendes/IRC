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

typedef struct *lis_util;
typedef struct utilizador
{
	char login[50];
	char password[50];
	mensagem msg[100];
	lis_util next; 
}utilizador;


typedef struct mensagem
{
	char *destinatario;
	char *assunto;
	char *corpo_msg
	
}mensagem;

void erro(char *msg);
void process_server(int servidor_fp);