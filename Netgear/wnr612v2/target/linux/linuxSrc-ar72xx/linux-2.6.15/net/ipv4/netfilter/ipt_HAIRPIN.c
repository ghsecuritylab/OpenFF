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

static unsigned int ipt_hairpin_target(struct sk_buff **pskb,
		const struct net_device *in,
		const struct net_device *out,
		unsigned int hooknum,
		const void *targinfo,
		void *userinfo)
{
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;
	const struct ip_nat_multi_range_compat *mr = targinfo;

	IP_NF_ASSERT(hooknum == NF_IP_POST_ROUTING);

	ct = ip_conntrack_get(*pskb, &ctinfo);

	/* Connection must be valid and new. */
	IP_NF_ASSERT(ct && (ctinfo == IP_CT_NEW || ctinfo == IP_CT_RELATED
				|| ctinfo == IP_CT_RELATED + IP_CT_IS_REPLY));
	IP_NF_ASSERT(out);

	/*
	* RFC 4787 NAT UDP Unicast Requirements
	* 6.  Hairpinning Behavior
	*
	*   If two hosts (called X1 and X2) are behind the same NAT and exchanging traffic, the
	* NAT may allocate an address on the outside of the NAT for X2, called X2':x2'.  If X1
	* sends traffic to X2':x2', it goes to the NAT, which must relay the traffic from X1 to
	* X2.  This is referred to as hairpinning and is illustrated below.
	*
	*     NAT
	*   +----+ from X1:x1 to X2':x2'   +-----+ X1':x1'
	*   | X1 |>>>>>>>>>>>>>>>>>>>>>>>>>>>>>--+---
	*   +----+                         |  v  |
	*                                  |  v  |
	*                                  |  v  |
	*                                  |  v  |
	*   +----+ from X1':x1' to X2:x2   |  v  | X2':x2'
	*   | X2 |<<<<<<<<<<<<<<<<<<<<<<<<<<<<<--+---
	*   +----+                         +-----+
	*
	*             Hairpinning Behavior
	*
	*   Hairpinning allows two endpoints on the internal side of the NAT to communicate even if
	* they only use each other's external IP addresses and ports.
	*
	*   Original Hairpinning implementation is with two rules added to 'POSTROUTING' chain of 'nat'
	* table:
	*
	* Chain POSTROUTING (policy ACCEPT 2 packets, 130 bytes)
	*  pkts bytes target     prot opt in     out     source               destination
	*     0     0 br0_masq   all  --  *      br0     0.0.0.0/0            0.0.0.0/0
	*
	* Chain br0_masq (1 references)
	*  pkts bytes target     prot opt in     out     source               destination
	*     0     0 ACCEPT     all  --  *      *       192.168.1.1          0.0.0.0/0
	*     0     0 SNATP2P    all  --  *      *       192.168.1.0/24       0.0.0.0/0           to:1.1.1.9
	*
	* This design will conflict with LAN static route:
	* [BUG 13984]
	* [SQA Taipei-52][Static Route]If DUT WAN port obtains an IP address, the static route for LAN side
	* is notworkable. DUT will modify the source IP address as its WAN address when traffic is forwarding
	* to the destination and therefore the other side will not be able to transmit packets back.
	*
	* For Hairpinned conntrack, it SHOULD be done 'DNAT' in 'ip_nat_rule_find'/'find_appropriate_p2p_dst';
	* If successfuly, it will be set to '1' on 'IPS_SNATP2P_DST_DONE_BIT', an NAT mapping MUST be existed.
	* But for normal traffic such as forwarded as 'static route', its conntrack doesn't have this bit set.
	*
	* To resolve the conflicting between 'Hairpinning' and 'LAN side Static Route', we implement new target
	* called 'HAIRPIN' which co-works with 'SNATP2P'. It is very easily understandable. 
	*/
	if (!test_bit(IPS_SNATP2P_DST_DONE_BIT, &ct->status))
		return IPT_CONTINUE;

	ct->status |= IPS_SNATP2P_SRC;

	return ip_nat_setup_info(ct, &mr->range[0], hooknum);
}

static int ipt_hairpin_checkentry(const char *tablename,
		const struct ipt_entry *e,
		void *targinfo,
		unsigned int targinfosize,
		unsigned int hook_mask)
{
	struct ip_nat_multi_range_compat *mr = targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		printk("HAIRPIN: multiple ranges no longer supported\n");
		return 0;
	}

	if (targinfosize != IPT_ALIGN(sizeof(struct ip_nat_multi_range_compat)))
	{
		DEBUGP("HAIRPIN: Target size %u wrong for %u ranges\n",
			targinfosize, mr->rangesize);
		return 0;
	}

	/* Only allow these for NAT. */
	if (strcmp(tablename, "nat") != 0) {
		DEBUGP("HAIRPIN: wrong table %s\n", tablename);
		return 0;
	}

	if (hook_mask & ~(1 << NF_IP_POST_ROUTING)) {
		DEBUGP("HAIRPIN: hook mask 0x%x bad\n", hook_mask);
		return 0;
	}

	return 1;
}

static struct ipt_target ipt_hairpin_reg = {
	.name		= "HAIRPIN",
	.target		= ipt_hairpin_target,
	.checkentry	= ipt_hairpin_checkentry,
};

static int __init init(void)
{
	if (ipt_register_target(&ipt_hairpin_reg))
		return -EINVAL;

	return 0;
}

static void __exit fini(void)
{
	ipt_unregister_target(&ipt_hairpin_reg);
}

module_init(init);
module_exit(fini);
