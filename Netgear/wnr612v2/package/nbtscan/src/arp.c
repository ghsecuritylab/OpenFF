

#include "arp.h"


struct arp_packet ARP;
struct sockaddr SA;
int arp_sock;

char g_local_mac[ETH_HW_ADDR_LEN];
char g_local_ip[IP_ADDR_LEN];
char g_local_mask[IP_ADDR_LEN];
char iplist_file[] = "/tmp/iplist-nbt";

/*
int main(){
	struct in_addr dest_addr;
	inet_aton("172.17.145.80", &dest_addr);
	init_arp_request("eth0");

	while(1){
		send_arp_req(arp_sock, dest_addr, &SA);
		recv_arp();
	}
	return 0;

}
*/

void send_arp_req(int sock, struct in_addr dest_addr, struct sockaddr *sa){

	memcpy(ARP.rcpt_ip_addr,&dest_addr,IP_ADDR_LEN);

	 if(sendto(sock,&ARP,sizeof(ARP),0,sa,sizeof(*sa)) < 0){

		perror("sendto");

	 }

}


int recv_arp( )
{
	//struct in_addr src_in_addr,dst_in_addr;
	struct arp_packet pkt;
	char buffer[BUFFER_SIZE];
	unsigned int recv_size = 0;
	//unsigned int count = 0;	
/*	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 1000;	

	//if(select(arp_sock+1, (fd_set *)NULL, (fd_set *)NULL,(fd_set *)NULL, &timeout) == 0)
	//{
	//	printf("time out \n");
	//	return -1;
//	}
	setsockopt(arp_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
*/	
	if((recv_size = recvfrom(arp_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&SA, sizeof(SA))) <= 0 )
	{
		printf("recv error \n");
		//exit(-1);
		return -1;
	}

	//printf("recv OK .\n");
	display_arp((struct arp_packet *)buffer);	
	//}
	return 1;
}


void display_arp(struct arp_packet *pkt)
{
//#ifdef DEBUG
	int verbose = 1;
	int type;
	char src_hw_addr[18];
	char src_ip[16];
	char dst_hw_addr[18];
	char dst_ip[16];
	struct sockaddr_in ina;
	
	sprintf(src_hw_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
			pkt->sndr_hw_addr[0],
			pkt->sndr_hw_addr[1],
			pkt->sndr_hw_addr[2],
			pkt->sndr_hw_addr[3],
			pkt->sndr_hw_addr[4],
			pkt->sndr_hw_addr[5]);
	sprintf(src_ip,"%d.%d.%d.%d",
			pkt->sndr_ip_addr[0],
			pkt->sndr_ip_addr[1],
			pkt->sndr_ip_addr[2],
			pkt->sndr_ip_addr[3]);

	sprintf(dst_hw_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
			pkt->rcpt_hw_addr[0],
			pkt->rcpt_hw_addr[1],
			pkt->rcpt_hw_addr[2],
			pkt->rcpt_hw_addr[3],
			pkt->rcpt_hw_addr[4],
			pkt->rcpt_hw_addr[5]);
	 sprintf(dst_ip,"%d.%d.%d.%d",
			 pkt->rcpt_ip_addr[0],
			 pkt->rcpt_ip_addr[1],
			 pkt->rcpt_ip_addr[2],
			 pkt->rcpt_ip_addr[3]);

	 ina.sin_addr.s_addr = inet_addr(src_ip);
	 //printf("\n%u\n%u\n%u\n",local_range.start_ip,ntohl(ina.sin_addr.s_addr),local_range.end_ip);
	/* if((ntohl(ina.sin_addr.s_addr) < local_range.start_ip ) || (ntohl(ina.sin_addr.s_addr) > local_range.end_ip))
	 {
		 //printf("recv arp out of range \n");
		 return;
	 }
	 */
	 //printf("Received ARP Packet:\n");
//#ifdef DEBUG
	if (verbose) {
		printf("Received ARP Packet:\n");
		printf("    Interface:                \n");
		printf("    Packet type:             %04X\n", ntohs(pkt->frame_type));
		printf("    Message type:             %d\n", ntohs(pkt->type));
		printf("    Sender hardware address:  %s\n", src_hw_addr);
		printf("    Sender protocol address:  %s\n", src_ip);
		printf("    Target hardware address:  %s\n", dst_hw_addr);
		printf("    Target protocol address:  %s\n", dst_ip);
	}
//#endif	
}



int init_arp_request(char * interface)
{
	//struct sockaddr sa;
	//struct in_addr src_in_addr,dst_in_addr;
	  struct timeval timeout;

	if (get_local_info (interface)){
    		arp_sock=socket(AF_INET,SOCK_PACKET,htons(ETH_P_ARP));
   			if(arp_sock<0){
        		//perror("socket");
        		return -1;
    		}
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

    //if(select(arp_sock+1, (fd_set *)NULL, (fd_set *)NULL,(fd_set *)NULL, &timeout) == 0)
    //{
    //  printf("time out \n");
    //  return -1;
//  }
    setsockopt(arp_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    		ARP.frame_type = htons(ARP_FRAME_TYPE);
    		ARP.hw_type = htons(ETHER_HW_TYPE);
    		ARP.prot_type = htons(IP_PROTO_TYPE);
    		ARP.hw_addr_size = ETH_HW_ADDR_LEN;
    		ARP.prot_addr_size = IP_ADDR_LEN;
    		ARP.type=htons(1);


    		memcpy(ARP.src_hw_addr, &g_local_mac, ETH_HW_ADDR_LEN);
    		memcpy(ARP.sndr_hw_addr, &g_local_mac, ETH_HW_ADDR_LEN);
    		if(get_hw_addr(ARP.dst_hw_addr, "ff:ff:ff:ff:ff:ff")){
				close(arp_sock);
				return -1;
    		}
	
    		//memcpy(&src_in_addr,&g_local_ip, IP_ADDR_LEN);
    		memcpy(ARP.sndr_ip_addr,&g_local_ip, IP_ADDR_LEN);


	
   		// get_hw_addr(pkt.src_hw_addr,src_hw_addr);
   		// get_hw_addr(pkt.dst_hw_addr, "ff:ff:ff:ff:ff:ff");
   		//get_hw_addr(pkt.sndr_hw_addr, src_hw_addr);
   			if(get_hw_addr(ARP.rcpt_hw_addr, "00:00:00:00:00:00")){
   				close(arp_sock);
   				return -1;
   			}


    		bzero(ARP.padding,18);

    		strcpy(SA.sa_data, interface);

			return 1;
	}

	return -1;
}








int get_hw_addr(char* buf,char* str){

    int i;
    char c,val;
    char hw_addr[64];

    strcpy (hw_addr, str);

    for(i=0;i<ETH_HW_ADDR_LEN;i++){
        if( !(c = tolower(*str++))) {
            //char msg[64];
            //sprintf(msg, "Invalid hardware address: %s", hw_addr);
            //die(msg);
            return 1;
        }
        if(isdigit(c)) val = c-'0';
        else if(c >= 'a' && c <= 'f') val = c-'a'+10;
        else {
            //char msg[64];
            //sprintf(msg, "Invalid hardware address: %s", hw_addr);
	     return 1;
            //die(msg);
        }

        *buf = val << 4;
        if( !(c = tolower(*str++))) return 1;//die("Invalid hardware address");
        if(isdigit(c)) val = c-'0';
        else if(c >= 'a' && c <= 'f') val = c-'a'+10;
        else {
            //char msg[64];
            //sprintf(msg, "Invalid hardware address: %s", hw_addr);
	     return 1;
            //die(msg);
        }

        *buf++ |= val;

        if(*str == ':')str++;
    }
	return 0;
}


int get_ip_addr(struct in_addr* in_addr,char* str){

    struct hostent *hostp;

    in_addr->s_addr=inet_addr(str);
    if(in_addr->s_addr == -1){
        if( (hostp = gethostbyname(str)))
            bcopy(hostp->h_addr,in_addr,hostp->h_length);
        else {
            //fprintf(stderr,"send_arp: unknown host %s\n",str);
            return 1;
        }
    }
	return 0;
}


int
get_local_info (char *interface)
{
int fd;
struct  in_addr myself, mymask; 
struct sockaddr_in *sin_ptr;
struct ifreq ifr;
int rev= 1;
  	 
memset (&ifr, 0, sizeof (struct ifreq));
if ((fd = socket(AF_INET, SOCK_PACKET, htons(0x0806))) < 0) { 
        //perror( "arp socket error"); 
       return -1; 
    } 
ifr.ifr_addr.sa_family = AF_INET;
strcpy (ifr.ifr_name, interface);
  	 
if (ioctl (fd, SIOCGIFHWADDR, &ifr) == 0)
{

    memcpy(&g_local_mac,ifr.ifr_hwaddr.sa_data,ETH_HW_ADDR_LEN);
}
else
{
  rev= -1;
  goto exit;
}

if (ioctl (fd, SIOCGIFADDR, &ifr) == 0)
{
    sin_ptr = (struct sockaddr_in *)&ifr.ifr_addr; 
    myself  = sin_ptr->sin_addr; 
    memcpy(&g_local_ip,&(myself.s_addr),IP_ADDR_LEN);
}
else
{
  rev= -2;
  goto exit;
}
if (ioctl (fd, SIOCGIFNETMASK, &ifr) == 0)
{
    sin_ptr = (struct sockaddr_in *)&ifr.ifr_addr; 
    mymask  = sin_ptr->sin_addr; 
    memcpy(&g_local_mask,&(mymask.s_addr),IP_ADDR_LEN);
}
else
{
  rev= -3;
  goto exit;
}
exit:

	close(fd);
	return rev;

}


void mergefile(FILE *mac, FILE *name, FILE *output)
{
    int ok;
	char m_ip[32];
	char m_mac[32];
	char n_ip[32];
	char n_name[128];

    if(mac == NULL || name == NULL || output == NULL)
        return -1;
    while(fscanf(mac, "%s %s\n", m_ip, m_mac))
    {
        ok = 0;
        //sleep(1);
        while(fscanf(name,"%s %s\n",n_ip, n_name))
        {
            if(strcmp(n_ip, m_ip) == 0)
            {
                ok = 1;
                rewind(name);
                break;

            }
            if(feof(name))
            {
                rewind(name);
                break;
            }
        }

        if(ok)
            fprintf(output,"%s %s %s   @#$&*!\n",m_ip, m_mac, n_name);
        else
            fprintf(output,"%s %s <unknown>      @#$&*!\n", m_ip, m_mac);

        if(feof(mac))
            break;
    }

    return 0;
}

