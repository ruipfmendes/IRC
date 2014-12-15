#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#define SERVER_PORT 9000
#define BUF_SIZE 1024
#define INFOLINE_SIZE 50
#define INFO_SIZE 25
#define MSG_SIZE 50
#define LOGIN "login.txt"
#define MAIL_FILE "emails.txt"

typedef struct
{
	char remetente[INFO_SIZE];
	char assunto[INFO_SIZE];
	char texto[BUF_SIZE];
}email;


typedef struct info_utilizador
{
	char utilizador[INFO_SIZE];
	char password[INFO_SIZE];
	email emails[BUF_SIZE];
	int num_emails;
}info_utilizador;

//Cliente.c
void erro(char *msg);
void process_servidor(int servidor_fp);
void login(int fd);
void enviar_email(int fd);
void listar_emails(int fd);
void apagar_email(int fd);
void ler_email(int fd);


//Servidor.c
void erro(char *msg);
void login_servidor(int fd);
void ler_infos_utilizador();
int comparar_login(char utilizador[], char password[]);
void guardar_emails();
void ler_emails();
void adicionar_emails_ut(int fd, char destinatario[INFO_SIZE], char utilizador[INFO_SIZE], char assunto[INFO_SIZE], char texto[BUF_SIZE]);
void process_client(int client_fd);

//Variáveis
char utilizador[INFO_SIZE];
info_utilizador infos_ut[100];
int infos_ut_size = 0;
