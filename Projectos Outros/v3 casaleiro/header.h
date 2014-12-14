#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

#define USERS_FILE "users.txt"
#define SERVER_PORT 9000
#define NORMAL_SIZE 100
#define SHORT_STRING 21
#define BUF_SIZE 1024

#define DEBUG //remove this line to remove DEBUG messages

typedef struct{
    char user[60];
    char password[60];
}credencial;

//Server variables
credencial credenciais[NORMAL_SIZE];
int credencial_size = 0;

//Client variables
int port;
char host[SHORT_STRING];

//Server
void autentication_server(int fd);
int verifica(char user[SHORT_STRING], char password[SHORT_STRING]);
void lerFicheiro();

//Client
int create_socket();
void send_request(int fd, char* message);
void autentication(int fd);
void menu(int fd);
