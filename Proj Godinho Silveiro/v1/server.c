#include "server.h"


void erro(char *msg)
{
    printf("Erro: %s\n", msg);
    exit(-1);
}


int main(){

    char endServer[100];
    struct hostent *hostPtr;
    int fd, client;

    char aux[10];

    shmid = shmget(IPC_PRIVATE, sizeof(sendbox), IPC_CREAT|0700);
    sb = (sendbox*) shmat(shmid, NULL, 0);
    shmid = shmget(IPC_PRIVATE, sizeof(sendbox), IPC_CREAT|0700);
    serverb = (sendbox*) shmat(shmid, NULL, 0);

    lerFicheiro();

    // ------------------- LIGACAO DO SERVIDOR LOCAL
      bzero((void *) &addr, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(SERVER_PORT);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) erro("na funcao socket");
 
    /*int optval = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
        fprintf(stderr, "Error occurred.\n");
        exit(1);
    }*/


    if (bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0) erro("na funcao bind");
    if (listen(fd, 5) < 0) erro("na funcao listen");

    printf("Servidor Local\n");
    while (1) {
        client_addr_size = sizeof(client_addr);
        client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
        if (client > 0) {
            if (fork() == 0) {  // cria fork para "cuidar" do cliente
                close(fd);
                autenticacao_server(client);

            }
            close(client);
        }
    }
    return 0;
}

void lerFicheiro() { 
    char buf[120];
    char tuser[60];
    char tPassword[60];

    int i = 0;
    FILE * fp;

    if((fp=fopen("credenciais.txt","r")))
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

int verifica(char user[60], char password[60])
{
    printf("username: %s pass: %s\n", user, password);
    int i =0;

    while(i<credencial_size)
    {
        if(strcmp(user,credenciais[i].user)==0 && strcmp(password, credenciais[i].password)==0)
            return 1;
        i++;
    }
    return 0;
}

void autenticacao_server(int fd)
{
    printf("autenticacao_server\n");
    int nread;
    char username[30];
    char password[30];
    nread = read(fd, username, 31);
    username[nread-1]='\0';

    nread = read(fd, password, 31);
    password[nread-1]='\0';

    if(verifica(username, password)==1){
        process_client(fd);
    }
    else{
        write(fd, "User não encontrado\n", strlen("User não encontrado\n")+1);
    
    }
}



void process_client(int client_fd)  // responsavel por receber as msg dos clientes
{
    puts("Cliente ligado!");
    int sai = 0, nread=0;
    pid_t id;
    char buffer[BUF_SIZE],msg[BUF_SIZE], teste[10],username[BUF_SIZE],tempname[BUF_SIZE],aux[1024];
    char * ip = malloc(client_addr_size);
    int port = client_addr.sin_port;
    inet_ntop(AF_INET,&client_addr.sin_addr,ip,client_addr_size);  // obter o IP do cliente


    read(client_fd, username, USER_SIZE-1);  // username

    sprintf(msg,"[SERVER] NewCon:%s:%i\n[SERVER] O user <%s> entrou :)\n",ip,port,username);
    strcpy(sb->msg,msg);

    do{

        nread = read(client_fd, buffer, BUF_SIZE-1);
        buffer[nread] = '\0';
        memcpy(teste,&buffer,5);
        teste[5] = '\0';

        if(strcmp(teste,".quit")==0)
        {
            break;
        }
        else if(strcmp(teste,".send")==0)
        {
            nread = read(client_fd, buffer, BUF_SIZE-1);
            buffer[nread] = '\0';
            memcpy(aux,&buffer,1024);
            //teste[5] = '\0';
            //enviar_mail();
            memcpy(tempname,&buffer[0],strlen(buffer));
            tempname[strlen(buffer)-1] = '\0';
            sprintf(msg,"[SERVER] <%s> mudou o nome para <%s>\n",username,tempname);
            strcpy(sb->msg,msg);
            strcpy(username,tempname);
        }

        else if(nread>0)
        {
            sprintf(msg,"[%s] %s",username,buffer);
            strcpy(sb->msg,msg);
        }

        puts("\t\tA enviar para local server...");

        }while(1);

        kill(id,SIGKILL);  // mata o sender

        puts("\tSender destruido!");

        sprintf(msg,"[SERVER] O user <%s> saiu :(\n",username);
        strcpy(sb->msg,msg);

        fflush(stdout);
        close(client_fd);

        puts("Cliente desligado!");
}
