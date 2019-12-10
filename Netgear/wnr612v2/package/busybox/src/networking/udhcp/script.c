/* script.c
 *
 * Functions to call the DHCP client notification scripts
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "options.h"
#include "dhcpd.h"
#include "dhcpc.h"
#include "common.h"
#include "script.h"

#include <stdio.h>
#include <ctype.h>
#include <sys/socket.h>

#include <net/if.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>


#include <string.h>

#ifdef  CONFLICT
#ifndef INADDR_NONE
# define INADDR_NONE (in_addr_t)-1
#endif

//#define DEBUGGING
#ifdef DEBUGGING
#define SYSTEM(arg...)   \
        do { \
                printf("%s: %s\n " ,__FUNCTION__,## arg);\
        } while (0)
#else
#define SYSTEM(arg)   do {system(arg); } while (0)
#endif

#define LAN_NET_ADDR0 0xC0A80100 //192.168.1.x
#define LAN_NET_ADDR1 0x0A000000 //10.0.0.x
#define LAN_NET_ADDR2 0xAC100000 // 172.16.0.x
#define LAN_NET_MASK  0xFFFFFF00 // 255.255.255.0

static char *set_lan_ip[]={"192.168.1.1","10.0.0.1","172.16.0.1"};
char *br_interface = "br0";

static int ip_conflict(char* ip_lan,char* mask_lan, char* ip_wan,char* mask_wan);
static int select_lan(char* ip_lan);
static int set_lan(int index);
#endif

#ifdef SUPPORT_OPTIONS_33_AND_121
#define OPT_33_BUF_SIZE 256
#define OPT_121_BUF_SIZE 288
#define OPT_249_BUF_SIZE 288

static int opt_33_len;
static char *opt_33_buf = NULL; // 32 pairs, 1 pair has two ip-addresses : 32 * 8 = 256
static int opt_121_len;
static char *opt_121_buf = NULL; // 32 pairs, 1 pair has max length 9 (1 + 4 + 4) : 32 * 9 = 288
static int opt_249_len;
static char *opt_249_buf = NULL; // 32 pairs, 1 pair has max length 9 (1 + 4 + 4) : 32 * 9 = 288

#define NIPQUAD(addr) \
	((unsigned char *)&addr)[0], \
	((unsigned char *)&addr)[1], \
	((unsigned char *)&addr)[2], \
	((unsigned char *)&addr)[3]

#endif
/* get a rough idea of how long an option will be (rounding up...) */
static const int max_option_length[] = {
    [OPTION_IP] =       sizeof("255.255.255.255 "),
    [OPTION_IP_PAIR] =  sizeof("255.255.255.255 ") * 2,
    [OPTION_STRING] =   1,
    [OPTION_BOOLEAN] =  sizeof("yes "),
    [OPTION_U8] =       sizeof("255 "),
    [OPTION_U16] =      sizeof("65535 "),
    [OPTION_S16] =      sizeof("-32768 "),
    [OPTION_U32] =      sizeof("4294967295 "),
    [OPTION_S32] =      sizeof("-2147483684 "),
#ifdef SUPPORT_OPTIONS_33_AND_121
    [OPTION_33] =      sizeof("255.255.255.255 "),
    [OPTION_121] =      sizeof("255.255.255.255 "),
    [OPTION_249] =      sizeof("255.255.255.255 ")
#endif
};


static inline int upper_length(int length, int opt_index)
{
    return max_option_length[opt_index] *
        (length / option_lengths[opt_index]);
}


static int sprintip(char *dest, char *pre, uint8_t *ip)
{
    return sprintf(dest, "%s%d.%d.%d.%d", pre, ip[0], ip[1], ip[2], ip[3]);
}


/* really simple implementation, just count the bits */
static int mton(struct in_addr *mask)
{
    int i;
    unsigned long bits = ntohl(mask->s_addr);
    /* too bad one can't check the carry bit, etc in c bit
     * shifting */
    for (i = 0; i < 32 && !((bits >> i) & 1); i++);
    return 32 - i;
}

#ifdef SUPPORT_OPTIONS_33_AND_121
static void to_file(char *fname, char *buff)
{
	char line[16];
	FILE *fp = NULL;
	sprintf(line, "/tmp/%s", fname);
	fp = fopen(line, "w+");
	if (fp)
	{
		fprintf(fp, "%s", buff);
		fclose(fp);
	}
}

// to process option 121 and option 249
static void process_classless_route(char *dest, uint8_t *option, int total_len)
{
	int  k, len = 0;
	unsigned char classless_sroute[1536];

	memset(classless_sroute, 0x0, sizeof(classless_sroute));
	for (k = 0; k < total_len; k += 5)
	{
		if (option[k] <= 8)
		{
			len += sprintf(classless_sroute + len, "%d.%d.0.0.0 ", option[k], option[k+1]);
			k++;
		}
		else if ((option[k] > 8) && (option[k] <= 16))
		{
			len += sprintf(classless_sroute + len, "%d.%d.%d.0.0 ", option[k], option[k+1], option[k+2]);
			k += 2;
		}
		else if ((option[k] > 16) && (option[k] <= 24))
		{
			len += sprintf(classless_sroute + len, "%d.%d.%d.%d.0 ", option[k], option[k+1], option[k+2], option[k+3]);
			k += 3;
		}
		else
		{
			len += sprintf(classless_sroute + len, "%d.%d.%d.%d.%d ", option[k], option[k+1], option[k+2], option[k+3], option[k+4]);
			k += 4;
		}
		len +=	sprintf(classless_sroute + len, "%d.%d.%d.%d ", option[k+1], option[k+2], option[k+3], option[k+4]);
	}

	to_file(dest, classless_sroute);
}
#endif

/* Fill dest with the text of option 'option'. */
static void fill_options(char *dest, uint8_t *option, struct dhcp_option *type_p)
{
    int type, optlen;
    uint16_t val_u16;
    int16_t val_s16;
    uint32_t val_u32;
    int32_t val_s32;
    int len= option[OPT_LEN - 2];
#ifdef SUPPORT_OPTIONS_33_AND_121
	int tmp_len;
#endif
    dest += sprintf(dest, "%s=", type_p->name);

    type = type_p->flags & TYPE_MASK;
    optlen = option_lengths[type];
    for(;;) {
#ifdef SUPPORT_OPTIONS_33_AND_121
		if ((type >= OPTION_33) && (type <= OPTION_121))
			len = option[OPT_LEN - 2];
#endif
        switch (type) {
        case OPTION_IP_PAIR:
            dest += sprintip(dest, "", option);
            *(dest++) = '/';
            option += 4;
            optlen = 4;
        case OPTION_IP: /* Works regardless of host byte order. */
            dest += sprintip(dest, "", option);
            break;
        case OPTION_BOOLEAN:
            dest += sprintf(dest, *option ? "yes" : "no");
            break;
        case OPTION_U8:
            dest += sprintf(dest, "%u", *option);
            break;
        case OPTION_U16:
            memcpy(&val_u16, option, 2);
            dest += sprintf(dest, "%u", ntohs(val_u16));
            break;
        case OPTION_S16:
            memcpy(&val_s16, option, 2);
            dest += sprintf(dest, "%d", ntohs(val_s16));
            break;
        case OPTION_U32:
            memcpy(&val_u32, option, 4);
            dest += sprintf(dest, "%lu", (unsigned long) ntohl(val_u32));
            break;
        case OPTION_S32:
            memcpy(&val_s32, option, 4);
            dest += sprintf(dest, "%ld", (long) ntohl(val_s32));
            break;
        case OPTION_STRING:
            memcpy(dest, option, len);
            dest[len] = '\0';
            return;  /* Short circuit this case */
#ifdef SUPPORT_OPTIONS_33_AND_121
        case OPTION_33:
			tmp_len = (opt_33_len + len) >= OPT_33_BUF_SIZE?(OPT_33_BUF_SIZE - opt_33_len):len;
			// buffer is full 
			if (!tmp_len)
				return;
			memcpy(opt_33_buf + opt_33_len, option, tmp_len);
			opt_33_len += tmp_len;
			if (*(option + tmp_len) == DHCP_STATIC_ROUTE || *(option + tmp_len) == DHCP_MICROSOFT_ENCODING_LONG_OPTIONS)
			{
				option += (tmp_len + 2); // shift to next option (current len + next op code + next op len);
				continue;
			}
			return;
        case OPTION_121:
			tmp_len = (opt_121_len + len) >= OPT_121_BUF_SIZE?(OPT_121_BUF_SIZE - opt_121_len):len;
			// buffer is full 
			if (!tmp_len)
				return;
			memcpy(opt_121_buf + opt_121_len, option, tmp_len);
			opt_121_len += tmp_len;
			if (*(option + tmp_len) == DHCP_CLASSLESS_STATIC_ROUTE || *(option + tmp_len) == DHCP_MICROSOFT_ENCODING_LONG_OPTIONS)
			{
				option += (tmp_len + 2); // shift to next option (current len + next op code + next op len);
				continue;
			}			
			return;
		case OPTION_249:
			tmp_len = (opt_249_len + len) >= OPT_249_BUF_SIZE?(OPT_249_BUF_SIZE - opt_249_len):len;
			// buffer is full 
			if (!tmp_len)
				return;
			memcpy(opt_249_buf + opt_249_len, option, tmp_len);
			opt_249_len += tmp_len;
			if (*(option + tmp_len) == DHCP_MICROSOFT_CLASSLESS_STATIC_ROUTE || *(option + tmp_len) == DHCP_MICROSOFT_ENCODING_LONG_OPTIONS)
			{
				option += (tmp_len + 2); // shift to next option (current len + next op code + next op len);
				continue;
			}

			return;
#endif
        }
        option += optlen;
        len -= optlen;
        if (len <= 0) break;
        dest += sprintf(dest, " ");
    }
}

#ifdef  CONFLICT
static int sprintcheckip(char *dest,  uint8_t *ip)
{
    return sprintf(dest, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

static int nvram_get(const char *name, char * buf, int len)
{
        char filename[64] = "";
        FILE *fp;

    snprintf(filename, sizeof(filename), "/tmp/configs/%s", name);
    if(!(fp = fopen(filename, "r"))){
        fprintf(stderr, "open file %s failed\n", filename);
        return 0;
    }
    if(fgets(buf, len, fp) <= 0){
        fprintf(stderr, "read file %s failed\n", filename);
            fclose(fp);
        return 0;
    }

    fclose(fp);
    return 1;
}

int nvram_set(const char *name, const char *value)
{
        char filename[64];
        FILE *fp;

    snprintf(filename, sizeof(filename), "/tmp/configs/%s", name);
        if(!(fp = fopen(filename, "w"))){
        fprintf(stderr, "Create the file %s failed\n", filename);
        return 0;
    }

    fputs(value, fp);
    fclose(fp);

    return 1;
}


static int set_lan(int index)
{
    char *lan_ip = NULL;
    char *dhcp_start = NULL;
    char *dhcp_end = NULL;

    if(index != 0 && index != 1 && index != 2)
        return -1;

    switch(index){
        case 1:
            lan_ip = "10.0.0.1";
            dhcp_start = "10.0.0.2";
            dhcp_end = "10.0.0.254";
            break;

        case 2:
            lan_ip = "172.16.0.1";
            dhcp_start = "172.16.0.2";
            dhcp_end = "172.16.0.254";
            break;

        case 0:
            lan_ip = "192.168.1.1";
            dhcp_start = "192.168.1.2";
            dhcp_end = "192.168.1.254";
            break;
        default:
            fprintf(stderr,"DHCP: there is something wrong when wan lan conflict\n");
            break;
    }

    nvram_set("lan_ipaddr", lan_ip);
    nvram_set("lan_netmask", "255.255.255.0");
    nvram_set("dhcp_start", dhcp_start);
    nvram_set("dhcp_end", dhcp_end);

    return 1;
}


static int select_lan(char* lan_ip)
{
    int ret = 0;
    unsigned long ip_lan;
    unsigned long lan_net;

        if((ip_lan = inet_addr(lan_ip)) == INADDR_NONE) {
        return -1;
    }

    lan_net = ntohl(ip_lan) & LAN_NET_MASK;
        if(lan_net == LAN_NET_ADDR0)
        ret = 1;
    else if (lan_net == LAN_NET_ADDR1)
        ret = 2;
    else
        ret = 0;

    return ret;
}

static int ip_conflict(char* ip_lan,char* mask_lan, char* ip_wan,char* mask_wan)
{
    unsigned long lan_ip;
    unsigned long lan_mask;
    unsigned long wan_ip;
    unsigned long wan_mask;
    unsigned long lan_subnet;
    unsigned long wan_subnet;

    int res = 0;

    printf("%s/%s\n%s/%s\n",ip_lan, mask_lan, ip_wan, mask_wan);

    if((lan_ip = inet_addr(ip_lan)) == INADDR_NONE){
        res = -1;
        goto exit;
    }

    if((lan_mask = inet_addr(mask_lan)) == INADDR_NONE){
        res = -2;

    }

    if((wan_ip = inet_addr(ip_wan)) == INADDR_NONE){
        res = -3;
        goto exit;
    }

    if((wan_mask = inet_addr(mask_wan)) == INADDR_NONE){
        res = 0;
        goto exit;
    }

    //use lan mask cmp
    lan_subnet = ntohl((lan_ip) & lan_mask);
    wan_subnet = ntohl((wan_ip) & lan_mask);
    if(lan_subnet == wan_subnet){
        res = 1;
        goto exit;
    }

    //use wan mask cmp
    lan_subnet = ntohl((lan_ip) & wan_mask);
    wan_subnet = ntohl((wan_ip) & wan_mask);
    if(lan_subnet == wan_subnet){
        res =2;
    }
exit:
    //printf("ip_conflict return %d  [1/2  |conflict,  0 | no conflict]\n",res);
    return res;



}



/* put all the parameters into an environment */
int check_ip_conflict(struct dhcpMessage *packet)
{
    int i,len,optlen;
    char ip[17];
    char mask[17];
    char br_ip[17];
    char br_mask[17];
    char dns_list[70];
    char dns_tmp[17];
    char *dns1,*dns2;
    char dns_addr1[17]={0};
    char dns_addr2[17]={0};
    int index;
    char cmd[64];
    int conflict_flag=0;
    uint8_t *temp;
    struct in_addr subnet;
    FILE *fp;
    //char over = 0;
    if (packet == NULL) return -1;

    for (i = 0; dhcp_options[i].code; i++) {
        if (!(temp = get_option(packet, dhcp_options[i].code)))
            continue;
	if (dhcp_options[i].code == DHCP_DNS_SERVER) {
		len = temp[OPT_LEN - 2]; 
		optlen = 4;
		for(;;) { 
			sprintf(dns_tmp, "%d.%d.%d.%d", temp[0], temp[1], temp[2], temp[3]);
			strcat(dns_list,dns_tmp);
			temp += optlen;
		        len -= optlen;
		        if (len <= 0) break;
		        strcat(dns_list, " ");
		} 
		if(strcmp(dns_list,"")!=0){
			dns1=strtok(dns_list," ");
			if(dns1)
				strcpy(dns_addr1,dns1);
			dns2=strtok(NULL," ");
			if(dns2)
				strcpy(dns_addr2,dns2);
		}
	}
        if (dhcp_options[i].code == DHCP_SUBNET) {
            memcpy(&subnet, temp, 4);
            sprintf(mask,"%s", inet_ntoa(subnet));
        }
    }
	
  	//printf("\n@@dns1(%s), dns2(%s)@@\n\n",dns_addr1, dns_addr2);
  	sprintcheckip(ip, (uint8_t *) &packet->yiaddr);

  	int fd;
  	struct ifreq ifreq;
    struct sockaddr_in *sin;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if( fd < 0 )
    {
        printf(" Can't open socket \n");
        return -1;	
    }	
    
	  memset (&ifreq, 0, sizeof (ifreq));
    
	  strncpy(ifreq.ifr_name, "br0", sizeof("br0"));
    
    /* use ioctl to get netdev ip addr */
    if (ioctl (fd, SIOCGIFADDR, &ifreq) < 0)
    {
          printf("ssc_create: Can't get %s device ip address", ifreq.ifr_name); 
          close(fd);
          return -1;   
    }
    
	  sin = (struct sockaddr_in *)&ifreq.ifr_addr;
	  strcpy(br_ip, inet_ntoa(sin->sin_addr));
    
	  if (ioctl (fd, SIOCGIFNETMASK, &ifreq) < 0)
    {
          printf("ssc_create: Can't get %s device ip address", ifreq.ifr_name);
          close(fd);
          return -1;
    }
    close(fd);

	  sin = (struct sockaddr_in *)&ifreq.ifr_netmask;
  	strcpy(br_mask, inet_ntoa(sin->sin_addr));

    if(ip_conflict(br_ip, br_mask, ip, mask) > 0)
		conflict_flag=1;
    
    if(dns1){
			if(ip_conflict(br_ip, br_mask, dns_addr1, mask) > 0)
				conflict_flag=1;
    }
    if(dns2){
			if(ip_conflict(br_ip, br_mask, dns_addr2, mask) > 0) 
				conflict_flag=1;
    }
	
    if(conflict_flag==1){
    	index = select_lan(br_ip);
      if(index != 0 && index != 1 && index != 2)
      	return 0;
			system("rm -rf /tmp/configs/no_need_conflict");
			sprintf(cmd, "/sbin/conflict_wanlan.sh %s %s %s %s",ip,mask,dns_addr1,dns_addr2);
			SYSTEM(cmd);
			if ((fp = fopen("/tmp/configs/no_need_conflict", "r")) != NULL) {
      	fclose(fp);
	    	return 0;
      }else{
	    	sprintf(cmd, "/sbin/cmdlan.sh stop");
        SYSTEM(cmd);
        sprintf(cmd, "/sbin/ip_conflict.sh start &");
        SYSTEM(cmd);
	    	return 1;
      }
    }

    return 0;
}

#endif

#ifdef SUPPORT_OPTIONS_33_AND_121
// ip data should be nework order
static inline int
valid_ip(unsigned long data)
{
	unsigned char *ip = (unsigned char *)&data;

	// loop-back
	if (ip[0] == 0x7f)
		return 0;
	// broadcast address
	if (ip[0] == 0xff || ip[3] ==  0xff)
		return 0;
	// multicast address
	if ((ip[0] & 0xff) == 0xe0)
		return 0;

	return 1;
}

static void process_static_route(void)
{
	if (opt_33_len)
	{
		unsigned char *buff = NULL;
		int  l = 0;
		unsigned long *addr, *gw, *end;
		// Because we accept 32 pairs static routes( each pair has a ipaddress and a gateway, 32*(16+16) = 1024),
		// to malloc 1100 should be enough...
		buff = xmalloc(1100);
		if (buff)
		{
			memset(buff, 0, 1100);
			addr = (unsigned long *)opt_33_buf;
			gw = addr + 1;
			end = (unsigned long *)(opt_33_buf + opt_33_len);
			for (; gw < end ; addr += 2, gw += 2)
			{
				if (valid_ip(*addr)&&valid_ip(*gw))
					l += sprintf(buff + l, "%u.%u.%u.%u %u.%u.%u.%u ", NIPQUAD(*addr), NIPQUAD(*gw));
			}

			to_file("sroute", buff);
			free(buff);
		}
    }

	if (opt_121_len)
		process_classless_route("csroute", opt_121_buf, opt_121_len);

	if (opt_249_len)
		process_classless_route("csroute2", opt_249_buf, opt_249_len);

}
static void init_buf(void)
{
	if (!opt_33_buf)
		opt_33_buf = xmalloc(OPT_33_BUF_SIZE);

	if (!opt_121_buf)
		opt_121_buf = xmalloc(OPT_121_BUF_SIZE);

	if (!opt_249_buf)
		opt_249_buf = xmalloc(OPT_249_BUF_SIZE);

	opt_33_len = 0;
	opt_121_len = 0;
	opt_249_len = 0;

	memset(opt_33_buf, 0, sizeof(opt_33_buf));
	memset(opt_121_buf, 0, sizeof(opt_121_buf));
	memset(opt_249_buf, 0, sizeof(opt_249_buf));

}

#endif


/* put all the parameters into an environment */
static char **fill_envp(struct dhcpMessage *packet)
{
    int num_options = 0;
    int i, j;
    char **envp;
    uint8_t *temp;
    uint32_t lease_time;
    FILE *fp;
    time_t timer;
    struct in_addr subnet;
    char over = 0;

    if (packet == NULL)
        num_options = 0;
    else {
        for (i = 0; dhcp_options[i].code; i++)
            if (get_option(packet, dhcp_options[i].code)) {
                num_options++;
                if (dhcp_options[i].code == DHCP_SUBNET)
                    num_options++; /* for mton */
            }
        if (packet->siaddr) num_options++;
        if ((temp = get_option(packet, DHCP_OPTION_OVER)))
            over = *temp;
        if (!(over & FILE_FIELD) && packet->file[0]) num_options++;
        if (!(over & SNAME_FIELD) && packet->sname[0]) num_options++;
    }

    envp = xcalloc(sizeof(char *), num_options + 5);
    j = 0;
    asprintf(&envp[j++], "interface=%s", client_config.interface);
    asprintf(&envp[j++], "%s=%s", "PATH",
        getenv("PATH") ? : "/bin:/usr/bin:/sbin:/usr/sbin");
    asprintf(&envp[j++], "%s=%s", "HOME", getenv("HOME") ? : "/");

    if (packet == NULL) return envp;

    envp[j] = xmalloc(sizeof("ip=255.255.255.255"));
    sprintip(envp[j++], "ip=", (uint8_t *) &packet->yiaddr);
/*******************************alime add*******************************/
    if (temp = get_option(packet, DHCP_LEASE_TIME)){
        memcpy(&lease_time, temp, 4);
        lease_time = ntohl(lease_time);
        if ((fp = fopen("/tmp/configs/dhcpc_lease_time", "w")) != NULL) {
            fprintf( fp, "%u", lease_time);
            fclose(fp);
        }

        /* Change the envionment variable: TZ, according to user's config */
        if((fp = fopen("/tmp/configs/time_zone", "r")) != NULL) {
            char time_zone[32];
            fgets(time_zone, 32, fp);
            setenv("TZ", time_zone, 1);
            fclose(fp);
        }
        timer = time(0);
        temp = ctime(&timer);
        if ((fp = fopen("/tmp/configs/dhcpc_lease_obtain", "w")) != NULL) {
            fprintf( fp, "%s", temp);
            fclose(fp);
        }
    }

//  envp[j] = xmalloc(sizeof("lease_time=4294967296"));
//  if (temp = get_option(packet, DHCP_LEASE_TIME)){
//      memcpy(&lease_time, temp, 4);
//      lease_time = ntohl(lease_time);
//  }
//  sprintip(envp[j++], "lease_time=%d", lease_time);
/***********************************************************************/
#ifdef SUPPORT_OPTIONS_33_AND_121
	init_buf();
#endif
    for (i = 0; dhcp_options[i].code; i++) {
        if (!(temp = get_option(packet, dhcp_options[i].code)))
            continue;
        envp[j] = xmalloc(upper_length(temp[OPT_LEN - 2],
            dhcp_options[i].flags & TYPE_MASK) + strlen(dhcp_options[i].name) + 2);
        fill_options(envp[j++], temp, &dhcp_options[i]);

        /* Fill in a subnet bits option for things like /24 */
        if (dhcp_options[i].code == DHCP_SUBNET) {
            memcpy(&subnet, temp, 4);
            asprintf(&envp[j++], "mask=%d", mton(&subnet));
        }
    }
#ifdef SUPPORT_OPTIONS_33_AND_121
	process_static_route();
#endif
    if (temp = get_option(packet, DHCP_SERVER_ID)){
//      envp[j] = xmalloc(sizeof("server=255.255.255.255"));
//      sprintip(envp[j++], "server=", (uint8_t *)temp);
        if (fp = fopen("/tmp/configs/wan_dhcp_server", "w")){
            fprintf( fp, "%s", inet_ntoa(*(struct in_addr *)temp) );
            fclose(fp);
        }

    }
    else{
        if (fp = fopen("/tmp/configs/wan_dhcp_server", "w")){
            fprintf( fp, "");
            fclose(fp);
        }
    }

    if (packet->siaddr) {
        envp[j] = xmalloc(sizeof("siaddr=255.255.255.255"));
        sprintip(envp[j++], "siaddr=", (uint8_t *) &packet->siaddr);
    }
    if (!(over & FILE_FIELD) && packet->file[0]) {
        /* watch out for invalid packets */
        packet->file[sizeof(packet->file) - 1] = '\0';
        asprintf(&envp[j++], "boot_file=%s", packet->file);
    }
    if (!(over & SNAME_FIELD) && packet->sname[0]) {
        /* watch out for invalid packets */
        packet->sname[sizeof(packet->sname) - 1] = '\0';
        asprintf(&envp[j++], "sname=%s", packet->sname);
    }
    return envp;
}


/* Call a script with a par file and env vars */
void run_script(struct dhcpMessage *packet, const char *name)
{
    int pid;
    char **envp, **curr;

    if (client_config.script == NULL)
        return;

    DEBUG(LOG_INFO, "vforking and execle'ing %s", client_config.script);

    envp = fill_envp(packet);
    /* call script */
    pid = vfork();
    if (pid) {
        waitpid(pid, NULL, 0);
        for (curr = envp; *curr; curr++) free(*curr);
        free(envp);
        return;
    } else if (pid == 0) {
        /* close fd's? */

        /* exec script */
        execle(client_config.script, client_config.script,
               name, NULL, envp);
        LOG(LOG_ERR, "script %s failed: %m", client_config.script);
        exit(1);
    }
}
