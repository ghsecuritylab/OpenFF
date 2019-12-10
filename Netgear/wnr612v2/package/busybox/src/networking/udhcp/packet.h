#ifndef _PACKET_H
#define _PACKET_H

#include <netinet/udp.h>
#include <netinet/ip.h>
#define OPTION_SIZE 1000
struct dhcpMessage {
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	uint32_t xid;
	uint16_t secs;
	uint16_t flags;
	uint32_t ciaddr;
	uint32_t yiaddr;
	uint32_t siaddr;
	uint32_t giaddr;
	uint8_t chaddr[16];
	uint8_t sname[64];
	uint8_t file[128];
	uint32_t cookie;
	// to enlarge the buffer size
	uint8_t options[OPTION_SIZE];
};

/* only dhcp server use this struct, because only if the options filed is no more than 308 
 * that nas1100 can connect this DUT */
struct dhcpMessage_server {
	uint8_t op;
	uint8_t htype;
	uint8_t hlen;
	uint8_t hops;
	uint32_t xid;
	uint16_t secs;
	uint16_t flags;
	uint32_t ciaddr;
	uint32_t yiaddr;
	uint32_t siaddr;
	uint32_t giaddr;
	uint8_t chaddr[16];
	uint8_t sname[64];
	uint8_t file[128];
	uint32_t cookie;
	uint8_t options[308]; /* 312 - cookie */
};

struct udp_dhcp_packet {
	struct iphdr ip;
	struct udphdr udp;
	struct dhcpMessage data;
};

struct udp_dhcp_packet_server {
	struct iphdr ip;
	struct udphdr udp;
	struct dhcpMessage_server data;
};

void init_header(struct dhcpMessage *packet, char type);
int get_packet(struct dhcpMessage *packet, int fd);
uint16_t checksum(void *addr, int count);
int raw_packet(struct dhcpMessage *payload, uint32_t source_ip, int source_port,
		   uint32_t dest_ip, int dest_port, uint8_t *dest_arp, int ifindex);
int kernel_packet(struct dhcpMessage *payload, uint32_t source_ip, int source_port,
		   uint32_t dest_ip, int dest_port);


void init_header_server(struct dhcpMessage_server *packet, char type);
int get_packet_server(struct dhcpMessage_server *packet, int fd);
int raw_packet_server(struct dhcpMessage_server *payload, uint32_t source_ip, int source_port,
		   uint32_t dest_ip, int dest_port, uint8_t *dest_arp, int ifindex);
int kernel_packet_server(struct dhcpMessage_server *payload, uint32_t source_ip, int source_port,
		   uint32_t dest_ip, int dest_port);
#endif
