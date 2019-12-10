#if defined(MODVERSIONS)
#include <linux/modversions.h>
#endif
#include <linux/module.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/version.h>
#include <linux/netfilter_ipv4/ipt_dnifilter.h>
#include <net/tcp.h>
#include <net/udp.h>

#define get_u8(X,O)  (*(__u8 *)(X + O))
#define get_u16(X,O)  (*(__u16 *)(X + O))
#define get_u32(X,O)  (*(__u32 *)(X + O))

MODULE_AUTHOR("Copyright (C) 2007-2008 Delta Networks Inc.");
MODULE_DESCRIPTION("An extension to iptables to identify specific traffic by port and content.");
MODULE_LICENSE("GPL");


/* search for Netgear EVA */
int udp_search_eva(struct sk_buff *skb)
{
    struct iphdr *ip = skb->nh.iph;
    int hlen = ntohs(ip->tot_len)-(ip->ihl*4);  /*hlen = packet-iphead length*/

    if (ip->protocol == IPPROTO_UDP) {
        struct udphdr *udph = (void *) ip + ip->ihl * 4;
        if (ntohs(udph->dest) >= 49152 && ntohs(udph->dest) <= 49155 && ntohs(udph->len) > 8+7) {
            unsigned char *haystack = (void *)udph + 8;
            if (haystack[4] == 0x53 && haystack[5] == 0x4a && haystack[6] == 0x61 
                && haystack[7] == 0x6d) {
                return DNIFILTER_EVA;
            }
        }
    }

	return 0;
}

static struct {
    int command;
    int (*function_name) (struct sk_buff *skb);
} match_list[] = {
    {DNIFILTER_EVA, &udp_search_eva},
    {0,NULL}
};


static int
match(const struct sk_buff *skb,
      const struct net_device *in,
      const struct net_device *out,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,17)
      const struct xt_match  *mymatch,
      const void *matchinfo,
      int offset,
      unsigned int myprotoff,
#else
      const void *matchinfo,
      int offset,
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
      const void *hdr,
      u_int16_t datalen,
#endif

      int *hotdrop)
{
    const struct ipt_dnifilter_info *info = matchinfo;

    int result = 0, i = 0;


    /*must not be a fragment*/
    if (offset) {
	    if (info->debug) printk("DNIFILTER.match: offset found %i \n",offset);
    	return 0;
    }
    
    /*make sure that skb is linear*/
    if(skb_is_nonlinear(skb)){
    	if (info->debug) printk("DNIFILTER.match: nonlinear skb found\n");
	    return 0;
    }
	    
    while (match_list[i].command){
		if (info->cmd  == match_list[i].command) {
			    result = match_list[i].function_name(skb);
			    if (result){
    				if (info->debug) {
                        struct iphdr *ip = skb->nh.iph;
                        struct udphdr *udph = (void *) ip + ip->ihl * 4;
                        int hlen = ntohs(ip->tot_len)-(ip->ihl*4);
                        printk("DNIFILTER.debug:UDP-match: %i from: %u.%u.%u.%u:%i to: %u.%u.%u.%u:%i Length: %i\n", 
    				    result, NIPQUAD(ip->saddr),ntohs(udph->source), NIPQUAD(ip->daddr),ntohs(udph->dest),hlen);
    				}
				    return result;
			    }
		}
        i++;
    }			
    
	return 0;

}



static int
checkentry(const char *tablename,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,17)
           const void *ip, 
           const struct xt_match *mymatch,
#else
           const struct ipt_ip *ip,
#endif
	   void *matchinfo,
	   unsigned int matchsize,
	   unsigned int hook_mask)
{
    return 1;
}
									    

// TODO: find out what this structure is for (scheme taken
// from kernel sources)
// content seems to have a length of 8 bytes 
// (at least on my x86 machine)
struct dnifilter_match_info {
	long int dunno_what_this_is_for;
	long int i_also_dunno_what_this_is_for;
};

static struct ipt_match dnifilter_match = { 
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
	{ NULL, NULL }, 
	"dnifilter", 
	&match, 
	&checkentry, 
	NULL, 
	THIS_MODULE
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,17))
	.name		= "dnifilter",
	.match		= &match,
	.checkentry	= &checkentry,
	.me		= THIS_MODULE,
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,17)
	.name		= "dnifilter",
	.match		= &match,
	.family         = AF_INET,
	.matchsize      = sizeof(struct dnifilter_match_info),
	.checkentry	= &checkentry,
	.me		= THIS_MODULE,
#endif
};


static int __init init(void)
{
    printk(KERN_INFO "DNIFILTER loading\n");
    return ipt_register_match(&dnifilter_match);
}
	
static void __exit fini(void)
{
    ipt_unregister_match(&dnifilter_match);
    printk(KERN_INFO "DNIFILTER unloaded\n");    
}
	
module_init(init);
module_exit(fini);


