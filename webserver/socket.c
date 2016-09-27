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

int creer_serveur(int port){
  const char *msg_bienvenue = "Bonjour ! Bienvenue\n";
  int sock = socket(AF_INET,SOCK_STREAM,0);
  int optval = 1;
  int pid;

  if(sock == -1){
    perror("erreur lors de la creation du serveur");
    exit(0);
  }

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;


  if(setsockopt(sock, SOL_SOCKET,SO_REUSEADDR, &optval, sizeof(int))==-1){
    perror("Can not set SO_REUSEADDR option");
  }

  if(bind(sock,(struct sockaddr *)&saddr,sizeof(saddr)) == -1){
    perror("probleme bind ");
  }

  if(listen(sock,10) == -1){
    perror("Probleme avec la file de connections");
  }

  int i = 0;

  while (1) { // tant que le client ne s'est pas connecté au serveur , attente de connection du client
    
    printf("%i client connecte(s) \n",i);
    i++;
    int client = accept(sock , NULL , NULL); // fonction blocquante

    if(client == -1){
      perror("erreur accept");
    }else{
      pid=fork();
      switch(pid){
      case -1:
          perror("bug de fork!!");
          exit(1);
          break;
      case 0:
        //fils (processus cree) nouveau client a gerer

        
        write(client, msg_bienvenue, strlen(msg_bienvenue));
        close(client);
        exit(0);
        break;
      default:
          close(client);
          //retour début de la boucle, attente nouvelle connection (process principal)
        break;
    }

  }
}

  return sock;
}