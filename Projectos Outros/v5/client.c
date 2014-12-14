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
        return 1;
    }

    if( connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0){
        perror("While connecting");
        return 1;
    }

    autentication(fd);

    nread = read(fd, aux, sizeof(aux) + 1);
    aux[nread]='\0';
    if(atoi(aux) == 1){
        menu(fd);
    } else {
        printf("Wrong username or password\n");
        exit(0);
    }

    //process_servidor(fd);
    //close(fd);
    //exit(0);
    return 0;
}

void menu(int fd){
    int option = 5;
    do{
        printf("Main Menu: \n[1] See inbox\n[2] Compose email\n[3] Open email\n[4] Delete email\n[0] Close\nOption: ");
        scanf("%d", &option);
        getchar();
        switch(option){
            case 0:
                close(fd);
                exit(0);
                break;
            case 1:
                write(fd, "inbox", strlen("inbox") + 1);
                fflush(stdin);
                write(fd, user, strlen(user) + 1);
                fflush(stdin);
                check_inbox_client(fd);
                break;
            case 2:
                write(fd, "cmail", strlen("cmail") + 1);
                fflush(stdin);
                write(fd, user, strlen(user) + 1);
                fflush(stdin);

                send_email_client(fd);
                break;
            case 3:
                write(fd, "omail", strlen("omail") + 1);
                fflush(stdin);
                write(fd, user, strlen(user) + 1);
                fflush(stdin);
                open_email_client(fd);
                break;
            case 4:
                write(fd, "dmail", strlen("dmail") + 1);
                fflush(stdin);
                write(fd, user, strlen(user) + 1);
                fflush(stdin);

                delete_email_client(fd);
                break;
            default:
                printf("Not a valid choice!");
        }

    }while(option < 0 && option > 4);
    	menu(fd);
}

void check_inbox_client(int servidor_fd)
{
    int nread;
    char n_emails[10];
    char buffer[BUF_SIZE];
    int i;

    nread=read(servidor_fd,n_emails,9);
    fflush(stdout);
    buffer[nread]='\0';

    printf("Numero de emails: %d\n", atoi(n_emails));

    for(i=0;i< atoi(n_emails) ;i++)
    {
        nread=read(servidor_fd, buffer, BUF_SIZE-1);
        fflush(stdout);
        buffer[nread]='\0';
        printf("%s\n", buffer);

    }
}

void open_email_client(int servidor_fd)
{
    int nread;
    char choice[10];
    char buffer[4096];
    //int index;

    printf("Email index: ");
    fgets(choice, 10, stdin);
    fflush(stdin);
    write(servidor_fd, choice, strlen(choice)+1);

    nread=read(servidor_fd,buffer,4095);
    fflush(stdout);
    buffer[nread]='\0';

    printf("Email->[%d]: %s\n", atoi(choice), buffer);
}

void delete_email_client(int servidor_fd)
{
    char choice[10];

    printf("Email index: ");
    fgets(choice, 10, stdin);
    fflush(stdin);
    write(servidor_fd, choice, strlen(choice)+1);
}

void send_email_client(int servidor_fd)
{
    char n_receivers[10];
    int i;
    char receiver[61];
    char message[4096];
    char subject[100];

    printf("Numero de Destinatarios: ");
    fgets(n_receivers,10,stdin);
    write(servidor_fd, n_receivers, strlen(n_receivers)+1);
    fflush(stdin);

    printf("Assunto: ");
    fgets(subject, 100, stdin);
    write(servidor_fd, subject, strlen(subject)+1);
    fflush(stdin);

    printf("Mensagem: ");
    fgets(message, 4096, stdin);
    write(servidor_fd, message, strlen(message)+1);
    fflush(stdin);

    for(i=0;i<atoi(n_receivers);i++)
    {
        printf("Destinarios [%d]: ", i+1);
        fgets(receiver, 60,stdin);
        write(servidor_fd, receiver, strlen(receiver)+1);
        fflush(stdin);
    }
}

void autentication(int fd){
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
    fflush(stdin);
}

void process_servidor(int servidor_fp)
{
    int nread = 0;
    char buffer[BUF_SIZE];

    nread = read(servidor_fp,buffer,BUF_SIZE-1);
    
    fflush(stdout);
    buffer[nread]='\0';
    printf("%s\n",buffer);
    fflush(stdout);

    close(servidor_fp);
}
