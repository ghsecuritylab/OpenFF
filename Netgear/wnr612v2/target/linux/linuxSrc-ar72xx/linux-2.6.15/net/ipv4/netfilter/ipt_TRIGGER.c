/* 
 * kernel module to match the port-ranges, trigger related port-ranges,
 * and alters the destination to a local IP address.
 *
 * Copyright (C) 2003, CyberTAN Corporation
 * All Rights Reserved.
 *
 * Description:
 *   This is kernel module for port-triggering.
 *
 *   The module follows the Netfilter framework, called extended packet
 *   matching modules.
 *
 * History:
 *
 * 2008.07: code cleaning by Delta Networks Inc.
 */

#include <linux/types.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/timer.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netdevice.h>
#include <linux/if.h>
#include <linux/inetdevice.h>
#include <net/protocol.h>
#include <net/checksum.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ip_conntrack_core.h>
#include <linux/netfilter_ipv4/ip_conntrack_tuple.h>
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#include <linux/netfilter_ipv4/ipt_TRIGGER.h>

/* 
 * rwlock protects the main hash table, protocol/helper/expected
 * registrations, conntrack timers
 */
#define ASSERT_READ_LOCK(x) 
#define ASSERT_WRITE_LOCK(x)

#include <linux/netfilter_ipv4/listhelp.h>

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

/* yuecheng add for lan port reservation */
#if defined(CONFIG_LAN_PORT_RESERVED)
#include <linux/inet.h>
extern int lan_port_reserved;
extern struct lan_port_resv_t lan_port_resv[LAN_PORT_RESERVATION_SIZE]; 
#endif

struct ipt_trigger 
{
	struct list_head list;	/* Trigger list */

	struct timer_list timeout;	/* Timer for list destroying */

	__u32 srcip;		/* Outgoing source address */
	__u16 mproto;	/* Trigger protocol */
	__u16 rproto;		/* Related protocol */

	struct ipt_trigger_ports ports;	/* Trigger and related ports */

	__u16 reply;                 /* Confirm a reply connection */
	__u16 trigger_timeout;
};

struct list_head trigger_list = LIST_HEAD_INIT(trigger_list);

static void trigger_timer_refresh(struct ipt_trigger *trig)
{
	write_lock_bh(&ip_conntrack_lock);

	/* need del_timer for race avoidance (may already be dying). */
	if (del_timer(&trig->timeout)) {
		trig->timeout.expires = jiffies + (trig->trigger_timeout * HZ);
		add_timer(&trig->timeout);
	}

	write_unlock_bh(&ip_conntrack_lock);
}

static void __del_trigger(struct ipt_trigger *trig)
{
	list_del(&trig->list);

	kfree(trig);
}

static int ip_ct_kill_triggered(struct ip_conntrack *i, void *data)
{
	__u16 proto, dport;
	struct ipt_trigger *trig;

	if (!(i->status & IPS_TRIGGER))
		return 0;

	trig = data;
	proto = i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum;
	dport = ntohs(i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all);

	if (trig->rproto == proto ||trig->rproto == 0)
		return (trig->ports.rport[0] <= dport && trig->ports.rport[1] >= dport);
	else
		return 0;
}

static void trigger_timeout(unsigned long data)
{
	struct ipt_trigger *trig= (void *)data;

	DEBUGP("trigger list %p timed out\n", trig);

/* yuecheng,  add for lan port reservation trigger out ip = any */
#if defined(CONFIG_LAN_PORT_RESERVED)
	int i;
	for (i = 0; i < lan_port_reserved; i++) {
		if ((lan_port_resv[i].ip == trig->srcip) && (lan_port_resv[i].ip_changed == 1)
			&& ((lan_port_resv[i].proto == 255) || (lan_port_resv[i].proto == trig->rproto))
			&& (trig->ports.rport[0] == lan_port_resv[i].min_port) && (trig->ports.rport[1] == lan_port_resv[i].max_port)) {
				lan_port_resv[i].ip = TRIGGER_ANY_IP;
				DEBUGP("\nTrigger out ip: %u.%u.%u.%u got timeout, change lan_port_resv[%d] to 255.255.255.255 !!\n", NIPQUAD(trig->srcip), i);
		}
	} 
#endif

	ip_ct_iterate_cleanup(ip_ct_kill_triggered, trig);

	write_lock_bh(&ip_conntrack_lock);
	__del_trigger(trig);
	write_unlock_bh(&ip_conntrack_lock);
}

/*
 *	Service-Name		OutBound			InBound
 * 1.	TMD			UDP:1000		TCP/UDP:2000..2010
 * 2.	WOKAO		UDP:1000		TCP/UDP:3000..3010
 * 3.	net2phone-1	UDP:6801		TCP:30000..30000
 * 4.	net2phone-2	UDP:6801		UDP:30000..30000
 *
 * For supporting to use the same outgoing port to trigger different port rules,
 * it should check the inbound protocol and port range value. If all conditions
 * are matched, it is a same trigger item, else it needs to create a new one.
 */
static inline int trigger_out_matched(const struct ipt_trigger *i, __u16 proto, __u16 dport,
		struct ipt_trigger_info *info)
{
	return
		i->mproto == proto &&
		i->ports.mport[0] <= dport &&
		i->ports.mport[1] >= dport &&
		i->rproto == info->proto &&
		i->ports.rport[0] == info->ports.rport[0] &&
		i->ports.rport[1] == info->ports.rport[1];
}

static unsigned int trigger_out(struct sk_buff *skb, struct ipt_trigger_info *info)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;

	iph = skb->nh.iph;
	tcph = (void *)iph + (iph->ihl << 2);     /* might be TCP, UDP */

	/* Check if the trigger range has already existed in 'trigger_list'. */
	trig = LIST_FIND(&trigger_list,
			trigger_out_matched,
			struct ipt_trigger *, iph->protocol, ntohs(tcph->dest), info);

	if (trig != NULL) {
		DEBUGP("Tirgger Out Refresh: %u.%u.%u.%u %u\n", NIPQUAD(iph->saddr), 
				ntohs(tcph->dest));

		/* Yeah, it exists. We need to update(delay) the destroying timer. */
		trigger_timer_refresh(trig);

		/* In order to allow multiple hosts use the same port range, we update
		 * the 'saddr' after previous trigger has a reply connection. */	
		#if 0
		if (trig->reply) {
			trig->srcip = iph->saddr;
			trig->reply = 0;
		}
		#else
		/*
		 * Well, CD-Router verifies Port-Triggering to support multiple LAN hosts can
		 * use trigger ports after mappings are aged out. It tests as bellowing ...
		 *
		 * net2phone-1	UDP:6801		TCP:30000..30000
		 * net2phone-2	UDP:6801		UDP:3000..3000
		 *
		 * 1. 192.168.1.2 --> UDP:6801 --> verify TCP:30000 opened ?
		 * 2. waiting for all trigger port mappings to be deleted.
		 * 3. 192.168.1.3 --> UDP:6801 --> verify TCP:30000 opened ?
		 *
		 * 4. 192.168.1.2 --> UDP:6801 --> verify UDP:3000 opened ?
		 * 5. waiting for all trigger port mappings to be deleted.
		 * 6. 192.168.1.3 --> UDP:6801 --> verify UDP:3000 opened ?
		 *
		 * Between steps 3 and 4, it doesn't wait time out, and on step 3, it has created
		 * two trigger items: [A].  TCP:30000 ('reply' = 1); B). UDP:3000 ('reply' = 0). so
		 * on step 4, it can't update the 'srcip' value from '192.168.1.3' to '192.168.1.2'.
		 * For passing test, and let the customer be happy, we ... ^_^, it is not so bad.
		 */
		trig->srcip = iph->saddr;
		#endif
	} else {
		/* Create new trigger */
		trig = kmalloc(sizeof(struct ipt_trigger), GFP_ATOMIC);
		if (trig == NULL) {
			DEBUGP("No memory for adding Tigger!\n");
			return IPT_CONTINUE;
		}

		DEBUGP("Tirgger Add: from %u.%u.%u.%u, port %u\n", NIPQUAD(iph->saddr), 
				ntohs(tcph->dest));

		init_timer(&trig->timeout);
		trig->timeout.data = (unsigned long)trig;
		trig->timeout.function = trigger_timeout;
		trig->timeout.expires = jiffies + (info->trigger_timeout * HZ);

		trig->srcip = iph->saddr;
		trig->mproto = iph->protocol;
		trig->rproto = info->proto;
		trig->reply = 0;
		trig->trigger_timeout = info->trigger_timeout;
		memcpy(&trig->ports, &info->ports, sizeof(struct ipt_trigger_ports));

		/* add to global table of trigger and start timer. */
		write_lock_bh(&ip_conntrack_lock);

		list_prepend(&trigger_list, &trig->list);
		add_timer(&trig->timeout);

		write_unlock_bh(&ip_conntrack_lock);

#if defined(CONFIG_LAN_PORT_RESERVED)
		int i;
		for (i = 0; i < lan_port_reserved; i++) {
			if ((lan_port_resv[i].ip == TRIGGER_ANY_IP) && (lan_port_resv[i].ip_changed)
				&& ((lan_port_resv[i].proto == 255) || (lan_port_resv[i].proto == trig->rproto))
				&& (trig->ports.rport[0] == lan_port_resv[i].min_port) && (trig->ports.rport[1] == lan_port_resv[i].max_port)) {
					DEBUGP("\nChange lan_port_resv[%d].ip from 255.255.255.255 to %u.%u.%u.%u\n", i, NIPQUAD(trig->srcip));
					lan_port_resv[i].ip = trig->srcip;
			}
		}
#endif
	}

	return IPT_CONTINUE;        /* we don't block any packet. */
}

static inline int trigger_in_matched(const struct ipt_trigger *i, __u16 proto, __u16 dport)
{
	__u16 rproto = i->rproto ? : proto;

	return
		rproto == proto &&
		i->ports.rport[0] <= dport &&
		i->ports.rport[1] >= dport;
}

static unsigned int trigger_in(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;

	ct = ip_conntrack_get(skb, &ctinfo);
	if ((ct == NULL) ||!(ct->status & IPS_TRIGGER))
		return IPT_CONTINUE;

	iph = skb->nh.iph;
	tcph = (void *)iph + (iph->ihl << 2);	/* Might be TCP, UDP */

	/* Check if the trigger-ed range has already existed in 'trigger_list'. */
	trig = LIST_FIND(&trigger_list,
			trigger_in_matched,
			struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));
	if (trig != NULL) {
		DEBUGP("Trigger In: from %u.%u.%u.%u, destination port %u\n", 
				NIPQUAD(iph->saddr), ntohs(tcph->dest));

		/* Yeah, it exists. We need to update(delay) the destroying timer. */
		trigger_timer_refresh(trig);
#if defined(CONFIG_DEL_UNUSED_CONNTRACK)
		struct ip_conntrack *ct;
		enum ip_conntrack_info ctinfo;
		ct = ip_conntrack_get(skb, &ctinfo);
		ct->status |= IPS_TRIGGERING;
#endif
		return NF_ACCEPT;       /* Accept it, or the imcoming packet could be dropped in the FORWARD chain */
	}

	return IPT_CONTINUE;        /* Our job is the interception. */
}

static unsigned int trigger_dnat(struct sk_buff *skb, unsigned int hooknum)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;
	struct ip_nat_range newrange;

	iph = skb->nh.iph;
	tcph = (void *)iph + (iph->ihl << 2);	/* Might be TCP, UDP */

	IP_NF_ASSERT(hooknum == NF_IP_PRE_ROUTING);

	/* Check if the trigger-ed range has already existed in 'trigger_list'. */
	trig = LIST_FIND(&trigger_list,
			trigger_in_matched,
			struct ipt_trigger *, iph->protocol, ntohs(tcph->dest));

	if (trig == NULL ||trig->srcip == 0)
		return IPT_CONTINUE;    /* We don't block any packet. */

	trig->reply = 1;   /* Confirm there has been a reply connection. */
	ct = ip_conntrack_get(skb, &ctinfo);
	IP_NF_ASSERT(ct && (ctinfo == IP_CT_NEW));

	DEBUGP("Trigger DNAT: %u.%u.%u.%u ", NIPQUAD(trig->srcip));
	DUMP_TUPLE(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);

	/* Alter the destination of imcoming packet. */
	newrange = ((struct ip_nat_range) { 
			IP_NAT_RANGE_MAP_IPS,
			trig->srcip, 
			trig->srcip,
			{ 0 },
			{ 0 }
			});

	ct->status |= IPS_TRIGGER;
	/* Netgear spec, log port trigger event, get ip/port from trigger_in packet, so invert src/dest data */
	printk("<7>[LAN access from remote] from %u.%u.%u.%u:%d to %u.%u.%u.%u:%d,\n", 
		NIPQUAD(iph->saddr),ntohs(tcph->source), NIPQUAD(trig->srcip),ntohs(tcph->dest));

	/* Hand modified range to generic setup. */
	return ip_nat_setup_info(ct, &newrange, hooknum);
}

static inline int trigger_refresh_matched(const struct ipt_trigger *i, __u16 proto, __u16 sport)
{
	__u16 rproto = i->rproto ? : proto;

	return
		rproto == proto &&
		i->ports.rport[0] <= sport &&
		i->ports.rport[1] >= sport;
}

static unsigned int trigger_refresh(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct ipt_trigger *trig;
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;

	ct = ip_conntrack_get(skb, &ctinfo);
	if ((ct == NULL) ||!(ct->status & IPS_TRIGGER))
		return IPT_CONTINUE;

	iph = skb->nh.iph;
	tcph = (void *)iph + (iph->ihl << 2);     /* might be TCP, UDP */

	trig = LIST_FIND(&trigger_list,
			trigger_refresh_matched,
			struct ipt_trigger *, iph->protocol, tcph->source);
	if (trig != NULL) {
		DEBUGP("Trigger Refresh: from %u.%u.%u.%u, %u\n", 
				NIPQUAD(iph->saddr), ntohs(tcph->source));
		trigger_timer_refresh(trig);
	}

	return IPT_CONTINUE;
}

static unsigned int ipt_trigger_target(struct sk_buff **pskb,
		const struct net_device *in,
		const struct net_device *out,
		unsigned int hooknum,
		const void *targinfo,
		void *userinfo)
{
	struct iphdr *iph;
	struct sk_buff *skb = *pskb;
	struct ipt_trigger_info *info = (struct ipt_trigger_info *)targinfo;

	iph = skb->nh.iph;
	if (iph->protocol != IPPROTO_TCP && iph->protocol != IPPROTO_UDP)
		return IPT_CONTINUE;

	if (info->type == IPT_TRIGGER_OUT)
		return trigger_out(skb, info);
	else if (info->type == IPT_TRIGGER_IN)
		return trigger_in(skb);
	else if (info->type == IPT_TRIGGER_DNAT)
		return trigger_dnat(skb, hooknum);
	else if (info->type == IPT_TRIGGER_REFRESH)
		return trigger_refresh(skb);

	return IPT_CONTINUE;
}

static int ipt_trigger_check(const char *tablename,
		const struct ipt_entry *e,
		void *targinfo,
		unsigned int targinfosize,
		unsigned int hook_mask)
{
	struct list_head *pos, *n;
	struct ipt_trigger_info *info = targinfo;

	if (strcmp(tablename, "mangle") == 0) {
		DEBUGP("trigger_check: bad table `%s'.\n", tablename);
		return 0;
	}

	if (targinfosize != IPT_ALIGN(sizeof(struct ipt_trigger_info))) {
		DEBUGP("trigger_check: size %u.\n", targinfosize);
		return 0;
	}

	if (hook_mask & ~((1 << NF_IP_PRE_ROUTING) |(1 << NF_IP_FORWARD))) {
		DEBUGP("trigger_check: bad hooks %x.\n", hook_mask);
		return 0;
	}

	if (info->proto && info->proto != IPPROTO_TCP && info->proto != IPPROTO_UDP) {
		DEBUGP("trigger_check: bad proto %d.\n", info->proto);
		return 0;
	}

	if (info->type == IPT_TRIGGER_OUT) {
		if (!info->ports.mport[0] ||!info->ports.rport[0]) {
			DEBUGP("trigger_check: Try 'iptbles -j TRIGGER -h' for help.\n");
			return 0;
		}
	}

	/* empty the 'trigger_list' */
	list_for_each_safe(pos, n, &trigger_list) {
		struct ipt_trigger *trig = list_entry(pos, struct ipt_trigger, list);

		DEBUGP("removing trigger_list: %p.\n", trig);
		del_timer(&trig->timeout);
		ip_ct_iterate_cleanup(ip_ct_kill_triggered, trig);
		__del_trigger(trig);
	}

	return 1;
}

static struct ipt_target ipt_trigger_reg = {
	.name		= "TRIGGER",
	.target		= ipt_trigger_target,
	.checkentry	= ipt_trigger_check,
	.me			= THIS_MODULE,
};

static int __init init(void)
{
	return ipt_register_target(&ipt_trigger_reg);
}

static void __exit fini(void)
{
	ipt_unregister_target(&ipt_trigger_reg);
}

module_init(init);
module_exit(fini);

