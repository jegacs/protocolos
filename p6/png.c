#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

unsigned short in_checksum (unsigned short *, int);

int main(int argc, char **argv) {
  srand(time(NULL));
  int rrr;
  int packet_size = sizeof (struct iphdr) + sizeof (struct icmphdr) + 21;
  /* unsigned char *paquete = (unsigned char *) malloc(packet_size); */
  /* int rrr = 0; */

  /* struct iphdr *iph = (struct iphdr *)(paquete); */
  /* struct icmphdr *icmph = (struct icmphdr *)(paquete + sizeof(struct iphdr)); */

  struct iphdr *iph = malloc(sizeof(struct iphdr));
  struct iphdr *ip_reply = malloc(sizeof(struct iphdr));
  struct icmphdr *icmph = malloc(sizeof(struct icmphdr));
  unsigned char *paquete = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr));
  /* unsigned char *buffer = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr)); */

  iph = (struct iphdr*) paquete;
  icmph = (struct icmphdr*) (paquete + sizeof(struct iphdr));
  
  /* char *dest_addr = "192.168.0.6";
   */
  char *dest_addr = argv[1];
  char *src_addr = "192.168.0.4";
  iph->id = htons(rand());
  iph->ttl = 255;
  iph->protocol = IPPROTO_ICMP;
  iph->saddr = inet_addr(src_addr);
  iph->daddr = inet_addr(dest_addr);

  /* memset((char*) icmph, 0, sizeof(icmph)); */
  iph->version = 4;
  iph->tot_len = packet_size; 
  iph->ihl = 5;
  iph->tos = 0;
  iph->check = in_checksum((unsigned short *)iph, sizeof(struct iphdr));
  
  icmph->type = ICMP_ECHO;
  icmph->code = 0;
  icmph->un.echo.id = 0;
  icmph->un.echo.sequence = 0;
  icmph->checksum = 0;
  icmph->checksum = in_checksum((unsigned short *)icmph, sizeof(struct icmphdr));


  
  struct sockaddr_in sa;
  int addrlen = sizeof(sa);

  
  int optval = 0;
  char buffer[100];
  
  int s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
  if(s == -1) {
   printf("Socket fallo\n");
   exit(1);
  }
  setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval));
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr(dest_addr);

  int i = 0;
  for(; i < 10; i++){
  rrr = sendto(s, paquete, packet_size, 0, (struct sockaddr *)&sa, sizeof(struct sockaddr));
  if(rrr == -1) {
    printf("sendto fallo\n");
    exit(1);
  }
  }
  int x;
  
  x = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, &addrlen);

  struct iphdr *iphr = (struct iphdr *)buffer;
  struct icmphdr *icmphr = (struct icmphdr *)(buffer + sizeof(struct iphdr));

  printf("");
  
  printf("Adiós!\n");
  free(paquete);
  close(s);
  
  return 0;
}

unsigned short in_checksum (unsigned short *addr, int len)
{ 
  register int sum = 0; 
  u_short answer = 0; 
  register u_short *w = addr; 
  register int nleft = len; 
  while  (nleft > 1) 
    { 
      sum += *w++; 
      nleft -= 2; 
      if (nleft == 1) 
	{ 
	  *(u_char *) (&answer) = *(u_char *) w; 
	  sum += answer;    
	} 
      sum = (sum >> 16) + (sum & 0xffff); 
      sum += (sum >> 16); 
      answer = ~sum; 
      return(answer); 
    } 
} 
