/* Protocolos de Internet
 * Gabriel Acosta
  *
 * Creacion de sockets entre servidor-cliente
 * 11/09/15
 * client.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  /* fflush(stdin); */
  /* fflush(stdout); */
  int client_dtr, i;
  struct sockaddr_in client;
  int port = 2500;
  char buf[30];
  char saludo[20];
  
  
  if((client_dtr = socket(AF_INET,
			  SOCK_STREAM, 0)) == -1) {
    printf("ERROR: socket\n");
    return -1;
  }

  client.sin_addr.s_addr = inet_addr("127.0.0.1");
  client.sin_family = AF_INET;
  client.sin_port = htons(port);

  int s = connect(client_dtr,
		  (struct sockaddr *)&client,
		  sizeof(client));
  if(s  == -1) {
    printf("ERROR: connect\n");
    return -1;
  }

  /* Tal vez se requieran mas de una llamada
   a recv para recibir toda la informacion 
  enviada desde el server*/
  if(recv(client_dtr, saludo, 20, 0) < 0) {
    printf("ERROR: recv\n");
    return -1;
  }
  printf("%s\n", saludo); 
  fflush(stdin);
  fflush(stdout);
  fgets(buf, 30, stdin);
  if(write(client_dtr, buf, 30) < 0) {
    printf("ERROR: write\n");
    return -1;
  }
  int l;

  if(recv(client_dtr, &l, sizeof(l), 0) < 0) {
    printf("ERROR: recv\n");
    return -1;
  }
  printf("Longitud : %i\n", l);
  close(client_dtr);
  
  fflush(stdout);
  return 0;
}
