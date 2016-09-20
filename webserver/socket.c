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

  int sock = socket(AF_INET,SOCK_STREAM,0);

  if(sock == -1){
    perror("erreur lors de la creation du serveur");
    exit(0);
  }

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;

  if(bind(sock,(struct sockaddr *)&saddr,sizeof(saddr)) == -1){
    perror("probleme bind maggle");
  }

  if(listen(sock,5) == -1){
    perror("Probleme avec la file de connections");
  }

  

  return sock;
}