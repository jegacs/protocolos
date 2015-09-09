#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>

int main() {
  char saludo[] = "Escriba su nombre : ";
  int port = 2500;
  char buf[256];
  struct sockaddr_in server;
  struct sockaddr_in client;

  int server_dtr;		/* Descriptor del server */
  int client_dtr;		/* Descriptor del cliente */
  
  socklen_t addrlen;

  server_dtr = socket(AF_INET, SOCK_STREAM,0);
  memset(&server, 0, sizeof(server)); /* 'Limpia' la estructura */
  
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port); /* Convierte entero desde orden de bytes de red al de host */

  bind(server_dtr, (struct sockaddr *)&server, sizeof(server));
  listen(server_dtr, 2);
  while(1) {
  printf("Esperando conexiones entrantes ... \n");
  client_dtr = accept(server_dtr, 
		      (struct sockaddr *)&client, 
		      &addrlen);
  printf("Conexión con cliente!\n");
  write(client_dtr, saludo, 20);
  recv(client_dtr, buf, 30, 0);
  puts(buf);
  int l = strlen(buf);
  int i;
  for(i = 0; i < strlen(buf); i++) {
    char c = buf[i];
    if(c == '\n' || c == ' ') {
      l--;
    }
  }
  printf("Longitud : %i\n", l);
  /* write(client_dtr, &l, sizeof(l)); */
  close(client_dtr);
  }
  return 0;
}
