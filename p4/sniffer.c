/* Practica 3: Sniffer que muestra direccion
 * MAC de destino y de ORIGEN.
 *
 * Protocolos de Internet
 *
 * 18/07/2015
 *
 * Gabriel Acosta
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
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

unsigned int* parse_ipv4(uint32_t direccion) {
  int current_byte = 0, i;
  unsigned int host[4] = {.0, .0, .0, .0};
  /* int residuo = 0, octeto = 0; */
  /* while(current_byte < 4) { */
  /*   if(direccion < 256 && current_byte == 0) { */
  /*     host[current_byte] = direccion; */
  /*     for(i = current_byte + 1; i < 4; i++) { */
  /* 	host[i] = 0; */
  /*     } */
  /*   } else { */
  /*     residuo = direccion%256; */
  /*     octeto = residuo*256; */
  /*     direccion -= octeto; */

  /*     host[current_byte++] = octeto; */
      
  /*     if(direccion < 256){ */
  /* 	for(i = current_byte + 1; i < 4; i++) { */
  /* 	  host[i] = 0; */
  /* 	} */
  /*     return host; */
  /*     }  */
  /*   } */
  /* } */
  return host;
  
}

int main(int argc, char **argv) {

  if(argc < 2) {
    printf("Uso: ./sniffer paquetes");
    exit(1);
  }

  int npack = atoi(argv[1]);
  
  char b[1542];
  int sdtr, data_size;
  socklen_t saddr_size;
  struct ifreq ethreq;
  struct sockaddr saddr;
  int icmp = 0, igmp = 0;
  int tcp = 0, udp = 0, otros = 0;
  int total = 0;
  int n_ipv4 = 0, n_arp = 0;
  sdtr = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  
  if(sdtr < 0) {
    printf("Error SOCKET");
    exit(1);
  }

  FILE *file;
  file = fopen("d.txt", "w");
  
  strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);
  
  ioctl(sdtr, SIOCGIFFLAGS, &ethreq);
  ethreq.ifr_flags |= IFF_PROMISC;
  
  ioctl(sdtr, SIOCSIFFLAGS, &ethreq);
  fprintf(file, "                                    \n");
  while((n_ipv4 + n_arp) < npack) {
    
    saddr_size = sizeof saddr;
    data_size = recvfrom(sdtr, b, 1542, 0, &saddr, (socklen_t *)&saddr_size);
    if(data_size < 0) {
      printf("\n:( recvfrom\n");
      exit(1);
    }

    struct ethhdr *eth = (struct ethhdr*)(b);
    struct iphdr *iph = (struct iphdr *)(b + sizeof(eth));
    switch(ntohs(eth->h_proto)) {
    case 0x800:
      n_ipv4++;
      fprintf(file,"Destino: %x:%x:%x:%x:%x:%x\t",
	     eth->h_dest[0],
	     eth->h_dest[1],
	     eth->h_dest[2],
	     eth->h_dest[3],
	     eth->h_dest[4],
	     eth->h_dest[5],
	     eth->h_dest[6]);
      fprintf(file, "Origen: %x:%x:%x:%x:%x:%x\t",
	     eth->h_source[0],
	     eth->h_source[1],
	     eth->h_source[2],
	     eth->h_source[3],
	     eth->h_source[4],
	     eth->h_source[5],
	     eth->h_source[6]);
      fprintf(file, "Trama: %d \t Datos: %d\t", data_size, data_size - 14);
      unsigned int *source = parse_ipv4(iph->saddr);
      unsigned int *dest = parse_ipv4(iph->daddr);
      fprintf(file, "src: %d.%d.%d.%d\t", source[3], source[2], source[1], source[0]);
      fprintf(file, "dest: %d.%d.%d.%d\n", dest[3], dest[2], dest[1], dest[0]);
      fprintf(file, "ttl %i\t servicio %i\t version %d\t hdrl %d\t\tLongitud total Datagrama %d\t",
	      iph->ttl,
	      iph->tos,
	      (unsigned int)(iph->version),
	      iph->ihl*4,
	      ntohs(iph->tot_len)
	      );
      fprintf(file, "id %d \t protocolo %d\n", ntohs(iph->id), iph->protocol);
      /* unsigned int *source  = parse_ipv4(iph->saddr); */
      /* printf("%s\n",inet_ntoa(iph->saddr)); */
      break;
    case 0x806:
      n_arp++;
      break;
    }
    
  }
  rewind(file);
  fprintf(file, "ipv4: %d \t arp: %d\n", n_ipv4, n_arp);
  fclose(file);
  close(sdtr);
  return 0;
}


