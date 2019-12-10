#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
//#include <sys/wait.h>
#include <sys/types.h>
#include <net/if.h>
#include "busybox.h"

/* ioctl interface */
#define SIOCGMIIREG	0x8948		/* Read MII PHY register.	*/

/* This structure is used in all SIOCxMIIxxx ioctl calls */
struct ioctl_data {
	unsigned short		phy_id;
	unsigned short		reg_num;
	unsigned short		val_in;
	unsigned short		val_out;
};

#define PLUG_OFF 0
#define PLUG_IN 1


int wanwd_main( int argc, char **argv )
{
	int fd;
	int err;
	int pid;
	int flag,pre_state , curr_state;
	struct ifreq ifr;
	struct ioctl_data miioctl;

	memset(&ifr, 0, sizeof(ifr));
	memset(&miioctl, 0, sizeof(miioctl));
	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ);
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		fprintf(stderr, "Cannot open socket");
		exit(1);
	}
	miioctl.phy_id = 18;
	miioctl.reg_num = 0x00;
	miioctl.val_in = 0;
	miioctl.val_out = 0;
	ifr.ifr_data = (caddr_t)&miioctl;
	while(1){
		if((err=ioctl(fd, SIOCGMIIREG, &ifr))<0){
			fprintf(stderr,"ioctl error!\n");
			continue;
		}
		
		if( flag==1 )
			pre_state = curr_state;	
		
		switch (miioctl.val_out & 0x0800) {
			case 0x0800 :
				if ( flag==0 ){
					pre_state = PLUG_IN;
					flag = 1;
				}
				curr_state = PLUG_IN;
				break;
			case 0x0000 :
				if ( flag==0){
					pre_state = PLUG_OFF;
					flag = 1;
				}
				curr_state = PLUG_OFF;
				break;
			default:
				break;
		}
		if( pre_state == PLUG_OFF && curr_state == PLUG_IN ){
			system("logger -- \"Wan interface Plug-in\"");
			//system("/demo/firewall.sh stop > /dev/null 2>&1");
			system("/demo/wan_conf.sh wanwd_det > /dev/null 2>&1");
			//system("/demo/static_route.sh > /dev/null 2>&1");
			//system("/demo/ddns.sh restart > /dev/null 2>&1");
			//system("/demo/upnp-igd.sh restart > /dev/null 2>&1");
			//system("/demo/firewall.sh restart > /dev/null 2>&1");
		}
		sleep(1);
		continue;
	}
	return -1;
}
		
	
	
