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
	char sucesso_login[30];
	char comando[30] = "";

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
	sucesso_login[nread-1]=0;
	if(strcmp(sucesso_login,"sucesso") == 0){
		printf("Iniciou a sua sessao com sucesso\n");
		printf("Menu de comandos:\n");
		//printf("ligar");
		printf("enviar - Enviar uma mensagem\n");
		printf("lista - Lista as mensagens do utilizador\n");
		printf("ler - Ler uma mensagem recebida\n");
		printf("apagar - Apagar uma mensagem do servidor\n");
		printf("desligar\n");
	}
	while(strcmp(comando,"desligar\n")!=0){
		fgets(comando,sizeof(comando),stdin);
		write(fd,comando,strlen(comando)+1);
		if(strcmp(comando,"enviar\n")==0){
			//enviar_email(fd);
		}
		else if(strcmp(comando,"lista\n")==0){
			//listar_mensagem(fd);
		}
	}
	
    //write(fd, argv[3], 1 + strlen(argv[3]));
    //process_servidor(fd);
	close(fd);
	exit(0);
}

void login(int fd){
	char utilizador[INFO_SIZE];
	char password[INFO_SIZE];
	
	printf("Utilizador: ");
	fgets(utilizador,sizeof(utilizador),stdin);
	utilizador[strlen(utilizador)-1] = 0; // remover \n
	write(fd,utilizador,strlen(utilizador)+1);
	printf("Password: ");
	//strcpy(password,getpass("Password: "));
	fgets(password,sizeof(password),stdin);
	password[strlen(password)-1] = 0; // remover \n
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
