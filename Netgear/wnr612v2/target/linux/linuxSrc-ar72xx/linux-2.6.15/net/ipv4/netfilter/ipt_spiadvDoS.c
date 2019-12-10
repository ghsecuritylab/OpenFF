/* 
  * Advanced SPI Firewall/DoS TCP Protection Implementation in NETFILTER match.
  * According to 'Home Wireless Router Engineer Spec v1.6'
  *
  * 			Copyright (C) 2008 Delta Networks Inc.
  */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/icmp.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ip_conntrack_core.h>
#include <linux/netfilter_ipv4/ip_conntrack_tuple.h>
#include <linux/netfilter_ipv4/ip_conntrack_protocol.h>

/********************************************************************
  *
  * 2.2 SPI Firewall/DoS Protection
  *   (9) SYN Flood Attack
  *   SYN Flood is that an attacker sends lots of TCP SYN packets with same or different
  * destination ports to the victim, and then attacker won't send an ACK to the victim to
  * complete the TCP 3-way handshaking even the attacker has received the SYN+ACK
  * from the victim.
  *   This will cause lots of TCP pending sessions on the attacked host and cause victim
  * to run out of all the resource.
  *
  *   A Router implementation MUST monitor the current TCP sessions to the router locally
  * and to the exposed LAN hosts.
  *
  * ----------------[NETGEAR CEC -- Defect ID: 731]--------------------
  *  Status: Open
  *  Subject: Security (110)
  *  Summary: DUT can't block TCP SYN scan packets correctly.
  *  Reproducible: Y
  *  Severity: 2-Medium
  *  Detected By: twang 
  *  Detected on Date: 2007-10-21
  *  Detected in Version: WPN824v3_v1.0.6_1.0.7NA
  *  Description:
  *  		Make a stream with Ixia, 
  *  		TCP SYN packet
  *		The destination IP should be WAN IP address
  *		The destination port should be increased 1000 times, from 1 to 1000
  *			size 64 byte, 500 packets/s
  *		Set DMZ to 192.168.1.3 in DUT WAN side.
  *		
  *		When start traffic, capture from the port 192.168.1.3.
  *
  *  The expected result:
  *		DUT can allow the first 200 packets.
  *  The actual result:
  *  		All the packets can pass through the DUT.
  * -------------------------------------------------------------
  * 
  * The bellowing implementation doesn't meet the requirement exactly, but MAY
  * be passed the CEC testing, the attack happens not very often, let the normal
  * Internet session NOT be interruptted too much, It SHOULD find a balance of it.
  * The code seems to be complex, but works well, and the performance is still ok.
  * That's OK~ :)
  ********************************************************************/

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

/*
  *   (1). If there are 200 TCP SYN-pending sessions (i.e. there is no corresponding
  * ACK packet received for those sessions) that come from the same sender, no
  * matter these sessions have the same destination port or not, the sender is
  * identified as a malicious host. Note that these sessions may go to the router
  * locally or to the exposed LAN hosts.
  *   After such a malicious host has been identified, all TCP sessions from this host
  * MUST be removed (i.e. the pending TCP sessions on the router locally and the
  * NAT mappings for those TCP sessions to exposed LAN hosts). Besides, the router
  * MUST block the malicious host for 10 minutes and prevent it from initiating a TCP
  * session to the router itself or to the exposed LAN hosts during this time interval.
  */
#define DOS_PENDING_MAX	200
#define DOS_BLOCK_TIME		(10 * 60 * HZ)

/*
  *   (2).  Besides, when there are 300 TCP SYN-pending sessions, even if they are
  * initiated by different sources, an implementation MUST remove a pending session
  * that has idled for more than 5 seconds.
  *
  *   (3). If the number of total TCP SYN-pending sessions is below 600, all other
  * normal hosts on the Internet still can initiate normal TCP sessions to the router
  * itself (e.g. the remote management function) and to the exposed LAN hosts
  * during the attack and after the attack stops.
  */
#define REAP_PENDING_NUM		300
#define REAP_PENDING_IDLETIME	(5 * HZ)

/* 
  *   (4). When the number of total TCP SYN-pending sessions reaches 600, all TCP
  * SYN packets from the Internet MUST be dropped, that is, no TCP session can be
  * created from the Internet under this condition.
  */
#define SYN_BLOCK_ALL_NUM		600


#define TCP_SYN	0x02
#define TCP_ALL	0x3F

#define DOS_HTABLE_SIZE	1024	/* MUST be power of 2 */

struct ipt_spiadvDoS
{
	void *unused;
};

/* The statistic information about the DoS protection. */
struct ipt_dos_stat
{
	struct list_head list;

	__u32 saddr;		/* The address of HOST from Internet. */
	int pending;		/* The number of  TCP pending sessions */

	__u32 blocking;		/* It is in blocking mode */
	unsigned long expires;	/* The blocking expired time */
};

struct ipt_dos_reap
{
	struct list_head list;

	__u32 saddr;
};

static atomic_t ip_conntrack_dos_count = ATOMIC_INIT(0);
static kmem_cache_t *ip_conntrack_dos_cachep;
static struct list_head dos_htable[DOS_HTABLE_SIZE];
static DEFINE_RWLOCK(ip_conntrack_dos_lock);

/* It's monitored if it is. */
static inline int is_monitored(struct ip_conntrack *ct)
{
	return test_bit(IPS_SPI_DoS_BIT, &ct->status);
}

static inline unsigned int ipt_dos_iphash(unsigned int addr)
{
	return (addr ^ (addr >> 16)) & (DOS_HTABLE_SIZE - 1);
}

/************************************************************************/

extern unsigned long ip_ct_tcp_timeout_syn_sent;
extern unsigned long ip_ct_tcp_timeout_syn_recv;

static int reap_pending;
static struct list_head reap_all_head = LIST_HEAD_INIT(reap_all_head);
static DEFINE_RWLOCK(dos_reap_lock);

static int reap_pending_cmp(struct ip_conntrack *i, void *unused)
{
	unsigned long timo;

	if (!is_monitored(i) ||!timer_pending(&i->timeout))
		return 0;

	if (i->proto.tcp.state == TCP_CONNTRACK_SYN_SENT)
		timo = ip_ct_tcp_timeout_syn_sent;
	else if (i->proto.tcp.state == TCP_CONNTRACK_SYN_RECV)
		timo = ip_ct_tcp_timeout_syn_recv;
	else
		return 0;

	return (long)(timo - REAP_PENDING_IDLETIME) > (long)(i->timeout.expires - jiffies);
}

static int reap_all_cmp(struct ip_conntrack *i, void *saddr)
{
	return
		i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP &&
		i->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip == (__u32)saddr;
}

static void reap_conntrack_tasklet(unsigned long data)
{
	struct ipt_dos_reap *reap;
	struct list_head *next, *head = &reap_all_head;

	if (reap_pending) {
		DEBUGP("Reaping the pending TCP sessions.\n");
		reap_pending = 0;
		ip_ct_iterate_cleanup(reap_pending_cmp, NULL);
	}

	/*
	  * The tasklet is executed under SOFTIRQ, need the LOCK ?_?
	  * I need to Learn more!^_^
	  */
	write_lock_bh(&dos_reap_lock);
	for (next = head->next; next != head;) {	
		reap = list_entry(next, struct ipt_dos_reap, list);
		next = next->next;

		DEBUGP("Reaping all TCP sessions from %u.%u.%u.%u\n",
					NIPQUAD(reap->saddr));
		list_del(&reap->list);
		ip_ct_iterate_cleanup(reap_all_cmp, (void *)reap->saddr);
		kfree(reap);
	}
	write_unlock_bh(&dos_reap_lock);
}

static struct tasklet_struct reap_tasklet  = {
	.next	= NULL,
	.state	= 0,
	.count	= ATOMIC_INIT(0),
	.func		= reap_conntrack_tasklet,
	.data	= 0,
};

static void trigger_reap_all(__u32 saddr)
{
	struct list_head *pos;
	struct ipt_dos_reap *reap = NULL;

	write_lock_bh(&dos_reap_lock);
	list_for_each(pos, &reap_all_head) {
		struct ipt_dos_reap *i = list_entry(pos, struct ipt_dos_reap, list);
		if (unlikely(i->saddr == saddr)) {
			DEBUGP("Duplicate reap info: %u.%u.%u.%u\n", NIPQUAD(saddr));
			reap = i;
			break;
		}
	}

	if (likely(reap == NULL) && (reap = kmalloc(sizeof(struct ipt_dos_reap), GFP_ATOMIC))) {
		DEBUGP("Add Reap Info: %u.%u.%u.%u\n", NIPQUAD(saddr));
		reap->saddr = saddr;
		list_add(&reap->list, &reap_all_head);
		tasklet_schedule(&reap_tasklet);
	}
	write_unlock_bh(&dos_reap_lock);
}

static void trigger_reap_pending(void)
{
	reap_pending = 1;
	tasklet_schedule(&reap_tasklet);
}

/*
  * If the malicious attack host is FOUND, it needs to start a timer to reap
  * the memory resource if it stops attacking.
  * @ If there is one attacker, the interval value is the DOS_BLOCK_TIME.
  * @ If there are more than one attacker, the timer scans the hash table
  * with some interval such as (DOS_BLOCK_TIME >> 2). 
  *
  * The timer makes sure releasing the memory finally, use the bellowing
  * command to verify:
  * [cat /proc/slabinfo | grep ipt_dos_stat]
  * ==> ipt_dos_stat           0      0     24  145    1 : tunables  120   60 .....
  */
static struct timer_list reap_block_timer;
static atomic_t dos_blocked_host_count = ATOMIC_INIT(0);

static void trigger_reap_block(void)
{
	int count;
	unsigned long timo;
	struct timer_list *timer = &reap_block_timer;

	/* Try to delete the timer anyway. */
	del_timer(timer);

	count = atomic_read(&dos_blocked_host_count);
	if (count <= 0)
		return;
	else if (count == 1)
		timo = DOS_BLOCK_TIME;
	else
		timo = DOS_BLOCK_TIME >> 2;

	timer->expires = jiffies + timo;
	add_timer(timer);
}

static void update_block_info(int add)
{
	if (add)
		atomic_inc(&dos_blocked_host_count);
	else
		atomic_dec(&dos_blocked_host_count);

	trigger_reap_block();
}

static void reap_block_fun(unsigned long unused)
{
	int i;
	struct ipt_dos_stat *reap;
	struct list_head *pos, *head;

	write_lock_bh(&ip_conntrack_dos_lock);
	for (i = 0; i < DOS_HTABLE_SIZE; i++) {
		head = &dos_htable[i];
		for (pos = head->next; pos != head;) {
			reap = list_entry(pos, struct ipt_dos_stat, list);
			pos = pos->next;
			if (!reap->blocking ||time_before(jiffies, reap->expires))
				continue;

			DEBUGP("HOST %u.%u%u.%u is out of black list\n",
						NIPQUAD(reap->saddr));
			reap->blocking = 0;
			if (reap->pending <= 0) {
				list_del(&reap->list);
				atomic_dec(&dos_blocked_host_count);
				kmem_cache_free(ip_conntrack_dos_cachep, reap);
			}
		}
	}
	write_unlock_bh(&ip_conntrack_dos_lock);

	if (atomic_read(&dos_blocked_host_count) > 0)
		trigger_reap_block();
}

/************************************************************************/

/* Bit value of 'IPS_SPI_DoS_BIT' MUST be set when calling this routine */
static void reset_dos_info(struct ip_conntrack *conntrack)
{
	__u32 saddr;
	struct ipt_dos_stat *info;
	struct list_head *pos, *head;

	saddr = conntrack->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;
	head = &dos_htable[ipt_dos_iphash(saddr)];

	write_lock_bh(&ip_conntrack_dos_lock);
	list_for_each(pos, head) {
		info = list_entry(pos, struct ipt_dos_stat, list);
		if (info->saddr == saddr) {
			if ((--info->pending <= 0) && (info->blocking == 0 ||
					time_after_eq(jiffies, info->expires))) {
				list_del(&info->list);
				kmem_cache_free(ip_conntrack_dos_cachep, info);
				if (info->blocking) 
					update_block_info(0);
			}
			atomic_dec(&ip_conntrack_dos_count);
			break;
		}
	}
	write_unlock_bh(&ip_conntrack_dos_lock);

	clear_bit(IPS_SPI_DoS_BIT, &conntrack->status);
}

/* @return: '1' ==> DROP; else ==> ACCEPT. */
static int update_dos_info(__u32 saddr)
{
	struct ipt_dos_stat *info;
	struct list_head *pos, *head;

	head = &dos_htable[ipt_dos_iphash(saddr)];

	write_lock_bh(&ip_conntrack_dos_lock);
	list_for_each(pos, head) {
		info = list_entry(pos, struct ipt_dos_stat, list);
		if (info->saddr == saddr) {
			int drop = 0;
			if (likely(!info->blocking)) {
				if (unlikely(++info->pending > DOS_PENDING_MAX)) {
					drop = 1;
					--info->pending;	/* Ignore this one */
					info->blocking = 1;
					info->expires = jiffies + DOS_BLOCK_TIME;
					trigger_reap_all(saddr);
					update_block_info(1);
				}
			} else {
				if (time_after_eq(jiffies, info->expires)) {
					info->blocking = 0;
					info->pending++;
					update_block_info(0);
					DEBUGP("Block %u.%u.%u.%u time is expired!\n",
								NIPQUAD(saddr));
				} else {
					drop = 1;
				}
			}

			if (!drop)
				atomic_inc(&ip_conntrack_dos_count);
			write_unlock_bh(&ip_conntrack_dos_lock);

			return drop;
		}
	}
	write_unlock_bh(&ip_conntrack_dos_lock);

	info = kmem_cache_alloc(ip_conntrack_dos_cachep, GFP_ATOMIC);
	if (!info) {
		DEBUGP("Can't allocate conntrack.\n");
		return 0;
	}
	info->saddr = saddr;
	info->blocking = 0;
	info->expires = 0;
	info->pending = 1;

	write_lock_bh(&ip_conntrack_dos_lock);
	list_add(&info->list, head);
	atomic_inc(&ip_conntrack_dos_count);
	write_unlock_bh(&ip_conntrack_dos_lock);

	return 0;
}

static int ipt_spiadvDoS_match(const struct sk_buff *skb,
			const struct net_device *in,
			const struct net_device *out,
			const void *matchinfo,
			int offset,
			int *hotdrop)
{
	__u8 *tcph;
	__u16 sport;
	struct iphdr *iph;
	struct ip_conntrack *ct;
	enum ip_conntrack_info ctinfo;

	ct = ip_conntrack_get(skb, &ctinfo);
	if (ct == NULL)
		return 0;

	if (is_monitored(ct)) {
		if (ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) {
			DEBUGP("The monitored DoS connection has been ESTABLISHED:\n");
			DUMP_TUPLE(&ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple);
			reset_dos_info(ct);
		}

		return 0;
	}

	/* MUST NOT be a fragment. */	
	if (offset)
		return 0;

	iph = skb->nh.iph;
	tcph = (__u8 *)iph + (iph->ihl << 2);
	if ((tcph[13] & TCP_ALL) != TCP_SYN)
		return 0;

	if (atomic_read(&ip_conntrack_dos_count) >= SYN_BLOCK_ALL_NUM) {
		DEBUGP("All TCP SYN packets from Internet are Dropped now!\n");

		if (net_ratelimit()) {
			sport = *((__u16 *)tcph);
			printk("[DoS Attack: SYN Flood] from source: %u.%u.%u.%u, port %u,\n",
					NIPQUAD(iph->saddr), ntohs(sport));
		}

		return 1;
	}

	if (atomic_read(&ip_conntrack_dos_count) >= REAP_PENDING_NUM) {
		DEBUGP("Need to reap the pending TCP sessions now!\n");
		trigger_reap_pending();
	}

#ifdef CONFIG_IP_NF_IP_ALIGNMENT
	__u32 saddr;
	u32_set(&saddr, &iph->saddr);
	if (!update_dos_info(saddr)) {
		set_bit(IPS_SPI_DoS_BIT, &ct->status);
		return 0;
	}
#else
	if (!update_dos_info(iph->saddr)) {
		set_bit(IPS_SPI_DoS_BIT, &ct->status);
		return 0;
	}
#endif

	if (net_ratelimit()) {
		sport = *((__u16 *)tcph);
		printk("[DoS Attack: SYN Flood] from source: %u.%u.%u.%u, port %u,\n",
				NIPQUAD(iph->saddr), ntohs(sport));
	}

	return 1;
}

static int ipt_spiadvDoS_checkentry(const char *tablename,
			const struct ipt_ip *ip,
			void *matchinfo,
			unsigned int matchsize,
			unsigned int hook_mask)
{
	if (strcmp(tablename, "mangle") != 0) {
		DEBUGP("spiDoS: wrong table %s\n", tablename);
		return 0;
	}

	if (hook_mask & ~(1 << NF_IP_PRE_ROUTING)) {
		DEBUGP("spiDoS: hook mask 0x%x bad\n", hook_mask);
		return 0;
	}

	if (ip->proto != IPPROTO_TCP) {
		printk("spiadvDoS: Only works on TCP packets\n");
		return 0;
	}

	return 1;
}

static void ipt_spiadvDoS_destroy(void *matchinfo,
			unsigned int matchinfosize)
{
	int i;
	struct ipt_dos_stat *reap;
	struct list_head *pos, *head;

	write_lock_bh(&ip_conntrack_dos_lock);
	for (i = 0; i < DOS_HTABLE_SIZE; i++) {
		head = &dos_htable[i];
		for (pos = head->next; pos != head;) {
			reap = list_entry(pos, struct ipt_dos_stat, list);
			pos = pos->next;

			if (reap->blocking)
				update_block_info(0);

			if (reap->pending <= 0) {
				DEBUGP("%u.%u%u.%u is %s blocking mode!\n",
						NIPQUAD(reap->saddr),
						reap->blocking ? "in" : "out");
				list_del(&reap->list);
				kmem_cache_free(ip_conntrack_dos_cachep, reap);
			} else {
				DEBUGP("%u.%u%u.%u has %d pending items!\n",
						NIPQUAD(reap->saddr),
						reap->pending);
				/*
				  * Reap this DoS item when 'proto->destroy' calls 'reset_dos_info',
				  * and clean the 'blocking' flag anyway for release.
				  */
				reap->blocking = 0;
			}
		}
	}
	write_unlock_bh(&ip_conntrack_dos_lock);
}

static struct ipt_match spiadvDoS_match = {
	.name		= "spiadvDoS",
	.match		= ipt_spiadvDoS_match,
	.checkentry	= ipt_spiadvDoS_checkentry,
	.destroy		= ipt_spiadvDoS_destroy,
	.me			= THIS_MODULE,
};

/* save the original conntrack TCP protocol callback function 'destroy', it is usually NULL value. */
static void (*ip_conntrack_tcp_destroyed)(struct ip_conntrack *conntrack);

static void tcp_dos_destroyed(struct ip_conntrack *conntrack)
{
	if (is_monitored(conntrack)) {
		DEBUGP("Deleting a DoS monitored conntrack!\n");
		reset_dos_info(conntrack);
	}

	if (ip_conntrack_tcp_destroyed)
		ip_conntrack_tcp_destroyed(conntrack);
}

static int __init init(void)
{
	int i;

	for (i = 0; i < DOS_HTABLE_SIZE; i++)
		INIT_LIST_HEAD(&dos_htable[i]);

	DEBUGP("The 'HZ' value = %d\n", HZ);

	init_timer(&reap_block_timer);
	reap_block_timer.function = reap_block_fun;

	ip_conntrack_dos_cachep = kmem_cache_create("ipt_dos_stat",
					sizeof(struct ipt_dos_stat), 0, 0, NULL, NULL);
	if (ip_conntrack_dos_cachep == NULL) {
		printk(KERN_ERR "Unable to create ipt_dos_stat slab cache!\n");
		return -1;
	}

	if (ipt_register_match(&spiadvDoS_match) != 0) {
		printk(KERN_ERR "Unable to register 'spiadvDoS' match!\n");
		kmem_cache_destroy(ip_conntrack_dos_cachep);
		return -1;
	}

	ip_conntrack_tcp_destroyed = ip_conntrack_protocol_tcp.destroy;
	ip_conntrack_protocol_tcp.destroy = tcp_dos_destroyed;

	return 0;
}

static void __exit fini(void)
{
	ip_conntrack_protocol_tcp.destroy = ip_conntrack_tcp_destroyed;
	ip_conntrack_tcp_destroyed = NULL;
	del_timer(&reap_block_timer);
	ipt_unregister_match(&spiadvDoS_match);
	kmem_cache_destroy(ip_conntrack_dos_cachep);	
}

module_init(init);
module_exit(fini);

