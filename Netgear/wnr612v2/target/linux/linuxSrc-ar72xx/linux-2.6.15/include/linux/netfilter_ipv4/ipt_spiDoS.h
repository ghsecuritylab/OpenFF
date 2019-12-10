#ifndef _IPT_SPIDOS_H
#define _IPT_SPIDOS_H

struct ipt_spiDoS {
	char iface[IFNAMSIZ];
	unsigned int echochargen;
	unsigned int lan_netaddr;
	unsigned int lan_netmask;
};

enum ipt_tcp_scan_state {
	TCP_SCAN_CHECK = 1,
	TCP_SCAN_COUNT
};

struct ipt_spiTcpScan {
	char iface[IFNAMSIZ];
	unsigned int state;	
};
#endif /*_IPT_SPIDOS_H*/
