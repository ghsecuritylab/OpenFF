/* This is a module which is used for source-NAT-P2P.
 * with concept helped by Rusty Russel <rusty at rustcorp.com.au>
 * and with code by Jesse Peng <tzuhsi.peng at msa.hinet.net>
 */
#include <linux/netfilter_ipv4.h>
#include <linux/module.h>
#include <linux/kmod.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_nat.h>
#include <linux/netfilter_ipv4/ip_nat_core.h>
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#include <net/ip.h>

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

/*
 * Defect ID : 331 Loopback mode is not supported
 *
 * Description 
 *   From the LAN PC of the same Router can not access the Router config using the
 * dyndns name, nor access a port which is forwarded to a computer on the lan.
 *   For example, If we set port forwarding to 192.168.1.2 and port 6000 and url is
 * http://3700.netgear.com, then we use the PC (192.168.1.3 or 192.168.1.2) to open
 * url and enter http://3700.netgear.com:6000 to access the service on 192.168.1.2.
 */
static unsigned int ipt_dnatp2p_target(struct sk_buff **pskb,
		const struct net_device *in,
		const struct net_device *out,
		unsigned int hooknum,
		const void *targinfo,
		void *userinfo)
{
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;
	const struct ip_nat_multi_range_compat *mr = targinfo;

	IP_NF_ASSERT(hooknum == NF_IP_PRE_ROUTING);

	ct = ip_conntrack_get(*pskb, &ctinfo);

	/* Connection must be valid and new. */
	IP_NF_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED));

/*
 * To support Loopback mode, we implement new target called 'DNATP2P' which co-works with 'HAIRPIN'.
 * It works like 'DNAT', such as adding a Port Forwarding item "FTP 20 21 192.168.1.13", if loopback
 * mode supporting, then the conntack SHOULD be as bellowing (WAN: 172.17.145.38; LAN: 192.168.1.2):
 *
 * tcp ESTABLISHED
 * [ORIGINAL] src=192.168.1.2  dst=172.17.145.38 sport=1155 dport=21
 * [REPLY]    src=192.168.1.13 dst=172.17.145.38 sport=21 dport=1155
 *
 * It means using 'DNATP2P' to change the destination IP to the Forwarded-LAN-Server (called 'DNAT')
 * firstly, then this conntrack has 'IPS_SNATP2P_DST_DONE_BIT' bit set; after that, the 'HAIRPIN' will
 * change the source IP to the Router-WAN-Side-IP (called 'SNAT'). These actions make the packets look
 * like coming from Internet side.
 */
	ct->status |= IPS_SNATP2P_DST;

	return ip_nat_setup_info(ct, &mr->range[0], hooknum);
}

static int ipt_dnatp2p_checkentry(const char *tablename,
		const struct ipt_entry *e,
		void *targinfo,
		unsigned int targinfosize,
		unsigned int hook_mask)
{
	struct ip_nat_multi_range_compat *mr = targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		printk("DNATP2P: multiple ranges no longer supported\n");
		return 0;
	}

	if (targinfosize != IPT_ALIGN(sizeof(struct ip_nat_multi_range_compat))) {
		DEBUGP("DNATP2P: Target size %u wrong for %u ranges\n",
			targinfosize, mr->rangesize);
		return 0;
	}

	/* Only allow these for NAT. */
	if (strcmp(tablename, "nat") != 0) {
		DEBUGP("DNATP2P: wrong table %s\n", tablename);
		return 0;
	}

	if (hook_mask & ~(1 << NF_IP_PRE_ROUTING)) {
		DEBUGP("DNATP2P: hook mask 0x%x bad\n", hook_mask);
		return 0;
	}

	return 1;
}

static struct ipt_target ipt_dnatp2p_reg = {
	.name		= "DNATP2P",
	.target		= ipt_dnatp2p_target,
	.checkentry	= ipt_dnatp2p_checkentry,
};

static int __init init(void)
{
	if (ipt_register_target(&ipt_dnatp2p_reg))
		return -EINVAL;

	return 0;
}

static void __exit fini(void)
{
	ipt_unregister_target(&ipt_dnatp2p_reg);
}

module_init(init);
module_exit(fini);

