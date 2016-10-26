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



const char * erreur400 = "HTTP/1.1 400 Bad request\r\nConnection: close\r\nContent-length: 17\r\n\r\n400 Bad request\r\n";
const char * erreur404 = "HTTP/1.1 404 Not Found\r\nConnection: close\r\nContent-length: 15\r\n\r\n404 Not Found\r\n";

enum http_method {
  HTTP_GET,
  HTTP_UNSUPPORTED ,
};

typedef struct{
  enum http_method method;
  int major_version;
  int minor_version;
  char *target;
} http_request;


int parse_http_request(const char * request_line, http_request *request){
    

    char *tmpLine = (char *) malloc(sizeof(request_line));

    strcpy(tmpLine,request_line);

    if(strcmp(strtok(tmpLine," "),"GET") != 0){
      request->method=HTTP_UNSUPPORTED;
      return 0;
    }

    request->target = strtok(NULL," ");
 

    char * mot3 = strtok(NULL," ");//on met de cote le 3e mot pr lanalyser apres
    
    //on verifie quil ny a pas de 4e mot
    
    if(strcmp(mot3,"HTTP/1.0\n") == 0 || strcmp(mot3,"HTTP/1.0\r\n") == 0){
      request->major_version=1;
      request->minor_version=0;
      request->method = HTTP_GET;
    }
    else if(strcmp(mot3,"HTTP/1.1\n") == 0 || strcmp(mot3,"HTTP/1.1\r\n") == 0){
      request->major_version=1;
      request->minor_version=1;
      request->method = HTTP_GET;

    }else{
      return 0;
    }
   

    if(strtok(NULL," ") != NULL){
      return 0;   
    }
    return 1;
}


void fgets_or_exit(char *buffer, int size, FILE * stream){
    char *rep = fgets(buffer,size,stream);
    if(rep == NULL){
      exit(0);
    }
}

void skip_headers(FILE *client){
  char tmpBuffer[200];
  fgets(tmpBuffer,200,client);

  while(strcmp(tmpBuffer,"\n") != 0 && strcmp(tmpBuffer,"\r\n") != 0){
    fgets(tmpBuffer,200,client);
  }
}



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
        int isErreur400=0;
        int isErreur404=0;
    	  FILE  * fichier = fdopen(client,"w+");

        int taille = 200;

    	  char buffer[taille];

        fgets_or_exit(buffer,taille,fichier);

        http_request * req = malloc(sizeof(http_request));

        if(parse_http_request(buffer,req)==0){
          isErreur400=1;
        }
        skip_headers(fichier);


            
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








 