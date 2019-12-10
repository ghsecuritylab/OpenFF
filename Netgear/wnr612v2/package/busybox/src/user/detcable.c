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
	unsigned int unit0;//LAN0 index
	unsigned int unit1;//LAN1 index
	unsigned int unit2;//LAN2 index
	unsigned int unit3;//LAN3 index
	unsigned int unit4;//WAN index
	unsigned int addr;
	unsigned int data0;//LAN0 link status
	unsigned int data1;//LAN1 link status
	unsigned int data2;//LAN2 link status
	unsigned int data3;//LAN3 link status
	unsigned int data4;//WAN link status
    unsigned int lan0_speed;
    unsigned int lan1_speed;
    unsigned int lan2_speed;
    unsigned int lan3_speed;
    unsigned int wan_speed;
    unsigned int lan0_isFullDuplex;
    unsigned int lan1_isFullDuplex;
    unsigned int lan2_isFullDuplex;
    unsigned int lan3_isFullDuplex;
    unsigned int wan_isFullDuplex;

};

#define PLUG_OFF 0
#define PLUG_IN 1

void stop_wan()
{
	char wan_proto[16], signal[] = "XXXX";
	FILE *fp;
	if ((fp = fopen("/tmp/configs/wan_proto", "r")) == NULL){
		fprintf(stderr, "open file wan_proto failed\n");
		return;
	}
	if (fgets(wan_proto, sizeof(wan_proto), fp) == NULL){
		wan_proto[0] = '\0';
	}
	fclose(fp);
	
	if (strcmp(wan_proto, "dhcp") == 0){
		system("ifconfig eth1 0.0.0.0");
	}
}

void cable_plugin()
{
	char wan_proto[16];
	FILE *fp;
	if ((fp = fopen("/tmp/configs/wan_proto", "r")) == NULL){
		fprintf(stderr, "open file wan_proto failed\n");
		return;
	}
	if (fgets(wan_proto, sizeof(wan_proto), fp) == NULL){
		wan_proto[0] = '\0';
	}
	fclose(fp);
	
	if (strcmp(wan_proto, "dhcp") == 0){
		system("/sbin/ifconfig eth1 $(/usr/sbin/nvram get wan_dhcp_ipaddr) netmask $(/usr/sbin/nvram get wan_dhcp_netmask)");  
		system("while /sbin/route del default gw 0.0.0.0 dev eth1 >&- 2>&-; do :; done");  
		system("for gateway in $(cat /tmp/configs/wan_dhcp_gateway); do /sbin/route add default gw $gateway dev eth1; done");
		system("killall -SIGUSR1 udhcpc");
	}
	else if(strcmp(wan_proto, "static") == 0) {
                system("/sbin/ledcontrol -n wan -c green -s on");
	}
}


int detcable_main( int argc, char **argv )
{
	int fd;
	int err;
	int pid;
	int flag,pre_state;
	int lan0_pre_status = PLUG_OFF ,lan1_pre_status = PLUG_OFF ,lan2_pre_status = PLUG_OFF ,lan3_pre_status = PLUG_OFF,wan_pre_status = PLUG_OFF;
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
	miioctl.unit0 = 0;
	miioctl.unit1 = 1;
	miioctl.unit2 = 2;
	miioctl.unit3 = 3;
	miioctl.unit4 = 4;
	ifr.ifr_data = (caddr_t)&miioctl;

if(argc == 1){
	if((err=ioctl(fd, SIOCGMIIREG, &ifr))<0){
		fprintf(stderr,"ioctl error!\n");
		return -1;
	}
	close(fd);
	/*LAN 0 link status*/
	if( miioctl.data0 == PLUG_IN )
		fprintf(stderr, "LAN 0 plug in\n");
	else
		fprintf(stderr, "LAN 0 plug off\n");

	/*LAN 1 link status*/
    if( miioctl.data1 == PLUG_IN )
        fprintf(stderr, "LAN 1 plug in\n");
    else
        fprintf(stderr, "LAN 1 plug off\n");

	/*LAN 2 link status*/
    if( miioctl.data2 == PLUG_IN )
        fprintf(stderr, "LAN 2 plug in\n");
    else
        fprintf(stderr, "LAN 2 plug off\n");    

    if( miioctl.data3 == PLUG_IN )
        fprintf(stderr, "LAN 3 plug in\n");
    else
        fprintf(stderr, "LAN 3 plug off\n");   
 
	/*WAN link status*/
	if (miioctl.data4 == PLUG_IN){
		fprintf(stderr, "wan cable plug in :)\n");
		return 1;
	}
	else{
		fprintf(stderr, "wan cable plug off :(\n");
		return 0;
	}
}
else {
	int pre_status = PLUG_IN;
	system("echo Link down > /tmp/LAN0_status");
	system("echo 0 > /tmp/LAN0_uptime");
    system("echo Link down > /tmp/LAN1_status");
    system("echo 0 > /tmp/LAN1_uptime");
    system("echo Link down > /tmp/LAN2_status");
    system("echo 0 > /tmp/LAN2_uptime");
    system("echo Link down > /tmp/LAN3_status");
    system("echo 0 > /tmp/LAN3_uptime");
	system("echo 1 > /tmp/port_status");
	system("echo Link down > /tmp/WAN_status");
	system("echo 0 > /tmp/WAN_uptime");
	while (1){
		if((err=ioctl(fd, SIOCGMIIREG, &ifr))<0){
			fprintf(stderr,"ioctl error!\n");
			return -1;
		}

		/*LAN 0 link status*/
        if( miioctl.data0 == PLUG_IN ){
            if( lan0_pre_status == PLUG_OFF ){
				if( miioctl.lan0_speed == 100 )
					if( miioctl.lan0_isFullDuplex == TRUE )
                		system("echo 100M/Full > /tmp/LAN0_status");
					else
						system("echo 100M/Half > /tmp/LAN0_status");
				else
					if( miioctl.lan0_isFullDuplex == TRUE )
                        system("echo 10M/Full > /tmp/LAN0_status");
                    else
                        system("echo 10M/Half > /tmp/LAN0_status");
                system("cat /proc/uptime | sed 's/ .*//' > /tmp/LAN0_uptime");
            }
            lan0_pre_status = PLUG_IN;
        }
        else{
            if( lan0_pre_status == PLUG_IN )
                system("echo Link down > /tmp/LAN0_status");
            lan0_pre_status = PLUG_OFF;
        }
    	/*LAN 1 link status*/
        if( miioctl.data1 == PLUG_IN ){
            if( lan1_pre_status == PLUG_OFF ){
                if( miioctl.lan1_speed == 100 )
                    if( miioctl.lan1_isFullDuplex == TRUE )
                        system("echo 100M/Full > /tmp/LAN1_status");
                    else
                        system("echo 100M/Half > /tmp/LAN1_status");
                else
                    if( miioctl.lan1_isFullDuplex == TRUE )
                        system("echo 10M/Full > /tmp/LAN1_status");
                    else
                        system("echo 10M/Half > /tmp/LAN1_status");
                system("cat /proc/uptime | sed 's/ .*//'> /tmp/LAN1_uptime");
            }
            lan1_pre_status = PLUG_IN;
        }
        else{
            if( lan1_pre_status == PLUG_IN )
                system("echo Link down > /tmp/LAN1_status");
            lan1_pre_status = PLUG_OFF;
        }

    	/*LAN 2 link status*/
        if( miioctl.data2 == PLUG_IN ){
            if( lan2_pre_status == PLUG_OFF ){
                if( miioctl.lan2_speed == 100 )
                    if( miioctl.lan2_isFullDuplex == TRUE )
                        system("echo 100M/Full > /tmp/LAN2_status");
                    else
                        system("echo 100M/Half > /tmp/LAN2_status");
                else
                    if( miioctl.lan2_isFullDuplex == TRUE )
                        system("echo 10M/Full > /tmp/LAN2_status");
                    else
                        system("echo 10M/Half > /tmp/LAN2_status");                
				system("cat /proc/uptime | sed 's/ .*//'> /tmp/LAN2_uptime");
            }
            lan2_pre_status = PLUG_IN;
        }
        else{
            if( lan2_pre_status == PLUG_IN )
                system("echo Link down > /tmp/LAN2_status");
            lan2_pre_status = PLUG_OFF;
        }

		/*LAN 3 link status*/
    	if( miioctl.data3 == PLUG_IN ){
			if( lan3_pre_status == PLUG_OFF ){
                if( miioctl.lan3_speed == 100 )
                    if( miioctl.lan3_isFullDuplex == TRUE )
                        system("echo 100M/Full > /tmp/LAN3_status");
                    else
                        system("echo 100M/Half > /tmp/LAN3_status");
                else
                    if( miioctl.lan3_isFullDuplex == TRUE )
                        system("echo 10M/Full > /tmp/LAN3_status");
                    else
                        system("echo 10M/Half > /tmp/LAN3_status");
				system("cat /proc/uptime | sed 's/ .*//'> /tmp/LAN3_uptime");
			}
			lan3_pre_status = PLUG_IN;
		}
    	else{
			if( lan3_pre_status == PLUG_IN )
        		system("echo Link down > /tmp/LAN3_status");
			lan3_pre_status = PLUG_OFF;
		}
		/*WAN link status*/
		if( miioctl.data4 == PLUG_IN ){
			if( wan_pre_status == PLUG_OFF ){
                if( miioctl.wan_speed == 100 )
                    if( miioctl.wan_isFullDuplex == TRUE )
                        system("echo 100M/Full > /tmp/WAN_status");
                    else
                        system("echo 100M/Half > /tmp/WAN_status");
                else
                    if( miioctl.wan_isFullDuplex == TRUE )
                        system("echo 10M/Full > /tmp/WAN_status");
                    else
                        system("echo 10M/Half > /tmp/WAN_status");
                system("cat /proc/uptime | sed 's/ .*//'> /tmp/WAN_uptime");
			}
			wan_pre_status = PLUG_IN;
		}else{
            if( wan_pre_status == PLUG_IN )
                system("echo Link down > /tmp/WAN_status");
            wan_pre_status = PLUG_OFF;
		}

		/*WAN link status*/
		if( miioctl.data4 == PLUG_IN ){
			if(pre_status == PLUG_OFF)
			{
				system("/sbin/ledcontrol -n wan -c amber -s on");
				cable_plugin();
				system("echo 1 > /tmp/port_status");
			}
			pre_status = PLUG_IN;
		}
		else {
			if( pre_status == PLUG_IN ){
				stop_wan();
				system("/sbin/ledcontrol -n wan -s off");
				system("echo 0 > /tmp/port_status");
			}
			pre_status = PLUG_OFF;
		}
		sleep( argv[1][0]-'0' );
	}
	close(fd);
	return -1;
}
}
