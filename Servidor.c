#include "header.h"


void verifica_exis(char* nome, char* pass, lis_util original){
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

}