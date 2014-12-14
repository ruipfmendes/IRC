#include "header.h"

void process_client(int client_fd);

int main (int argc, char const *argv[])
{
    int fd, client;
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
    int i=0;
    char buffer[BUF_SIZE];
    nread = read(client_fd, buffer, BUF_SIZE-1);
    buffer[nread] = '\0';
    printf("%s", buffer);

    //Função para tirar vogais
    for(i=0;i<nread;i++){
        if (buffer[i]==97 || buffer[i]==101 || buffer[i]==105 || buffer[i]==111 || buffer[i]==117) //minusculas
            buffer[i]=' ';
        if(buffer[i]== 65||buffer[i]==45|| buffer[i]==49||buffer[i]==79|| buffer[i]==55)	//maiusculas
            buffer[i]=' ';
    }
    write(client_fd,buffer,1+strlen(buffer));

    fflush(stdout);
    close(client_fd);
}
