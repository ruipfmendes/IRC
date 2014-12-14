#include "header.h"

#define BUF_SIZE 1024

void process_servidor(int servidor_fp);

int main (int argc, char const *argv[])
{
    int fd;
    struct hostent *hostPtr;
    int nread;
    char aux[SHORT_STRING];
    struct sockaddr_in addr, serv_addr;
    int servidor;
    int serv_addr_size;
    if (argc != 3) {
        printf("cliente <host> <port>\n");
        exit(-1);
    } else {
        strcpy(host, argv[1]);
        port = atoi(argv[2]);
    }


    if ((hostPtr = gethostbyname(host)) == 0){
        perror("While getting host name");
    }

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));

    if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("While creating socket");
    }

    if( connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0){
        perror("While connecting");
    }

    autentication(fd);

    nread = read(fd, aux, sizeof(aux) + 1);
    aux[nread - 1] = 0;

    if(atoi(aux) == 1){
        menu(fd);
    } else {
        printf("Wrong username or password\n");
        exit(0);
    }

    process_servidor(fd);
    close(fd);
    exit(0);
    return 0;
}

void autentication(int fd){
    char user[SHORT_STRING];
    char password[SHORT_STRING];

    printf("Utilizador: ");
    fgets(user, sizeof(user), stdin);
    user[strlen(user) -1] = 0;
    send_request(fd, user);
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    //strcpy(password, getpass("Password: "));
    password[strlen(password) - 1] = 0;
    send_request(fd, password);
}

void send_request(int fd, char* message){
    write(fd, message, strlen(message) + 1);
}

void process_servidor(int servidor_fp)
{
    int nread = 0;
    char buffer[BUF_SIZE];

    nread = read(servidor_fp,buffer,BUF_SIZE-1);
    buffer[nread]='\0';
    printf("%s\n",buffer);
    fflush(stdout);

    close(servidor_fp);
}
