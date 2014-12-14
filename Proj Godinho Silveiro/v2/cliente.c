

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define PORT 9000

void enviar_mail(int fd);
void autenticacao(int fd);

int fd;



void erro(char *msg)
{
    printf("Erro: %s\n", msg);
    exit(-1);
}

int main() {
    char endServer[100];
    char msg[1024];
    struct sockaddr_in addr, client_addr;
    int client_addr_size;
    struct hostent *hostPtr;
    char aux[1000];


    strcpy(endServer, "localhost");
    if ((hostPtr = gethostbyname(endServer)) == 0) {
        printf("Couldn't get host address.\n");
        exit(-1);
    }

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) PORT);

    if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1) erro("socket");
    if( connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0) erro("Connect");

    int sai=0;
    pid_t id;

    //autenticaçao->espera pela entrega do servidor
    autenticacao(fd);
    int nread;
    nread = read(fd, aux, sizeof(aux) +1);
    printf("Res:%s", aux);
    aux[nread-1]=0;
    if(strcmp(aux, "1") == 0){
        printf("------------------------\n");
        printf("Esta ligado ao servidor!\n");
        printf("------------------------\n");
        printf(" .connect --------------\n");
        printf(" .send -----------------\n");
        printf(" .list------------------\n");
        printf(" .read----------------\n");
        printf(" .clear---------------\n");
        printf("  .quit ----------------\n");
        printf("------------------------\n");

        do{
            fgets(msg,sizeof(msg),stdin);
            write(fd, msg, 1 + strlen(msg));

            if(strcmp(msg,".send\n")==0)
                enviar_mail(fd);

            else if (strcmp(msg, ".list\n")==0)
                //listar_mensagem(fd);

            if(strcmp(msg,".quit\n")==0) sai = 1;

        }while(sai==0);

        printf("------------------\n");
        printf("Esta desconectado!\n");
        printf("------------------\n");

        kill(id,SIGKILL); // mata o reciver

        close(fd);
        }
    else{
        printf("Desligar\n");
        exit(0);
    }

    
    
}
void enviar_mail(int fd){
    //user, destinatario, assunto, mensagem
    int num_dest;
    char msg[1024];
    fgets(msg,sizeof(msg),stdin);
    write(fd, msg, 1 + strlen(msg));
}

void autenticacao(int fd)
{
    char utilizador[30];
    char password[30];
    //int nread;
    //char verificacao[10];

    printf("Utilizador: ");
    fgets(utilizador, sizeof(utilizador), stdin);
    utilizador[strlen(utilizador) - 1] = 0;
    write(fd, utilizador, 1+strlen(utilizador));
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strlen(password) - 1] = 0;
    write(fd, password, 1+strlen(password));

}