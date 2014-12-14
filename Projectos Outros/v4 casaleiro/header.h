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

#define USERS_FILE "users.txt"
#define SERVER_PORT 9000
#define NORMAL_SIZE 100
#define SHORT_STRING 21
#define N_THREADS 10
#define BUF_SIZE 1024

#define DEBUG //remove this line to remove DEBUG messages

typedef struct{
    char sender[SHORT_STRING];
    char subject[NORMAL_SIZE];
    char body[BUF_SIZE];
} email;

typedef struct{
    char user[SHORT_STRING];
    char password[SHORT_STRING];
    email inbox[BUF_SIZE];
    int n_emails;
}credencial;

//Server variables
credencial credenciais[NORMAL_SIZE];
int credencial_size = 0;
int client;


//Client variables
int port;
char host[SHORT_STRING];
char user[SHORT_STRING];

//Server
void autentication_server(int fd);
int verifica(char user[SHORT_STRING], char password[SHORT_STRING]);
void lerFicheiro();
void check_inbox(char *username, int fd);
void open_email(char username[SHORT_STRING],int index, int fd);
void delete_email(char username[SHORT_STRING], int index, int fd);
void inserir_email(char sender[SHORT_STRING], char destiny[SHORT_STRING],  char subject[NORMAL_SIZE],char body[BUF_SIZE], int fd);
void send_email_client(int servidor_fd);
void* worker(void* idp);

//Client
int create_socket();
void send_request(int fd, char* message);
void autentication(int fd);
void menu(int fd);
void check_inbox_client(int servidor_fp);
void open_email_client(int servidor_fd);
void delete_email_client(int servidor_fd);
