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

void initialiser_signaux(void){
	if ( signal (SIGPIPE,SIG_IGN) == SIG_ERR ) {
		perror ( " signal ");
	}


}