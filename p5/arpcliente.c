#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <string.h>
/* #include <if_arp.h> */
struct msgARP{
  unsigned char destinoEthernet[6]; /* Direccion de difusion 0xFF */
  unsigned char origenEthernet[6];  /* Direccion MAC del transmisor */
  unsigned short tipoEthernet;	    /* Tipo de mensaje en la trama Ethernet */
  unsigned short tipoHardware;	    /* Tipo de mensaje utilizado para difundir el mensaje ARP(Ethernet) */
  unsigned short tipoProtocolo;	    /* Tipo de red utilizado para difundir el mensaje ARP (Ip) */
  unsigned char longitudHardware;   /* Tamaño de direcciones de hardware */
  unsigned char longitudProtocolo;  /* Tamaño de direcciones del protocolo (4 bytes)  */
  unsigned short tipoMensaje;	    /* Solicitud o respuesta */
  unsigned char origenMAC[6];	    /* Direccion MAC del transmisor */
  unsigned char origenIP[6];	    /* Direccion IP del transmisor */
  unsigned char destinoMAC[6];	    /* Direccion MAC del receptor(direccion solicitada) */
  unsigned char destinoIP[4];	    /* Direccion IP del receptor (dato de entrada)  */
};


int main(int argc, char **argv) {
  int dummy;
  unsigned char dummyIP[4];
  inet_pton(dummy, "192.168.0.10", dummyIP);
  struct msgARP msg;
  struct ifreq ifr;
  int socketdtr;
  int optvalue = 0;
  /* msg.destinoIP = dummyIP; */
  /* memset(msg.destinoIP, ) */
  msg.longitudHardware = 6;
  msg.longitudProtocolo = 4;
  /* memset(msg.destinoEthernet, 0xFFFFFFFF, 4); */
  /* memset((msg.destinoEthernet + 4), 0xFFFF, 2); */
  /* unsigned char *multi = (unsigned char )0xFFFFFFFFFFFF;
   */
  memset(msg.destinoEthernet, 0xFF, 6*sizeof(uint8_t));
  msg.tipoEthernet = htons(ETH_P_ARP);
  msg.tipoHardware = htons(1);
  msg.tipoProtocolo = htons(ETH_P_IP);
  msg.tipoMensaje = htons(1);
  
  socketdtr = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ARP));
  int cuack =  setsockopt(socketdtr, SOL_PACKET, SO_BROADCAST, &optvalue,
	     sizeof(optvalue));

  strcpy(ifr.ifr_name, "eth0");
  
  ioctl(socketdtr, SIOCGIFHWADDR, &ifr);
  bcopy(&ifr.ifr_hwaddr.sa_data, msg.origenMAC, 6);
  /* sa_data: los primeros dos bytes son el puerto 
   *          los siguientes cuatro la ipv4
   */
  ioctl(socketdtr, SIOCGIFADDR, &ifr);
  bcopy(&ifr.ifr_addr.sa_data[2], msg.origenIP, 4);
  struct sockaddr s;
  sendto(socketdtr, &msg, sizeof(msg), 0, &s, sizeof(s));
  
  return 0;

}
