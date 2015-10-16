#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

unsigned short in_cksum(const unsigned short *, int, u_short);


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
  unsigned char *paquete = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr) + 21);
  /* unsigned char *buffer = malloc(sizeof(struct iphdr) + sizeof(struct icmphdr)); */

  iph = (struct iphdr*) paquete;
  icmph = (struct icmphdr*) (paquete + sizeof(struct iphdr) - 21);
  
  char *dest_addr = "192.168.0.3";
  char *src_addr = "192.168.0.4";
  iph->id = htons(rand());
  iph->ttl = 255;
  iph->protocol = IPPROTO_ICMP;
  iph->saddr = inet_addr(src_addr);
  iph->daddr = inet_addr(dest_addr);
  
  iph->version = 4;
  iph->tot_len = packet_size; 
  iph->ihl = 5;
  iph->tos = 0;
    
  icmph->type = ICMP_ECHO;
  icmph->code = 0;
  /* icmph->un.echo.id = 0; */
  /* icmph->un.echo.sequence = 0; */
  icmph->checksum = 0;
  icmph->checksum = in_cksum((unsigned short *)icmph, sizeof(struct icmphdr), 0);

  /* iph->check = in_cksum((unsigned short *)iph, sizeof(struct iphdr)); */
  
  struct sockaddr_in sa;
  int addrlen = sizeof(sa);

  
  int optval = 0;
  char buffer[100];
  
  int s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
  setsockopt(s, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval));
  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = inet_addr(dest_addr);

  int i = 0;
  for(; i < 10; i++)
  rrr = sendto(s, paquete, packet_size, 0, (struct sockaddr *)&sa, sizeof(struct sockaddr));
  int x;
  
  x = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&sa, &addrlen);

  struct iphdr *iphr = (struct iphdr *)buffer;
  struct icmphdr *icmphr = (struct icmphdr *)(buffer + sizeof(struct iphdr));
  printf("Adiós!\n");
  free(paquete);
  close(s);
  
  return 0;
}

#if BYTE_ORDER == LITTLE_ENDIAN
# define ODDBYTE(v)	(v)
#elif BYTE_ORDER == BIG_ENDIAN
# define ODDBYTE(v)	((u_short)(v) << 8)
#else
# define ODDBYTE(v)	htons((u_short)(v) << 8)
#endif


u_short
in_cksum(const u_short *addr, int len, u_short csum)
{
	register int nleft = len;
	const u_short *w = addr;
	register u_short answer;
	register int sum = csum;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1)
		sum += ODDBYTE(*(u_char *)w); /* le16toh() may be unavailable on old systems */

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}
