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
  int client_dtr, i;
  struct sockaddr_in client;
  int port = 9999;
  char buf[30];
  char saludo[20];
  client_dtr = socket(AF_INET, SOCK_STREAM, 0);

  client.sin_addr.s_addr = inet_addr("127.0.0.1");
  client.sin_family = AF_INET;
  client.sin_port = htons(port);

  
  connect(client_dtr , (struct sockaddr *)&client , sizeof(client));
  recv(client_dtr, saludo, 20, 0);
  puts(saludo);
  fgets(buf, 30, stdin);
  send(client_dtr, buf, strlen(buf), 0);
  close(client_dtr);
  return 0;
}
