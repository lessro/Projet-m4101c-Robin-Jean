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
  //const char *msg_bienvenue = "  /\\_/\\ \n=( °w° )= \n  )   (  // \n (__ __)// \n ";
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
	{
	  FILE  * fichier = fdopen(client,"w+");

	  char buffer[200];

	  fgets(buffer,200,fichier);

	  printf("recu : %s",buffer);

    int isErreur400 = 0;
    int isErreur404 =0;

	  const char * erreur400 = "HTTP/1.1 400 Bad request\r\nConnection: close\r\nContent-length: 17\r\n\r\n400 Bad request\r\n";
	  const char * erreur404 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-length: 15\r\n\r\n404 Not Found\r\n";
    
	  //VERIFICATIONS REQUETE
	  //verification premier mot = GET
	  if(strcmp(strtok(buffer," "),"GET") != 0){
	    isErreur400=1;

	  }

	  //verification deuxieme mot existe
	  else if(strcmp(strtok(NULL," "),"/")!=0){
	    isErreur404=1;

	  }
	  char * mot3 = strtok(NULL," ");//on met de cote le 3e mot pr lanalyser apres
	  
	  //on verifie quil ny a pas de 4e mot
	  printf("%s", mot3);

	  if(strncmp(mot3,"HTTP/1.0",8) != 0 && strncmp(mot3,"HTTP/1.1",8) != 0 ){
	    isErreur400 =1;
      printf("erreur 3éme mot\n");
	  }

    else if(strtok(NULL," ") != NULL){
      isErreur400=1;  
      printf("mot 4 pas sencé être al\n");     
    }


	  while(strcmp(buffer,"\n") != 0 && strcmp(buffer,"\r\n") != 0){
	    fgets(buffer,200,fichier);
	    
	  }

        
    if(isErreur400==1){
      fprintf(fichier, "%s\n", erreur400);
      
    }
    else if(isErreur404==1){
      fprintf(fichier, "%s\n", erreur404);
      
    }

	  else if (fprintf (fichier,"pawnee %s",buffer)==-1){
	    perror ("erreur de transposition");
	  }
        
        

	  fflush(fichier);

  
	  close(client);
	  exit(0);
	  break;
	}
      default:
	close(client);
	//retour début de la boucle, attente nouvelle connection (process principal)
        break;
      }
    }
  }

  return sock;
}
