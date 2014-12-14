#include "header.h"

void process_client(int client_fd);

int main (int argc, char const *argv[])
{
    int fd;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);

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

void autentication_server(int fd){
    int nread;
    char username[SHORT_STRING];
    char password[SHORT_STRING];

    nread = read(fd, username, SHORT_STRING-1);
    username[nread-1]='\0';

    nread = read(fd, password, SHORT_STRING-1);
    password[nread-1]='\0';

    if(verifica(username, password)==1){
        write(fd, "1", strlen("1") + 1);
        process_client(fd);
    }
    else{
        write(fd, "0", strlen("0") + 1);
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
    char buffer[BUF_SIZE];
    char user[SHORT_STRING];
    char sender[SHORT_STRING];
    char subject[NORMAL_SIZE];
    char body[BUF_SIZE];
    char index[SHORT_STRING];

    nread = read(client_fd, buffer, BUF_SIZE-1);
    buffer[nread - 1] = '\0';
    printf("option: %s\n", buffer);

    if(strcmp(buffer, "inbox") == 0){
        nread = read(client_fd, user, SHORT_STRING-1);
        user[nread - 1] = '\0';

        check_inbox(user, client_fd);
    } else if(strcmp(buffer, "cmail") == 0){
        //Poderá ter varios users - destinatarios
        nread = read(client_fd, buffer, SHORT_STRING-1);
        buffer[nread - 1] = '\0';
        n_receivers = atoi(buffer);
        //info mail
        nread = read(client_fd, sender, SHORT_STRING-1);
        sender[nread - 1] = '\0';
        nread = read(client_fd, subject, NORMAL_SIZE-1);
        subject[nread - 1] = '\0';
        nread = read(client_fd, body, BUF_SIZE-1);
        body[nread - 1] = '\0';

        for(i = 0; i < n_receivers; i++){
            nread = read(client_fd, user, SHORT_STRING-1);
            user[nread - 1] = '\0';
            inserir_email(sender, user, subject, body, client_fd);
        }
    } else if(strcmp(buffer, "omail") == 0){
        nread = read(client_fd, user, SHORT_STRING-1);
        user[nread - 1] = '\0';
        nread = read(client_fd, index, SHORT_STRING-1);
        index[nread - 1] = '\0';

        open_email(user, atoi(index), client_fd);
    } else if(strcmp(buffer, "dmail") == 0){
        nread = read(client_fd, user, SHORT_STRING-1);
        user[nread - 1] = '\0';
        nread = read(client_fd, index, SHORT_STRING-1);
        index[nread - 1] = '\0';

        delete_email(user, atoi(index), client_fd);
    } else {
        printf("Wrong key word\n");
    }

    write(client_fd,buffer,1+strlen(buffer));

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

            while(j < credenciais[i].n_emails) {
                snprintf(buffer, 4096,"[%d] %s | %s | %s", j, credenciais[i].inbox[j].sender, credenciais[i].inbox[j].subject, credenciais[i].inbox[j].body);
                write(fd, buffer, strlen(buffer) + 1);

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

void inserir_email(char sender[SHORT_STRING], char destiny[SHORT_STRING],  char subject[NORMAL_SIZE],char body[BUF_SIZE], int fd)
{
    int i;

    for(i = 0; i < credencial_size;i++)
    {
        if(strcmp(credenciais[i].user,destiny)==0)
        {
            strcpy(credenciais[i].inbox[credenciais[i].n_emails - 1].sender, sender);
            strcpy(credenciais[i].inbox[credenciais[i].n_emails - 1].subject, subject);
            strcpy(credenciais[i].inbox[credenciais[i].n_emails - 1].body,body);
            credenciais[i].n_emails++;

        }
    }
}
