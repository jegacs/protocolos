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
  char saludo[25];
  
  
  client_dtr = socket(AF_INET, SOCK_STREAM, 0);

  client.sin_addr.s_addr = inet_addr("127.0.0.1");
  client.sin_family = AF_INET;
  client.sin_port = htons(port);

  
  connect(client_dtr , (struct sockaddr *)&client , sizeof(client));
  
  recv(client_dtr, saludo, 20, 0);
  printf("%s\n", saludo);
  fflush(stdin);
  fgets(buf, 30, stdin);
  write(client_dtr, buf, 30);
  int l;
  recv(client_dtr, &l, sizeof(l), 0);
  printf("Longitud : %i\n", l);
  close(client_dtr);
  
  fflush(stdout);
  return 0;
}
