#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
/* Practica 2: Simple sniffer.
 *
 * Protocolos de Internet
 *
 * 18/07/2015
 *
 * Gabriel Acosta
 *
 */


#include <netinet/in.h>
#include <net/if.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <linux/if_ether.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <net/ethernet.h>
#include <linux/if_packet.h>

int main() {
  
  char b[1500];
  int sdtr, data_size;
  socklen_t saddr_size;
  struct ifreq ethreq;
  struct sockaddr saddr;
  int icmp = 0, igmp = 0;
  int tcp = 0, udp = 0, otros = 0;
  int total = 0;
  
  sdtr = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  
  if(sdtr < 0) {
    printf("Error SOCKET");
    exit(1);
  }
  strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);
  
  ioctl(sdtr, SIOCGIFFLAGS, &ethreq);
  ethreq.ifr_flags |= IFF_PROMISC;
  
  ioctl(sdtr, SIOCSIFFLAGS, &ethreq);

  while(1) {
    
    saddr_size = sizeof saddr;
    data_size = recvfrom(sdtr, b, 256, 0, &saddr, (socklen_t *)&saddr_size);
    if(data_size < 0) {
      printf("\n:( recvfrom\n");
      exit(1);
    }

    struct iphdr *iph = (struct iphdr*)(b + sizeof(struct ethhdr));
    
    ++total;
    switch (iph->protocol) {
    case 1:  
      ++icmp;
      break;
    case 2:  
      ++igmp;
      break;
    case 6:  
      ++tcp;
      break;
    case 17: 
      ++udp;
      break;
    default: 
      ++otros;
      break;
    }
    printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Otros : %d   Total : %d\r",
	   tcp,
	   udp,
	   icmp,
	   igmp,
	   otros,
	   total);
  }
  close(sdtr);
  return 0;
}
