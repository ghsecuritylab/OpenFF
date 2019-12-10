/* Kernel module to match packet ingress port. */
/* (C) 1999-2001 James Morris <jmorros@intercode.com.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter_ipv4/ipt_ingressport.h>
#include <linux/netfilter_ipv4/ip_tables.h>

MODULE_AUTHOR("ALEX <alex.yc.chang@delta.com.tw>");
MODULE_DESCRIPTION("IP tables ingressport matching module");
MODULE_LICENSE("GPL");

static int
match(const struct sk_buff *skb,
      const struct net_device *in,
      const struct net_device *out,
      const void *matchinfo,
      int offset,
	  const void *hdr,
      u_int16_t datalen,
      int *hotdrop)
{
	const struct ipt_ingressport_info *info = matchinfo;

	/* LAN1=0x01, LAN2=0x02, LAN3=0x03, LAN4=0x04, WAN=0x00 */
	if (skb->phy_ingress_port != info->port_num){
		return 0;
	}
	else
	{
		return 1;
	}
}

static int
checkentry(const char *tablename,
           const struct ipt_ip *ip,
           void *matchinfo,
           unsigned int matchsize,
           unsigned int hook_mask)
{
	if (matchsize != IPT_ALIGN(sizeof(struct ipt_ingressport_info)))
		return 0;

	return 1;
}

static struct ipt_match ingressport_match = {
	.name		= "ingressport",
	.match		= &match,
	.checkentry	= &checkentry,
	.me		= THIS_MODULE,
	
};

static int __init init(void)
{
	return ipt_register_match(&ingressport_match);
}

static void __exit fini(void)
{
	ipt_unregister_match(&ingressport_match);
}

module_init(init);
module_exit(fini);
