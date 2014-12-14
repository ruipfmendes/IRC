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
#define INFOLINE_SIZE 50
#define INFO_SIZE 25
#define MSG_SIZE 50
#define LOGIN "login.txt"

typedef struct info_utilizador{
	char utilizador[50];
	char password[50];
//	mensagem msg[100];
//	lis_util next; 
}info_utilizador;

/*typedef struct mensagem
{
	char *destinatario;
	char *assunto;
	char *corpo_msg;
}mensagem;*/

//Cliente.c
void erro(char *msg);
void process_servidor(int servidor_fp);
void login(int fd);
//Servidor.c
void erro(char *msg);
void login_servidor(int fd);
void ler_infos_utilizador();
int comparar_login(char utilizador[], char password[]);
void process_cliente(int client_fd);



info_utilizador infos_ut[100];
int infos_ut_size = 0;
