#include "header.h"

void erro(char *msg){
	printf("Erro: %s\n",msg);
	exit(-1);
}

int main() {
	int fd, client;
	struct sockaddr_in addr, client_addr;
	int client_addr_size;
	bzero((void *) &addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(SERVER_PORT);
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		erro("na funcao socket");
	}
	if(bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0){
		erro("na funcao bind");
	}
	if(listen(fd,5) < 0){
		erro("na funcao listen");
	}
	ler_infos_utilizador();
	while (1) {
		client_addr_size = sizeof(client_addr);
		client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
		if (client > 0) {
			if (fork() == 0) {
				close(fd);
				login_servidor(client);
			}
		close(client);
		}
	}
	return 0;
}	

void login_servidor(int fd){
	printf("Utilizador está a tentar fazer login\n");
	char utilizador[INFO_SIZE];
	char password[INFO_SIZE];
	int nread;
	
	nread = read(fd,utilizador,INFO_SIZE+1);
	utilizador[nread]='\0';
	nread = read(fd,password,INFO_SIZE+1);
	password[nread]='\0';
	if(comparar_login(utilizador,password)==1){
		write(fd,"sucesso",strlen("sucesso")+1);
		process_client(fd);
	}
	else{
		if(comparar_login(utilizador,password)==0){
			write(fd,"password",strlen("password")+1);
		}
		else if(comparar_login(utilizador,password)==0){
			write(fd,"utilizador",strlen("utilizador")+1);
		}
	}
}



void ler_infos_utilizador(){
	char buffer[INFOLINE_SIZE];
	char utilizador[INFO_SIZE];
	char password[INFO_SIZE];

	int i = 0;
	FILE* fp;
	if((fp=fopen("login.txt","r"))){
		while(fgets(buffer,INFOLINE_SIZE, fp)){
			sscanf(buffer,"%s %s",utilizador,password);
			strcpy(infos_ut[i].utilizador,utilizador);
			strcpy(infos_ut[i].password,password);
			printf("Utilizador %d: %s; Password: %s\n", i, infos_ut[i].utilizador,infos_ut[i].password);
			i++;
			infos_ut_size++;
		}
	}
	fclose(fp);
}

int comparar_login(char utilizador[INFO_SIZE], char password[INFO_SIZE]){
	printf("Servidor recebeu:\nUtilizador: %s; Password: %s;\n",utilizador,password);
	int i;
	for(i=0;i<infos_ut_size;i++){
		if(strcmp(utilizador,infos_ut[i].utilizador)==0 && strcmp(password,infos_ut[i].password)==0){
			printf("%s fez log-in com sucesso.\n",utilizador);
			return 1;
		}
		else if(strcmp(utilizador,infos_ut[i].utilizador)==0){
			return 0;
		}
	}
	return -1;
}

/*void guardar_emails(){
	FILE *fp;
	char email_info[BUF_SIZE];
	int i,j;
	fp=fopen(MAIL_FILE,"a");
	for(i=0;i<infos_ut_size;i++){
		for(j=0;j<infos_ut[i].num_emails;j++){
			fprintf("%d %d %s %s %s %s",i, j, infos_ut[i].utilizador, infos_ut[i].emails[j].remetente, infos_ut[i].emails[j].assunto, infos_ut[i].emails[j].texto);
		}
	}
	fclose(fp);
	exit(0);
}*/

void ler_emails(){
	FILE *fp;
	char linha[BUF_SIZE];
	char remetente[INFO_SIZE];
	char utilizador[INFO_SIZE];
	char assunto[MSG_SIZE];
	char texto[BUF_SIZE];
	fp=fopen(MAIL_FILE,"r");
	int i,j;
	while(fgets(linha,BUF_SIZE,fp) != NULL){
		sscanf(linha,"%d %d %s %s %s %s", &i, &j, utilizador, remetente, assunto, texto);
		strcpy(infos_ut[i].utilizador,utilizador);
		strcpy(infos_ut[i].emails[j].remetente,remetente);
		strcpy(infos_ut[i].emails[j].assunto,assunto);
		strcpy(infos_ut[i].emails[j].texto,texto);
	}
}

void process_client(int client_fd)
{
	int nread = 0;
	char buffer[BUF_SIZE];
	nread = read(client_fd, buffer, BUF_SIZE-1);
	buffer[nread] = '\0';
	int num_destinatarios,i,j,index;
	char cindex[INFO_SIZE];
	char cnum_destinatarios[INFO_SIZE];
	char destinatario[INFO_SIZE];
	char assunto[INFO_SIZE];
	char texto[INFO_SIZE];
	char num_emails[INFO_SIZE];
	if(strcmp(buffer,"enviar")==0){
		printf("OI");
		//utilizador
		nread = read(client_fd,utilizador,INFO_SIZE);
		fflush(stdout);
		utilizador[nread] = '\0';
		printf("Utilizador: %s\n",utilizador);
		//num_destinatarios
		nread = read(client_fd,cnum_destinatarios,INFO_SIZE);
		fflush(stdout);
		cnum_destinatarios[nread] = '\0';
		num_destinatarios = atoi(cnum_destinatarios);
		printf("Numero de destinatarios: %d\n",num_destinatarios);
		//assunto
		nread = read(client_fd,assunto,INFO_SIZE);
		fflush(stdout);
		assunto[nread] = '\0';
		printf("Assunto: %s\n",assunto);
		//texto
		nread = read(client_fd,texto,BUF_SIZE);
		fflush(stdout);
		texto[nread] = '\0';
		printf("Texto: %s\n",texto);
		for(i=0;i<infos_ut_size;i++){
			nread = read(client_fd,destinatario,INFO_SIZE);
			fflush(stdout);
			destinatario[nread] = '\0';
			adicionar_emails_ut(client_fd, destinatario, utilizador, assunto, texto);
		}
	}
	else if(strcmp(buffer,"listar") == 0){
		nread = read(client_fd,utilizador,BUF_SIZE-1);
		utilizador[nread] = '\0';
		for(i=0;i<infos_ut_size;i++)
		{
			if(strcmp(utilizador,infos_ut[i].emails->remetente)==0){
				//sprintf(num_emails,"%d",infos_ut[i].num_emails);
				snprintf(num_emails,strlen(num_emails), "%d", infos_ut[i].num_emails);
				write(client_fd, num_emails, strlen(num_emails)+1);
				fflush(stdin);
				for(j=0;j<infos_ut[i].num_emails;j++) {
					snprintf(buffer,BUF_SIZE,"Email nº %d - Remetente: %s - Assunto: %s - Texto: %s", j, infos_ut[i].emails[j].remetente,infos_ut[i].emails[j].assunto, infos_ut[i].emails[j].texto);
					write(client_fd, buffer, strlen(buffer)+1);
					//fflush(stdin);
		    		}
			}
	    	}
	}
	else if(strcmp(buffer,"ler")==0){
		nread = read(client_fd,utilizador,BUF_SIZE-1);
		//fflush(stdout);
		utilizador[nread] = '\0';
		nread = read(client_fd,cindex,BUF_SIZE-1);
		//fflush(stdout);
		cindex[nread] = '\0';
		index = atoi(cindex);
		for(i=0;i<infos_ut_size;i++){
			if(strcmp(infos_ut[i].utilizador,utilizador)==0){
				if(index<infos_ut[i].num_emails){
					snprintf(buffer,BUF_SIZE,"Email nº %d:\n Remetente: %s -- Assunto: %s\n%s", index, infos_ut[index].emails[index].remetente,infos_ut[i].emails[index].assunto, infos_ut[i].emails[index].texto);
					write(client_fd,buffer,strlen(buffer)-1);
				}
			}
		}
	}
	else if(strcmp(buffer,"apagar")==0){
		nread = read(client_fd,utilizador,BUF_SIZE-1);
		fflush(stdout);
		utilizador[nread] = '\0';
		nread = read(client_fd,cindex,BUF_SIZE-1);
		fflush(stdout);
		cindex[nread] = '\0';
		index = atoi(cindex);
		for(i=0;i<infos_ut_size;i++){
			if(strcmp(infos_ut[i].utilizador,utilizador)==0){
				for(i=index;i<infos_ut[i].num_emails-1;i++){
					infos_ut[i].emails[index] = infos_ut[i].emails[index+1];
				}
				infos_ut[i].num_emails--;
			}
		}
	}
	//printf("%s", buffer);
	//write(client_fd,buffer,1+strlen(buffer));
	fflush(stdout);
	close(client_fd);
}

void adicionar_emails_ut(int fd, char destinatario[INFO_SIZE], char utilizador[INFO_SIZE], char assunto[INFO_SIZE], char texto[BUF_SIZE]){
	int i;
	if(strcmp(infos_ut[i].utilizador,destinatario)==0){
		strcpy(infos_ut[i].emails[infos_ut[i].num_emails-1].remetente,utilizador);
		strcpy(infos_ut[i].emails[infos_ut[i].num_emails-1].assunto,assunto);
		strcpy(infos_ut[i].emails[infos_ut[i].num_emails-1].texto,texto);
		infos_ut[i].num_emails++;
	}
}
