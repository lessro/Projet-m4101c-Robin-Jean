#include <stdio.h>
#include <string.h>



  
  int socket_serveur;
  
  socket_serveur = socket(AF_INET, STOCK_STREAM,0);
  
  if (socket_serveur == -1) {
    
    perror("socket_serveur");
    
  }

struct sockaddr_in {
  sa_family_t sin_family;
  in-port_t sin_port;
  struct in-addr sin_addr;

}


  if (listen(socket_serveur,10)==-1) {

    perror("Listen socket_serveur");

  }



  
 
