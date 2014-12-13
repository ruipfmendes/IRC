#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUFLEN 512 // Tamanho do buffer
#define PORT 9876 // Porto para recep��o das mensagens
void erro(char *s)
{
	perror(s);
	exit(1);
}
int main(void)
{
	struct sockaddr_in si_minha, si_outra;
	int s, i, slen = sizeof(si_outra) , recv_len;
	char buf[BUFLEN];
	// Cria um socket para recep��o de pacotes UDP
	if((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		erro("Erro na cria��o do socket");
	}
	si_minha.sin_family = AF_INET;
	si_minha.sin_port = htons(PORT);
	si_minha.sin_addr.s_addr = htonl(INADDR_ANY);
	// Associa o socket � informa��o de endere�o
	if(bind(s,(struct sockaddr*)&si_minha, sizeof(si_minha)) == -1)
	{
		erro("Erro no bind");
	}
	// Espera recep��o de mensagem (a chamada � bloqueante)
	if((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_outra, &slen)) == -1)
	{
		erro("Erro no recvfrom");
	}
	// Para ignorar o restante conte�do (anterior do buffer)
	buf[recv_len]='\0';
	printf("Recebi uma mensagem do sistema com o endere�o %s e o porto %d\n",
	inet_ntoa(si_outra.sin_addr), ntohs(si_outra.sin_port));
	printf("Conte�do da mensagem: %s\n" , buf);
	// Fecha socket e termina programa
	close(s);
	return 0;
}
