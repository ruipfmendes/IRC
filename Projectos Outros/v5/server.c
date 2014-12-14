#include "header.h"

void process_client(int client_fd);
void second_server_connection();

int main (int argc, char const *argv[])
{
    int fd;
    FILE *fp;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);

    signal(SIGINT, catch_ctrlc);
    //getMailFile();
    lerFicheiro();


    if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("na funcao socket");
    }
    if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0){
        perror("na funcao bind");
    }
    if( listen(fd, 5) < 0){
        perror("na funcao listen");
    }
    //second_server_connection();
    while (1) {
        client_addr_size = sizeof(client_addr);
        client = accept(fd,(struct sockaddr *)&client_addr,(socklen_t*)&client_addr_size);
        if (client > 0) {
            if(fork() == 0){
                close(fd);
                autentication_server(client);

            }
            close(client);
        }
    }
    return 0;
}
/*
void second_server_connection()
{
    int sd;
    FILE *fp;
    int second_port=8000;
    struct sockaddr_in addr, second_addr;
    int second_addr_size;

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(second_port);


    if ( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("na funcao socket");
    }
    if ( bind(sd,(struct sockaddr*)&addr,sizeof(addr)) < 0){
        perror("na funcao bind");
    }
    if( listen(sd, 5) < 0){
        perror("na funcao listen");
    }  
    while (1) {
            second_addr_size = sizeof(second_addr);
            second = accept(sd,(struct sockaddr *)&second_addr,(socklen_t*)&second_addr_size);
            if (second > 0) {
                if(fork() == 0){
                        printf("communication estabilished\n");
                }
                close(second);
            }
        }
    }*/   

void autentication_server(int fd){
    int nread;
    char username[SHORT_STRING];
    char password[SHORT_STRING];

    nread = read(fd, username, SHORT_STRING-1);
    fflush(stdout);
    username[nread]='\0';

    nread = read(fd, password, SHORT_STRING-1);
    fflush(stdout);
    password[nread]='\0';

    if(verifica(username, password)==1){
        write(fd, "1", strlen("1") + 1);
        fflush(stdin);
        process_client(fd);
    }
    else{
        write(fd, "0", strlen("0") + 1);
        fflush(stdin);
    }
}

int verifica(char user[SHORT_STRING], char password[SHORT_STRING])
{
    int i =0;

    while(i < credencial_size)
    {
        if(strcmp(user, credenciais[i].user)==0 && strcmp(password, credenciais[i].password)==0){
            return 1;
        }
        i++;
    }
    return 0;
}

void catch_ctrlc()
{
    FILE *fp;
    int i=0;
    int j=0;
    char buffer[4096];
    char ficheiro[]="mail_struct.txt";
    
    printf("catch_ctrlc\n");
    fp=fopen(ficheiro, "a");
    while(i<credencial_size)
    {
        while(j<credenciais[i].n_emails)
        {
            sprintf(buffer, "%d %d %s %s %s %s",i,j,credenciais[i].user, credenciais[i].inbox[j].sender, credenciais[i].inbox[j].subject , credenciais[i].inbox[j].body);
            fprintf(fp, "%s", buffer); 
            j++;
        }    
        i++;
    }
    fclose(fp);
    exit(0);

}

void getMailFile()
{
    FILE * fp;
    int i;
    int j;
    char username[SHORT_STRING];
    char sender[SHORT_STRING];
    char subject[NORMAL_SIZE];
    char body[BUF_SIZE];
    char buffer[6000];
    char ficheiro[]="mail_struct.txt";
    fp = fopen(ficheiro, "r");
    
    while(fgets(buffer, 6000, fp)!=NULL)
    {

        sscanf(buffer, "%d %d %s %s %s %s", &i,&j, username,sender, subject, body);
        //printf("%d %d %s %s %s %s", &i,&j, username,sender, subject, body);
        strcpy(credenciais[i].user, username);
        strcpy(credenciais[i].inbox[j].sender,sender);
        strcpy(credenciais[i].inbox[j].subject,subject);
        strcpy(credenciais[i].inbox[j].body,body);
    }
}


void lerFicheiro()
{
    char buf[120];
    char tuser[60];
    char tPassword[60];

    int i = 0;
    FILE * fp;

    if((fp=fopen("users.txt","r")))
    {
        while(fgets(buf, 120, fp))
        {
            sscanf(buf, "%s %s", tuser , tPassword);
            strcpy(credenciais[i].user, tuser);
            strcpy(credenciais[i].password, tPassword);
            printf("user[%d]->%s -> %s\n", i, credenciais[i].user,credenciais[i].password);

            i++;
            credencial_size++;
        }
    }
    fclose(fp);
}

void process_client(int client_fd)
{
    int nread = 0;
    int i;
    int n_receivers;
    char times [SHORT_STRING];
    char buffer[BUF_SIZE];
    char user[SHORT_STRING];
    char sender[SHORT_STRING];
    char subject[NORMAL_SIZE];
    char body[BUF_SIZE];
    char index[SHORT_STRING];
    char receiver[SHORT_STRING];

    char waste[100];

    nread = read(client_fd, buffer, BUF_SIZE-1);
    fflush(stdout);
    buffer[nread] = '\0';

    if(strcmp(buffer, "inbox") == 0){
        nread = read(client_fd, user, SHORT_STRING-1);
        fflush(stdout);
        user[nread] = '\0';
        check_inbox(user, client_fd);

    } else if(strcmp(buffer, "cmail") == 0){
        printf("ENTROU NA FUNÇAO CMAIL\n");

        nread = read(client_fd, user, SHORT_STRING-1);
        fflush(stdout);
        user[nread] = '\0';
        printf("user-> %s\n", user);

        nread=read(client_fd, times,SHORT_STRING-1);
        fflush(stdout);
        times[nread-1]='\0';
        n_receivers = atoi(times);
        printf("n_receivers->%d\n", n_receivers);

        nread=read(client_fd,subject, NORMAL_SIZE-1);
        fflush(stdout);
        subject[nread]=0;
        printf("SUBJECT-> %s\n", subject);

        nread=read(client_fd,body, BUF_SIZE-1);
        fflush(stdout);
        body[nread]=0;
        printf("BODY-> %s\n", body);

        for(i = 0; i < n_receivers; i++){
            nread = read(client_fd, receiver, SHORT_STRING-1);
            fflush(stdout);
            receiver[nread] = '\0';
            printf("user->%s subject->%s body->%s receiver-> %s\n", user, subject, body, receiver);
            inserir_email(user, receiver, subject, body, client_fd);
        }
    } else if(strcmp(buffer, "omail") == 0){
        nread = read(client_fd, user, SHORT_STRING-1);
        fflush(stdout);
        user[nread] = '\0';

        nread = read(client_fd, index, SHORT_STRING-1);
        fflush(stdout);
        index[nread] = '\0';

        open_email(user, atoi(index), client_fd);
    } else if(strcmp(buffer, "dmail") == 0){
        nread = read(client_fd, user, SHORT_STRING-1);
        fflush(stdout);
        user[nread] = '\0';
        nread = read(client_fd, index, SHORT_STRING-1);
        fflush(stdout);
        index[nread] = '\0';

        delete_email(user, atoi(index), client_fd);
    } else {
        printf("Wrong key word\n");
    }

    /*write(client_fd,buffer,1+strlen(buffer));
    fflush(stdin);*/


    fflush(stdout);
    close(client_fd);
}

void check_inbox(char *username, int fd)
{
    int i;
    int j=0;
    char n_emails[2];
    char buffer[4096];

    for(i = 0;i<credencial_size;i++)
    {
        if(strcmp(username, credenciais[i].inbox->sender)==0)
        {
            sprintf(n_emails, "%d", credenciais[i].n_emails);
            write(fd, n_emails, strlen(n_emails)+1);
            fflush(stdin);

            while(j < credenciais[i].n_emails) {
                snprintf(buffer, 4096,"[%d] %s | %s | %s", j, credenciais[i].inbox[j].sender, credenciais[i].inbox[j].subject, credenciais[i].inbox[j].body);
                write(fd, buffer, strlen(buffer) + 1);
                fflush(stdin);

                j++;
            }
        }
    }
}

void open_email(char username[SHORT_STRING],int index, int fd)
{
    int i;
    char buffer[4096];

    for(i=0;i<credencial_size;i++)
    {
        if(strcmp(credenciais[i].user, username)==0)
        {
            if(index<credenciais[i].n_emails)
            {
                snprintf(buffer, 4096, "%s | %s | %s", credenciais[i].inbox[index].sender,credenciais[i].inbox[index].subject,credenciais[i].inbox[index].body);
                write(fd,buffer, strlen(buffer)+1);
                fflush(stdin);
            }
        }

    }
}

void delete_email(char username[SHORT_STRING], int index, int fd)
{
    int i;
    int j=0;

    for(i=0;i<credencial_size;i++)
    {
        if(strcmp(credenciais[i].user,username)==0)
        {
            for(j=index;j<credenciais[i].n_emails-1;j++)
            {
                credenciais[i].inbox[index]=credenciais[i].inbox[index+1];
            }
            //credenciais[i].inbox[(credenciais[i].n_emails)- 1]=NULL;
            credenciais[i].n_emails--;
            break;
        }
    }
}

void inserir_email(char sender[SHORT_STRING], char receiver[SHORT_STRING],  char subject[NORMAL_SIZE],char body[BUF_SIZE], int fd)
{
    int i;

    for(i = 0; i < credencial_size;i++)
    {
        if(strcmp(credenciais[i].user,receiver)==0)
        {
            strcpy(credenciais[i].inbox[credenciais[i].n_emails - 1].sender, sender);
            strcpy(credenciais[i].inbox[credenciais[i].n_emails - 1].subject, subject);
            strcpy(credenciais[i].inbox[credenciais[i].n_emails - 1].body,body);
            credenciais[i].n_emails++;

        }
    }
}
