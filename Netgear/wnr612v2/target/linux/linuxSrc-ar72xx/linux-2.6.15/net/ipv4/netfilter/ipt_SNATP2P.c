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

static unsigned int ipt_snatp2p_target(struct sk_buff **pskb,
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
	ct->status |= IPS_SNATP2P_SRC;

	return ip_nat_setup_info(ct, &mr->range[0], hooknum);
}

static int ipt_snatp2p_checkentry(const char *tablename,
		const struct ipt_entry *e,
		void *targinfo,
		unsigned int targinfosize,
		unsigned int hook_mask)
{
	struct ip_nat_multi_range_compat *mr = targinfo;

	/* Must be a valid range */
	if (mr->rangesize != 1) {
		printk("SNATP2P: multiple ranges no longer supported\n");
		return 0;
	}

	if (targinfosize != IPT_ALIGN(sizeof(struct ip_nat_multi_range_compat)))
	{
		DEBUGP("SNATP2P: Target size %u wrong for %u ranges\n",
			targinfosize, mr->rangesize);
		return 0;
	}

	/* Only allow these for NAT. */
	if (strcmp(tablename, "nat") != 0) {
		DEBUGP("SNATP2P: wrong table %s\n", tablename);
		return 0;
	}

	if (hook_mask & ~(1 << NF_IP_POST_ROUTING)) {
		DEBUGP("SNATP2P: hook mask 0x%x bad\n", hook_mask);
		return 0;
	}

	return 1;
}

static struct ipt_target ipt_snatp2p_reg = {
	.name		= "SNATP2P",
	.target		= ipt_snatp2p_target,
	.checkentry	= ipt_snatp2p_checkentry,
};

#if defined(CONFIG_IP_NF_CT_NAT_SESSION)

#define sin_addr(s) (((struct sockaddr_in *)(s))->sin_addr.s_addr)

struct routeinfo
{
	struct list_head list;

	__u32 netdest;
	__u32 netmask;
};

static struct list_head routeinfo_head = LIST_HEAD_INIT(routeinfo_head);

static int route_cmp(struct ip_conntrack *i, void *ptr)
{
	struct routeinfo *infop = (struct routeinfo *) ptr;

	return infop->netdest ==
		(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.ip & infop->netmask);
}

static void route_conntrack_tasklet(unsigned long data)
{
	int flush;
	unsigned long flags;
	struct routeinfo *infop;
	struct list_head *head;

	flush = 0;
	head  = (struct list_head *) data;

	local_irq_save(flags);
	while (!list_empty(head)) {
		infop = list_entry(head->next, struct routeinfo, list);
		list_del(head->next);
		local_irq_restore(flags);

		flush = 1;
		ip_ct_iterate_cleanup(route_cmp, (void *)infop);

		kfree(infop);

		local_irq_save(flags);
	}
	local_irq_restore(flags);

	if (flush)
		rt_cache_flush(0);
}

static struct tasklet_struct route_tasklet  = {
	.next	= NULL,
	.state	= 0,
	.count	= ATOMIC_INIT(0),
	.func		= route_conntrack_tasklet,
	.data	= (unsigned long) &routeinfo_head,
};

/*
  * If the specific destination route is changed(Add or Delete), remove the old
  * conntrack and flush the route cache.
  */
void inet_route_event(unsigned int cmd, struct rtentry *rt)
{
	__u32 dest;
	__u32 netmask;
	__u32 gateway;
	unsigned long flags;
	struct routeinfo *infop;

	dest = sin_addr(&rt->rt_dst);
	netmask = sin_addr(&rt->rt_genmask);
	gateway = sin_addr(&rt->rt_gateway);
	if (dest == 0 ||netmask == 0 ||gateway == 0)
		return;

	DEBUGP("\nSNATP2P: %s %u.%u.%u.%u/%u.%u.%u.%u with GW %u.%u.%u.%u\n",
			cmd == SIOCADDRT ? "adding" : "deleting",
			NIPQUAD(dest), NIPQUAD(netmask), NIPQUAD(gateway));

	infop = kmalloc(sizeof(struct routeinfo), GFP_ATOMIC);
	if (infop == NULL)
		return;
	infop->netdest = dest & netmask;
	infop->netmask = netmask;

	local_irq_save(flags);
	list_add_tail(&infop->list, &routeinfo_head);
	local_irq_restore(flags);

	tasklet_schedule(&route_tasklet);
}

/************************************************************************/

struct inetinfo
{
	struct list_head list;

	__u32 ip;

	char dev[IFNAMSIZ];
};

static struct list_head inetinfo_head = LIST_HEAD_INIT(inetinfo_head);

static void del_inetinfo(char *dev, __u32 ipaddr)
{
	unsigned long flags;
	struct list_head *pos;
	struct inetinfo *infop;

	infop = NULL;

	local_irq_save(flags);
	list_for_each(pos, &inetinfo_head) {
		struct inetinfo *info = list_entry(pos, struct inetinfo, list);
		if (strcmp(dev, info->dev) == 0 && ipaddr == info->ip) {
			list_del(pos);
			infop = info;
			break;
		}
	}
	local_irq_restore(flags);

	kfree(infop); /* `kfree' does nothing if @infop is NULL */
}

static void add_inetinfo(char *dev, __u32 ipaddr)
{
	unsigned long flags;
	struct list_head *pos;
	struct inetinfo *infop, *dup;

	/* Duplicate items can happen ? Do some optimization. */
	infop = kmalloc(sizeof(struct inetinfo), GFP_ATOMIC);
	if (infop == NULL)
		return;
	infop->ip = ipaddr;
	strcpy(infop->dev, dev);

	dup = NULL;
	local_irq_save(flags);
	list_for_each(pos, &inetinfo_head) {
		struct inetinfo *inp = list_entry(pos, struct inetinfo, list);
		if (strcmp(dev, inp->dev) == 0 && ipaddr == inp->ip) {
			dup = inp;
			break;
		}
	}
	if (likely(dup == NULL))
		list_add_tail(&infop->list, &inetinfo_head);
	local_irq_restore(flags);

	if (unlikely(dup != NULL))
		kfree(infop);
}

/*
 * This routine removes three kind of conntrack(s) by comparing the down IP address, 
 * the key is to find the Router's WAN address (called RT.IP) of conntrack.
 *
 * @ conntrack is initiated from LAN hosts to Internet (IPS_SRC_NAT).
 *     RT.IP is the destination address from reply direction.
 *
 * @ conntrack is initiated from Internet to LAN hosts (IPS_DST_NAT).
 *     RT.IP is the destination address from original direction.
 *
 * @ conntrack is initiated from Router to Internet.
 *     RT.IP `MAY' be the source address of original direction with no mark.
 *
 * PS: this solution is NOT so perfect ......
 */
static int internet_cmp(struct ip_conntrack *i, void *ptr)
{
	__u32 ipaddr;

	if (i->status & IPS_SRC_NAT)
		ipaddr = i->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip;
	else if (i->status & IPS_DST_NAT)
		ipaddr = i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.ip;
	else
		ipaddr = i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;

	return ipaddr == (__u32)ptr;
}

/*
 * If the NAT table's rules information have be updated, and the Router's
 * IP address has been down, remove the invalid internet conntrack.
 */
static void internet_conntrack_tasklet(unsigned long data)
{
	unsigned long flags;
	struct inetinfo *infop;
	struct list_head *head;

	head  = (struct list_head *) data;

	local_irq_save(flags);

	while (!list_empty(head)) {
		infop = list_entry(head->next, struct inetinfo, list);
		list_del(head->next);
		local_irq_restore(flags);

		ip_ct_iterate_cleanup(internet_cmp, (void *)infop->ip);

		kfree(infop);

		local_irq_save(flags);
	}

	local_irq_restore(flags);
}

struct tasklet_struct nat_tasklet  = {
	.next	= NULL,
	.state	= 0,
	.count	= ATOMIC_INIT(0),
	.func	= internet_conntrack_tasklet,
	.data	= (unsigned long) &inetinfo_head,
};

/*
 * when setting the IP value to the interface, the bellow message is traced:
 *
 * A). configure the interface `ppp0' with IP address 1.1.1.7 by `pppd'
 * 	  1). ppp0 1.1.1.7 is up.
 *      2). ppp0 1.1.1.7 is down.
 *      3). ppp0 1.1.1.7 is up.
 *
 * B). onfigure the interface `eth1' with IP address 172.17.144.103 by `udhcpc'
 *      1). eth1 172.17.144.103 is up.
 *      2). eth1 172.17.144.103 is down.
 *      3). eth1 172.17.144.103 is up.
 *
 * To trace the down IP address correctly, if 'up', remove the IP, if 'down', add it.
 */
static int snatp2p_inet_event(struct notifier_block *this,
		unsigned long event,
		void *ptr)
{
	struct in_ifaddr *ina = ptr;

	if (event == NETDEV_DOWN)
		add_inetinfo(ina->ifa_label, ina->ifa_local);
	else if (event == NETDEV_UP)
		del_inetinfo(ina->ifa_label, ina->ifa_local);

	return NOTIFY_DONE;
}

static struct notifier_block snatp2p_inet_notifier = {
	.notifier_call	= snatp2p_inet_event,
};

#endif

static int __init init(void)
{
	if (ipt_register_target(&ipt_snatp2p_reg))
		return -EINVAL;

#if defined(CONFIG_IP_NF_CT_NAT_SESSION)
	register_inetaddr_notifier(&snatp2p_inet_notifier);
#endif
	return 0;
}

static void __exit fini(void)
{
#if defined(CONFIG_IP_NF_CT_NAT_SESSION)
	unregister_inetaddr_notifier(&snatp2p_inet_notifier);
#endif
	ipt_unregister_target(&ipt_snatp2p_reg);
}

module_init(init);
module_exit(fini);
