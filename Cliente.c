#include "header.h"

void erro(char *msg){
	printf("Erro: %s\n",msg);
	exit(-1);
}

int main(int argc, char const *argv[]){
    char endServer[100];
    int fd;
    struct hostent *hostPtr;
	struct sockaddr_in addr, serv_addr;
	char sucesso_login[MSG_SIZE];
	char comando[MSG_SIZE] = "";

    if(argc != 3) {
        printf("cliente <host> <port>\n");
        exit(-1);
    }
    strcpy(endServer, argv[1]);
    if((hostPtr = gethostbyname(endServer)) == 0){
        erro("Nao consegui obter endereço");
	}
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr->h_addr))->s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));
    if((fd = socket(AF_INET,SOCK_STREAM,0)) == -1){
        erro("socket");
	}
    if(connect(fd,(struct sockaddr *)&addr,sizeof (addr)) < 0){
		erro("Connect");
	}
	
	login(fd);
	int nread;
	nread = read(fd,sucesso_login,sizeof(sucesso_login)+1);
	sucesso_login[nread]=0;
	if(strcmp(sucesso_login,"sucesso") == 0){
		printf("Iniciou a sua sessão com sucesso\n");
		printf("Menu de comandos:\n");
		printf("enviar - Enviar uma mensagem\n");
		printf("listar - Listar e-mails\n");
		printf("ler - Ler e-mail recebido\n");
		printf("apagar - Apagar um e-mail\n");
		printf("desligar - Desligar sessão\n");
	}
	else if(strcmp(sucesso_login,"password") == 0){
		printf("Password incorrecta.");
	}
	else if(strcmp(sucesso_login,"utilizador") == 0){
		printf("Utilizador não existe.");
	}
	while(strcmp(comando,"desligar\n")!=0){
		fgets(comando,sizeof(comando),stdin);
		write(fd,comando,strlen(comando)+1);
		if(strcmp(comando,"enviar\n")==0){
			write(fd,comando,strlen(comando)+1);
			write(fd,utilizador, strlen(utilizador) + 1);
			enviar_email(fd);
		}
		else if(strcmp(comando,"listar\n")==0){
			write(fd,comando,strlen(comando)+1);
			write(fd,utilizador,strlen(utilizador) + 1);
			listar_emails(fd);
		}
		else if(strcmp(comando,"ler\n")==0){
			write(fd,comando,strlen(comando)+1);
			write(fd,utilizador,strlen(utilizador) + 1);
			ler_email(fd);
		}
		else if(strcmp(comando,"apagar\n")==0){
			write(fd,comando,strlen(comando)+1);
			write(fd,utilizador,strlen(utilizador) + 1);
			apagar_email(fd);
		}
	}
	close(fd);
	exit(0);
}

void enviar_email(int fd)
{
    char num_destinatarios[INFO_SIZE];
    int i;
    char destinatario[INFO_SIZE];
    char assunto[MSG_SIZE];
    char texto[BUF_SIZE];
    printf("Numero de Destinatarios: ");
    fgets(num_destinatarios,INFO_SIZE,stdin);
	printf("%s",num_destinatarios);
	num_destinatarios[strlen(num_destinatarios] = '\0';
    write(fd,num_destinatarios,strlen(num_destinatarios)+1);
	fflush(stdin);
    printf("Assunto: ");
    fgets(assunto,MSG_SIZE, stdin);
    write(fd,assunto,strlen(assunto)+1);
    printf("Texto: ");
    fgets(texto,BUF_SIZE, stdin);
    write(fd,texto,strlen(texto)+1);
    for(i=0;i<atoi(num_destinatarios);i++)
    {
        printf("Indique o destinatario %d", i+1);
        fgets(destinatario,INFO_SIZE,stdin);
        write(fd,destinatario,strlen(destinatario)+1);
    }
}

void listar_emails(int fd){
	char num_emails[MSG_SIZE];
	char buffer[BUF_SIZE];
	int nread;
	int i;
	nread = read(fd,num_emails,strlen(num_emails)-1);
	buffer[nread] = '\0';
	for(i=0;i<atoi(num_emails);i++){
		nread = read(fd,buffer,strlen(buffer)-1);
		buffer[nread]='\0';
		printf("%s\n",buffer);
	}
}

void apagar_email(int fd){
    char index[MSG_SIZE];

    printf("Index do email a apagar: ");
    fgets(index, MSG_SIZE, stdin);
    write(fd, index, strlen(index)+1);
}

void ler_email(int fd){
	char index[MSG_SIZE];
	char buffer[BUF_SIZE];
	int nread;
	printf("Index do email a ler: ");
	fgets(index,MSG_SIZE,stdin);
	write(fd,index,strlen(index)+1);
	nread=read(fd,buffer,strlen(buffer)+1);
	buffer[nread]='\0';
	printf("Email %d:\n%s",atoi(index),buffer);
}

void login(int fd){
	char password[INFO_SIZE];
	
	printf("Utilizador: ");
	fgets(utilizador,sizeof(utilizador),stdin);
	utilizador[strlen(utilizador)-1] = 0;
	write(fd,utilizador,strlen(utilizador)+1);
	
	printf("Password: ");
	fgets(password,sizeof(password),stdin);
	password[strlen(password)-1] = 0;
	write(fd,password,strlen(password)+1);
}

void process_servidor(int server_fd)
{
	int nread = 0;
	char buffer[BUF_SIZE];
	nread = read(server_fd,buffer,BUF_SIZE-1);
	buffer[nread]='\0';
	printf("%s\n",buffer);
	fflush(stdout);
	close(server_fd);
}
