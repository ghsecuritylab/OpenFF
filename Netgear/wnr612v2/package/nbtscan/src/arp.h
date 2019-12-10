
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h> 
#include <net/if_arp.h> 
#include <signal.h> 
#include <netinet/ip.h> 

#define ETH_HW_ADDR_LEN 6
#define IP_ADDR_LEN 4
#define ARP_FRAME_TYPE 0x0806
#define RARP_FRAME_TYPE 0x8035
#define ETHER_HW_TYPE 1
#define IP_PROTO_TYPE 0x0800

#define BUFFER_SIZE 1024
#define NAME_SIZE 24
#define ARP_TABLE_SIZE 256

#define IP_STRING_LEN 16
#define MAC_STRING_LEN 18


struct arp_packet {
	u_char dst_hw_addr[ETH_HW_ADDR_LEN];
	u_char src_hw_addr[ETH_HW_ADDR_LEN];
	u_short frame_type;
	u_short hw_type;
	u_short prot_type;
	u_char hw_addr_size;
	u_char prot_addr_size;
	u_short type;
	u_char sndr_hw_addr[ETH_HW_ADDR_LEN];
	u_char sndr_ip_addr[IP_ADDR_LEN];
	u_char rcpt_hw_addr[ETH_HW_ADDR_LEN];
	u_char rcpt_ip_addr[IP_ADDR_LEN];
	u_char padding[18];
};


extern struct arp_packet ARP;
extern struct sockaddr SA;
extern int arp_sock;
extern char g_local_mac[ETH_HW_ADDR_LEN];

extern char iplist_file[];

int get_local_info (char *interface);
int get_hw_addr(char* buf,char* str);
int get_ip_addr(struct in_addr* in_addr,char* str);
int init_arp(char * interface);
void send_arp_req(int sock, struct in_addr dest_addr, struct sockaddr *sa);


void display_arp(struct arp_packet *pkt);

#define MAC_CMP(a,b) \
	memcmp((a),(b)->rcpt_hw_addr,ETH_HW_ADDR_LEN)


void mergefile(FILE *mac, FILE *name, FILE *output);
