#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "dhcpd.h"
#include "leases.h"
#include "libbb_udhcp.h"

#define REMAINING 0
#define ABSOLUTE 1

char *arg_get(const char *name, char *arg)
{
	char filename[64] = "";
	FILE *fp;

	snprintf(filename, sizeof(filename), "/tmp/configs/%s", name);
	if(!(fp = fopen(filename, "r"))){
		fprintf(stderr, "open file %s failed\n", filename);
		return "";
	}
	if(fgets(arg, 32, fp) == NULL)
		arg[0] = '\0';
        fclose(fp);
        return arg;
}

int get_interface(char *interface, uint32_t *addr, uint8_t *arp)
{
	int fd;
	struct ifreq ifr;
	struct sockaddr_in *our_ip;

	memset(&ifr, 0, sizeof(struct ifreq));
	if((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) >= 0) {
		ifr.ifr_addr.sa_family = AF_INET;
		strcpy(ifr.ifr_name, interface);
		
		if (addr) {
			if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
				our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
				*addr = our_ip->sin_addr.s_addr;
			} else {
				return -1;
			}
		}

		if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
			memcpy(arp, ifr.ifr_hwaddr.sa_data, 6);
		} else {
			return -1;
		}
	} else {
		return -1;
	}
	
	close(fd);
	return 0;
}

#ifndef IN_BUSYBOX
static void __attribute__ ((noreturn)) show_usage(void)
{
	printf(
"Usage: dumpleases -f <file> -[r|a] -l\n\n"
"  -f, --file=FILENAME             Leases file to load\n"
"  -r, --remaining                 Interepret lease times as time remaing\n"
"  -a, --absolute                  Interepret lease times as expire time\n"
"  -l, --live                      Show only current attatched devices with dynamic assigned\n");
	exit(0);
}
#else
#define show_usage bb_show_usage
#endif


#ifdef IN_BUSYBOX
int dumpleases_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	char server_interface[32];
	uint8_t server_arp[6];
	uint32_t server_ip;
	FILE *fp;
	int i, c, mode = REMAINING;
	int live = 0;
	long expires;
	const char *file = LEASES_FILE;
	struct dhcpOfferedAddr lease;
	struct in_addr addr;

	static const struct option options[] = {
		{"absolute", 0, 0, 'a'},
		{"remaining", 0, 0, 'r'},
		{"file", 1, 0, 'f'},
		{"live", 0, 0, 'l'},
		{0, 0, 0, 0}
	};

	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "arf:l", options, &option_index);
		if (c == -1) break;

		switch (c) {
		case 'a': mode = ABSOLUTE; break;
		case 'r': mode = REMAINING; break;
		case 'f':
			file = optarg;
			break;
		case 'l': live = 1; break;
		default:
			show_usage();
		}
	}

	if (live == 1){
		arg_get("lan_ifname", server_interface);
		get_interface(server_interface, &server_ip, server_arp); 
	}
	fp = xfopen(file, "r");

//	printf("Mac Address       IP-Address      Host-Name	Expires %s\n", mode == REMAINING ? "in" : "at");
	/*     "00:00:00:00:00:00 255.255.255.255 hostname	Wed Jun 30 21:49:08 1993" */
	while (fread(&lease, sizeof(lease), 1, fp)) {
		if (live == 1 
			&& arpping( lease.yiaddr, server_ip, 
				server_arp, server_interface)==1){
			continue;
		}
		
/*
		for (i = 0; i < 6; i++) {
			printf("%02x", lease.chaddr[i]);
			if (i != 5) printf(":");
		}    
*/
		if( strcmp(lease.hostname, "") == 0 )
			sprintf(lease.hostname, "N/A");				
		addr.s_addr = lease.yiaddr;
		printf(" %-15s", inet_ntoa(addr));
		printf(" %-20s", lease.hostname);
		for (i = 0; i < 6; i++) {
			printf("%02x", lease.chaddr[i]);
			if (i != 5) printf(":");
		}
		printf("\n");	
		expires = ntohl(lease.expires);
		printf(" ");
		if (mode == REMAINING) {
			if (!expires) printf("expired\n");
			else {
				if (expires > 60*60*24) {
					printf("%ld days, ", expires / (60*60*24));
					expires %= 60*60*24;
				}
				if (expires > 60*60) {
					printf("%ld hours, ", expires / (60*60));
					expires %= 60*60;
				}
				if (expires > 60) {
					printf("%ld minutes, ", expires / 60);
					expires %= 60;
				}
				printf("%ld seconds\n", expires);
			}
		} else printf("%s", ctime(&expires));
		
	}
	fclose(fp);

	return 0;
}
