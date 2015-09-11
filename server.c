/* Protocolos de Internet
 * Gabriel Acosta
  *
 * Creacion de sockets entre servidor-cliente
 * 11/09/15
 * server.c
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
int main() {
  char saludo[] = "Escriba su nombre ";
  int port = 2500;
  char buf[256];
  struct sockaddr_in server;
  struct sockaddr_in client;

  int server_dtr;		/* Descriptor del server */
  int client_dtr;		/* Descriptor del cliente */
  
  socklen_t addrlen;

  server_dtr = socket(AF_INET,
		      SOCK_STREAM,0);

  memset(&server, 0, sizeof(server)); /* 'Limpia' la estructura */
  
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port); /* Convierte entero desde 
				    orden de bytes de red al de host 
				 */

  /* Bind asigna la direccion de sockaddr al descriptor */
  bind(server_dtr,
       (struct sockaddr *)&server,
       sizeof(server));

  
  listen(server_dtr, 2);

  while(1) {
  printf("Esperando conexiones entrantes ... \n");

  client_dtr = accept(server_dtr, 
		      (struct sockaddr *)&client, 
		      &addrlen);
    
  printf("Conexión con cliente!\n");

  if(write(client_dtr, saludo, 20) < 0) {
    printf("ERROR: write\n");
    return -1;
  }


  if(recv(client_dtr, buf, 30, 0) < 0) {
    printf("ERROR: recv\n");
    return -1;
  }

  puts(buf);
  
  /* Conteo de los caracteres y eliminacion
     de saltos de linea y espacios.*/

  int l = strlen(buf);
  int i;
  for(i = 0; i < strlen(buf); i++) {
    char c = buf[i];
    if(c == '\n' || c == ' ') {
      l--;
    }
  }

  printf("Longitud : %i\n", l);

  if(write(client_dtr, &l, sizeof(l)) < 0) {
    printf("ERROR: write\n");
    return -1;
  }

  close(client_dtr);
  }
  return 0;
}
