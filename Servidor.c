#include "header.h"


int main() {
    int fd, client;
    struct sockaddr_in addr, client_addr;
    int client_addr_size;
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);
    if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        erro("na funcao socket");
    if(bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
        erro("na funcao bind");
    if(listen(fd, 5) < 0)
        erro("na funcao listen");
    while (1) {
        client_addr_size = sizeof(client_addr);
        client = accept(fd,(struct sockaddr *)&client_addr,&client_addr_size);
        if (client > 0) {
            if (fork() == 0) {
                close(fd);
				login_server(client);
                //process_client(client);
            }
            close(client);
        }
    }
    return 0;
}	

void login_servidor(int fd){
    printf("Login\n");
    char utilizador[INFO_SIZE];
    char password[INFO_SIZE];
	char utilizador_notfound[MSG_SIZE] = "Utilizador não existe";
	int nread;
	
	nread = read(fd, username, INFO_SIZE+1);
	username[nread-1]='\0';
    nread = read(fd, password, INFO_SIZE+1);
    password[nread-1]='\0';
    if(comparar_login(username,password)==1){
        process_client(fd);
    }
    else{
		write(fd,utilizador_notfound,strlen(utilizador_notfound)+1);
    }
}
void ler_infos_utilizador(){
	char buffer[INFOLINE_SIZE];
    char utilizador[INFO_SIZE];
    char password[INFO_SIZE];

    int i = 0;
    FILE* fp;
    if((fp=fopen("login.txt","r")))
    {
        while(fgets(buffer,INFOLINE_SIZE, fp))
        {
            sscanf(buffer,"%s %s",utilizador,password);
            strcpy(infos_ut[i].user,utilizador);
            strcpy(infos_ut[i].password,password);
            printf("Utilizador %d -> %s -> %s\n", i, infos_ut[i].utilizador,infos_ut[i].password);
            i++;
            infos_ut_size++;
        }
    }
    fclose(fp);
}
void comparar_login(){
    printf("Utilizador: %s; Password: %s", utilizador,password);
    int i =0;
	for(i = 0;i<infos_ut_size;i++){
        if(strcmp(utilizador,infos_ut[i].utilizador)==0 && strcmp(password,infos_ut[i].password)==0){
            return 1;
		}
		else if(strcmp(user,credenciais[i].utilizador)==0){
			printf("Password errada");
			return 0;
		}
	}
    return 0;
}
void process_client(int client_fd)
{
	int nread = 0;
	char buffer[BUF_SIZE];
	nread = read(client_fd, buffer, BUF_SIZE-1);
	buffer[nread] = '\0';
	printf("%s", buffer);
	write(client_fd,buffer,1+strlen(buffer));
	fflush(stdout);
	close(client_fd);
}