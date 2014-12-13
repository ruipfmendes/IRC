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
                process_client(client);
                exit(0);
            }
            close(client);
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

/*void verifica_exis(char* nome, char* pass, lis_util original){
	lis_util aux;
	aux = aux->next;

	while(aux != NULL){
		if (strcmp(aux->login,nome) != 0){
			printf("Nao existe esse utilizador \n", );
		}
		else {
			if (strcmp(aux->password,pass) != 0){
				printf("A password introduzida esta errada\n", );
			}
			else{
				return 1
			}
		}
		aux = aux->next;

	}
	return 0;

}*/