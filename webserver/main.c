#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "socket.h"

void initialiser_signaux(void);


int main(void)
{
	initialiser_signaux();
	//if(argc > 1 && argv[1] == ""){
	creer_serveur(8888);
	//}else{
	//	printf("precisez le port en parametre svp");
	//}
	


	return 0;
	
}


void traitement_signal(int sig){
	printf("Signal %d reçu\n", sig);
	waitpid(0,NULL,WNOHANG);
}

void initialiser_signaux(void){

	struct sigaction sa;

	sa.sa_handler = traitement_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) ==-1){
		perror("sigaction(SIGCHLD)");
	}
}

