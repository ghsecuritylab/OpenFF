/* (C) 1999-2001 Paul `Rusty' Russell
 * (C) 2002-2004 Netfilter Core Team <coreteam@netfilter.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Jozsef Kadlecsik <kadlec@blackhole.kfki.hu>:
 *	- Real stateful connection tracking
 *	- Modified state transitions table
 *	- Window scaling support added
 *	- SACK support added
 *
 * Willy Tarreau:
 *	- State table bugfixes
 *	- More robust state changes
 *	- Tuning timer parameters
 *
 * version 2.2
 */

#include <linux/config.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/netfilter.h>
#include <linux/module.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/spinlock.h>

#include <net/tcp.h>

#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_conntrack.h>
#include <linux/netfilter_ipv4/ip_conntrack_protocol.h>

#if 0
#define DEBUGP printk
#define DEBUGP_VARS
#else
#define DEBUGP(format, args...)
#endif

/* Protects conntrack->proto.tcp */
static DEFINE_RWLOCK(tcp_lock);

/* "Be conservative in what you do, 
    be liberal in what you accept from others." 
    If it's non-zero, we mark only out of window RST segments as INVALID. */
int ip_ct_tcp_be_liberal = 0;

/* When connection is picked up from the middle, how many packets are required
   to pass in each direction when we assume we are in sync - if any side uses
   window scaling, we lost the game. 
   If it is set to zero, we disable picking up already established 
   connections. */
int ip_ct_tcp_loose = 3;

/* Max number of the retransmitted packets without receiving an (acceptable) 
   ACK from the destination. If this number is reached, a shorter timer 
   will be started. */
int ip_ct_tcp_max_retrans = 3;

  /* FIXME: Examine ipfilter's timeouts and conntrack transitions more
     closely.  They're more complex. --RR */

static const char *tcp_conntrack_names[] = {
	"NONE",
	"ESTABLISHED",
	"SYN_SENT",
	"SYN_RECV",
	"FIN_WAIT",
	"TIME_WAIT",
	"CLOSE",
	"CLOSE_WAIT",
	"LAST_ACK",
	"LISTEN"
};

#define SECS * HZ
#define MINS * 60 SECS
#define HOURS * 60 MINS
#define DAYS * 24 HOURS

unsigned long ip_ct_tcp_timeout_syn_sent =      2 MINS;
unsigned long ip_ct_tcp_timeout_syn_recv =     60 SECS;
#if defined(CONFIG_IP_NF_NAT_PROTO_TIMEOUT)
/*
  * [NETGEAR SPEC V1.6]:
  * 1.3 Behavioral Requirements for TCP
  *	REQ-2. A NAT TCP mapping timer MUST expire in 30 minutes.
  */
/*
  * [NETGEAR SPEC V1.8]:
  * 1.3 Behavioral Requirements for TCP
  *   REQ-1. When both TCP parties have sent a TCP packet with FIN set to each
  *     other, an implementation MUST remove the corresponding NAT mapping session instead of
  *     waiting for timeout.
  * 
*/
unsigned long ip_ct_tcp_timeout_established =  30 MINS;
unsigned long ip_ct_tcp_timeout_fin_wait =      1 MINS;
unsigned long ip_ct_tcp_timeout_close_wait =    1 MINS;
unsigned long ip_ct_tcp_timeout_last_ack =      1 SECS;
unsigned long ip_ct_tcp_timeout_time_wait =     1 SECS;
unsigned long ip_ct_tcp_timeout_close =         1 SECS;
#else
unsigned long ip_ct_tcp_timeout_established =   5 DAYS;
unsigned long ip_ct_tcp_timeout_fin_wait =      2 MINS;
unsigned long ip_ct_tcp_timeout_close_wait =   60 SECS;
unsigned long ip_ct_tcp_timeout_last_ack =     30 SECS;
unsigned long ip_ct_tcp_timeout_time_wait =     2 MINS;
unsigned long ip_ct_tcp_timeout_close =        10 SECS;
#endif

/* RFC1122 says the R2 limit should be at least 100 seconds.
   Linux uses 15 packets as limit, which corresponds 
   to ~13-30min depending on RTO. */
unsigned long ip_ct_tcp_timeout_max_retrans =     5 MINS;

static unsigned long * tcp_timeouts[]
= { 0,                                 /*      TCP_CONNTRACK_NONE */
    &ip_ct_tcp_timeout_established,    /*      TCP_CONNTRACK_ESTABLISHED,      */
    &ip_ct_tcp_timeout_syn_sent,       /*      TCP_CONNTRACK_SYN_SENT, */
    &ip_ct_tcp_timeout_syn_recv,       /*      TCP_CONNTRACK_SYN_RECV, */
    &ip_ct_tcp_timeout_fin_wait,       /*      TCP_CONNTRACK_FIN_WAIT, */
    &ip_ct_tcp_timeout_time_wait,      /*      TCP_CONNTRACK_TIME_WAIT,        */
    &ip_ct_tcp_timeout_close,          /*      TCP_CONNTRACK_CLOSE,    */
    &ip_ct_tcp_timeout_close_wait,     /*      TCP_CONNTRACK_CLOSE_WAIT,       */
    &ip_ct_tcp_timeout_last_ack,       /*      TCP_CONNTRACK_LAST_ACK, */
    0,                                 /*      TCP_CONNTRACK_LISTEN */
 };

#define sNO TCP_CONNTRACK_NONE
#define sES TCP_CONNTRACK_ESTABLISHED
#define sSS TCP_CONNTRACK_SYN_SENT
#define sSR TCP_CONNTRACK_SYN_RECV
#define sFW TCP_CONNTRACK_FIN_WAIT
#define sTW TCP_CONNTRACK_TIME_WAIT
#define sCL TCP_CONNTRACK_CLOSE
#define sCW TCP_CONNTRACK_CLOSE_WAIT
#define sLA TCP_CONNTRACK_LAST_ACK
#define sLI TCP_CONNTRACK_LISTEN
#define sIV TCP_CONNTRACK_MAX

/*
 * The TCP state transition table needs a few words...
 *
 * We are the man in the middle. All the packets go through us
 * but might get lost in transit to the destination.
 * It is assumed that the destinations can't receive segments 
 * we haven't seen.
 *
 * The checked segment is in window, but our windows are *not*
 * equivalent with the ones of the sender/receiver. We always
 * try to guess the state of the current sender.
 *
 * The meaning of the states are:
 *
 * NONE:	initial state
 * SYN_SENT:	SYN-only packet seen 
 * SYN_RECV:	SYN-ACK packet seen
 * ESTABLISHED:	ACK packet seen
 * FIN_WAIT:	FIN packet seen
 * CLOSE_WAIT:	ACK seen (after FIN) 
 * LAST_ACK:	FIN seen (after FIN)
 * TIME_WAIT:	last ACK seen
 * CLOSE:	closed connection
 *
 * LISTEN state is not used.
 *
 * Packets marked as IGNORED (sIG):
 *	if they may be either invalid or valid 
 *	and the receiver may send back a connection 
 *	closing RST or a SYN/ACK.
 *
 * Packets marked as INVALID (sIV):
 *	if they are invalid
 *	or we do not support the request (simultaneous open)
 */
static enum tcp_conntrack tcp_conntracks[2][5][TCP_CONNTRACK_MAX] = {
	{
/*	ORIGINAL */
/* 	  sNO, sES, sSS, sSR, sFW, sTW, sCL, sCW, sLA, sLI 	*/
/*syn*/	{sSS, sES, sSS, sSR, sSS, sSS, sSS, sSS, sSS, sLI },
/*fin*/	{sTW, sFW, sSS, sTW, sFW, sTW, sCL, sTW, sLA, sLI },
/*ack*/	{sES, sES, sSS, sES, sFW, sTW, sCL, sCW, sLA, sES },
/*rst*/ {sCL, sCL, sSS, sCL, sCL, sTW, sCL, sCL, sCL, sCL },
/*none*/{sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV }
	},
	{
/*	REPLY */
/* 	  sNO, sES, sSS, sSR, sFW, sTW, sCL, sCW, sLA, sLI 	*/
/*syn*/	{sSR, sES, sSR, sSR, sSR, sSR, sSR, sSR, sSR, sSR },
/*fin*/	{sCL, sCW, sSS, sTW, sTW, sTW, sCL, sCW, sLA, sLI },
/*ack*/	{sCL, sES, sSS, sSR, sFW, sTW, sCL, sCW, sCL, sLI },
/*rst*/ {sCL, sCL, sCL, sCL, sCL, sCL, sCL, sCL, sLA, sLI },
/*none*/{sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV, sIV }
	}
};


static int tcp_pkt_to_tuple(const struct sk_buff *skb,
			    unsigned int dataoff,
			    struct ip_conntrack_tuple *tuple)
{
	struct tcphdr _hdr, *hp;

	/* Actually only need first 8 bytes. */
	hp = skb_header_pointer(skb, dataoff, 8, &_hdr);
	if (hp == NULL)
		return 0;

	tuple->src.u.tcp.port = hp->source;
	tuple->dst.u.tcp.port = hp->dest;

	return 1;
}

static int tcp_invert_tuple(struct ip_conntrack_tuple *tuple,
			    const struct ip_conntrack_tuple *orig)
{
	tuple->src.u.tcp.port = orig->dst.u.tcp.port;
	tuple->dst.u.tcp.port = orig->src.u.tcp.port;
	return 1;
}

/* Print out the per-protocol part of the tuple. */
static int tcp_print_tuple(struct seq_file *s,
			   const struct ip_conntrack_tuple *tuple)
{
	return seq_printf(s, "sport=%hu dport=%hu ",
			  ntohs(tuple->src.u.tcp.port),
			  ntohs(tuple->dst.u.tcp.port));
}

/* Print out the private part of the conntrack. */
static int tcp_print_conntrack(struct seq_file *s,
			       const struct ip_conntrack *conntrack)
{
	enum tcp_conntrack state;

	read_lock_bh(&tcp_lock);
	state = conntrack->proto.tcp.state;
	read_unlock_bh(&tcp_lock);

	return seq_printf(s, "%s ", tcp_conntrack_names[state]);
}

#if defined(CONFIG_IP_NF_CONNTRACK_NETLINK) || \
    defined(CONFIG_IP_NF_CONNTRACK_NETLINK_MODULE)
static int tcp_to_nfattr(struct sk_buff *skb, struct nfattr *nfa,
			 const struct ip_conntrack *ct)
{
	struct nfattr *nest_parms;
	
	read_lock_bh(&tcp_lock);
	nest_parms = NFA_NEST(skb, CTA_PROTOINFO_TCP);
	NFA_PUT(skb, CTA_PROTOINFO_TCP_STATE, sizeof(u_int8_t),
		&ct->proto.tcp.state);
	read_unlock_bh(&tcp_lock);

	NFA_NEST_END(skb, nest_parms);

	return 0;

nfattr_failure:
	read_unlock_bh(&tcp_lock);
	return -1;
}

static const size_t cta_min_tcp[CTA_PROTOINFO_TCP_MAX] = {
	[CTA_PROTOINFO_TCP_STATE-1]	= sizeof(u_int8_t),
};

static int nfattr_to_tcp(struct nfattr *cda[], struct ip_conntrack *ct)
{
	struct nfattr *attr = cda[CTA_PROTOINFO_TCP-1];
	struct nfattr *tb[CTA_PROTOINFO_TCP_MAX];

	/* updates could not contain anything about the private
	 * protocol info, in that case skip the parsing */
	if (!attr)
		return 0;

        nfattr_parse_nested(tb, CTA_PROTOINFO_TCP_MAX, attr);

	if (nfattr_bad_size(tb, CTA_PROTOINFO_TCP_MAX, cta_min_tcp))
		return -EINVAL;

	if (!tb[CTA_PROTOINFO_TCP_STATE-1])
		return -EINVAL;

	write_lock_bh(&tcp_lock);
	ct->proto.tcp.state = 
		*(u_int8_t *)NFA_DATA(tb[CTA_PROTOINFO_TCP_STATE-1]);
	write_unlock_bh(&tcp_lock);

	return 0;
}
#endif

static unsigned int get_conntrack_index(const struct tcphdr *tcph)
{
	if (tcph->rst) return 3;
	else if (tcph->syn) return 0;
	else if (tcph->fin) return 1;
	else if (tcph->ack) return 2;
	else return 4;
}

/* TCP connection tracking based on 'Real Stateful TCP Packet Filtering
   in IP Filter' by Guido van Rooij.
   
   http://www.nluug.nl/events/sane2000/papers.html
   http://www.iae.nl/users/guido/papers/tcp_filtering.ps.gz
   
   The boundaries and the conditions are changed according to RFC793:
   the packet must intersect the window (i.e. segments may be
   after the right or before the left edge) and thus receivers may ACK
   segments after the right edge of the window.

   	td_maxend = max(sack + max(win,1)) seen in reply packets
	td_maxwin = max(max(win, 1)) + (sack - ack) seen in sent packets
	td_maxwin += seq + len - sender.td_maxend
			if seq + len > sender.td_maxend
	td_end    = max(seq + len) seen in sent packets
   
   I.   Upper bound for valid data:	seq <= sender.td_maxend
   II.  Lower bound for valid data:	seq + len >= sender.td_end - receiver.td_maxwin
   III.	Upper bound for valid ack:      sack <= receiver.td_end
   IV.	Lower bound for valid ack:	ack >= receiver.td_end - MAXACKWINDOW
   	
   where sack is the highest right edge of sack block found in the packet.
   	
   The upper bound limit for a valid ack is not ignored - 
   we doesn't have to deal with fragments. 
*/

static inline __u32 segment_seq_plus_len(__u32 seq,
					 size_t len,
					 struct iphdr *iph,
					 struct tcphdr *tcph)
{
	return (seq + len - (iph->ihl + tcph->doff)*4
		+ (tcph->syn ? 1 : 0) + (tcph->fin ? 1 : 0));
}
  
/* Fixme: what about big packets? */
#define MAXACKWINCONST			66000
#define MAXACKWINDOW(sender)						\
	((sender)->td_maxwin > MAXACKWINCONST ? (sender)->td_maxwin	\
					      : MAXACKWINCONST)


#ifdef CONFIG_IP_NF_NAT_NEEDED
/* Update sender->td_end after NAT successfully mangled the packet */
void ip_conntrack_tcp_update(struct sk_buff *skb,
			     struct ip_conntrack *conntrack, 
			     enum ip_conntrack_dir dir)
{
}
 
#endif

#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20
#define	TH_ECE	0x40
#define	TH_CWR	0x80

/* table of valid flag combinations - ECE and CWR are always valid */
static const u8 tcp_valid_flags[(TH_FIN|TH_SYN|TH_RST|TH_PUSH|TH_ACK|TH_URG) + 1] =
{
	[TH_SYN]			= 1,
	[TH_SYN|TH_ACK]			= 1,
	[TH_SYN|TH_PUSH]		= 1,
	[TH_SYN|TH_ACK|TH_PUSH]		= 1,
	[TH_RST]			= 1,
	[TH_RST|TH_ACK]			= 1,
	[TH_RST|TH_ACK|TH_PUSH]		= 1,
	[TH_FIN|TH_ACK]			= 1,
	[TH_ACK]			= 1,
	[TH_ACK|TH_PUSH]		= 1,
	[TH_ACK|TH_URG]			= 1,
	[TH_ACK|TH_URG|TH_PUSH]		= 1,
	[TH_FIN|TH_ACK|TH_PUSH]		= 1,
	[TH_FIN|TH_ACK|TH_URG]		= 1,
	[TH_FIN|TH_ACK|TH_URG|TH_PUSH]	= 1,
};

/* Protect conntrack agaist broken packets. Code taken from ipt_unclean.c.  */
static int tcp_error(struct sk_buff *skb,
		     enum ip_conntrack_info *ctinfo,
		     unsigned int hooknum)
{
	struct iphdr *iph = skb->nh.iph;
	struct tcphdr _tcph, *th;
	unsigned int tcplen = skb->len - iph->ihl * 4;
	u_int8_t tcpflags;

	/* Smaller that minimal TCP header? */
	th = skb_header_pointer(skb, iph->ihl * 4,
				sizeof(_tcph), &_tcph);
	if (th == NULL) {
		if (LOG_INVALID(IPPROTO_TCP))
			nf_log_packet(PF_INET, 0, skb, NULL, NULL, NULL,
				"ip_ct_tcp: short packet ");
		return -NF_ACCEPT;
  	}
  
	/* Not whole TCP header or malformed packet */
	if (th->doff*4 < sizeof(struct tcphdr) || tcplen < th->doff*4) {
		if (LOG_INVALID(IPPROTO_TCP))
			nf_log_packet(PF_INET, 0, skb, NULL, NULL, NULL,
				"ip_ct_tcp: truncated/malformed packet ");
		return -NF_ACCEPT;
	}
  
	/* Checksum invalid? Ignore.
	 * We skip checking packets on the outgoing path
	 * because the semantic of CHECKSUM_HW is different there 
	 * and moreover root might send raw packets.
	 */
	/* FIXME: Source route IP option packets --RR */
	if (hooknum == NF_IP_PRE_ROUTING
	    && skb->ip_summed != CHECKSUM_UNNECESSARY
	    && csum_tcpudp_magic(iph->saddr, iph->daddr, tcplen, IPPROTO_TCP,
			         skb->ip_summed == CHECKSUM_HW ? skb->csum
			      	 : skb_checksum(skb, iph->ihl*4, tcplen, 0))) {
		if (LOG_INVALID(IPPROTO_TCP))
			nf_log_packet(PF_INET, 0, skb, NULL, NULL, NULL,
				  "ip_ct_tcp: bad TCP checksum ");
		return -NF_ACCEPT;
	}

	/* Check TCP flags. */
	tcpflags = (((u_int8_t *)th)[13] & ~(TH_ECE|TH_CWR));
	if (!tcp_valid_flags[tcpflags]) {
		if (LOG_INVALID(IPPROTO_TCP))
			nf_log_packet(PF_INET, 0, skb, NULL, NULL, NULL,
				  "ip_ct_tcp: invalid TCP flag combination ");
		return -NF_ACCEPT;
	}

	return NF_ACCEPT;
}

/* Returns verdict for packet, or -1 for invalid. */
static int tcp_packet(struct ip_conntrack *conntrack,
		      const struct sk_buff *skb,
		      enum ip_conntrack_info ctinfo)
{
	enum tcp_conntrack new_state, old_state;
	enum ip_conntrack_dir dir;
	struct iphdr *iph = skb->nh.iph;
	struct tcphdr *th, _tcph;
	unsigned int index;
	
	th = skb_header_pointer(skb, iph->ihl * 4,
				sizeof(_tcph), &_tcph);
	BUG_ON(th == NULL);
	
	write_lock_bh(&tcp_lock);
	old_state = conntrack->proto.tcp.state;
	dir = CTINFO2DIR(ctinfo);
	index = get_conntrack_index(th);
	new_state = tcp_conntracks[dir][index][old_state];


	/* Invalid */
	if (new_state == TCP_CONNTRACK_MAX) {
		/* Invalid packet */
		DEBUGP("ip_ct_tcp: Invalid dir=%i index=%u ostate=%u\n",
		       dir, get_conntrack_index(th),
		       old_state);
		write_unlock_bh(&tcp_lock);
		if (LOG_INVALID(IPPROTO_TCP))
			nf_log_packet(PF_INET, 0, skb, NULL, NULL, NULL,
				  "ip_ct_tcp: invalid state ");
		return -NF_ACCEPT;
	}

	conntrack->proto.tcp.state = new_state;

	/* Poor man's window tracking: record SYN/ACK for handshake check */
	if (old_state == TCP_CONNTRACK_SYN_SENT
	    && CTINFO2DIR(ctinfo) == IP_CT_DIR_REPLY
	    && th->syn && th->ack)
		conntrack->proto.tcp.handshake_ack
			= htonl(ntohl(th->seq) + 1);

	/* If only reply is a RST, we can consider ourselves not to
	   have an established connection: this is a fairly common
	   problem case, so we can delete the conntrack
	   immediately.  --RR */
	if (!test_bit(IPS_SEEN_REPLY_BIT, &conntrack->status) && th->rst) {
		write_unlock_bh(&tcp_lock);
	#if defined(CONFIG_IP_NF_TARGET_SNATP2P)
		/* Maybe we don't need to care this condition  anymore, Rusty?
		   Since we take care of holepunch and hairpin!
		   */
		if (conntrack->status & IPS_NAT_MASK) {
			ip_ct_refresh_acct(conntrack, ctinfo, skb, 2 MINS);
			return NF_DROP;
		}
	#endif
		if (del_timer(&conntrack->timeout))
			conntrack->timeout.function((unsigned long)conntrack);
	} else {
		/* Set ASSURED if we see see valid ack in ESTABLISHED after SYN_RECV */
		if (old_state == TCP_CONNTRACK_SYN_RECV
		    && CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL
		    && th->ack && !th->syn
		    && th->ack_seq == conntrack->proto.tcp.handshake_ack)
			set_bit(IPS_ASSURED_BIT, &conntrack->status);

	#if defined(CONFIG_IP_NF_ALG_REFRESH)
		/*
		 * The protocol state of `master` conntrack and its refreshing time value SHOULD
		 * be considered more. --HY@20090119
		 */
		if ((conntrack->master != NULL)
		    && (conntrack->master->status & IPS_ALG_REFRESH) 
		    && (conntrack->master->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED)
		    && (del_timer(&conntrack->master->timeout))) {
			/* Need del_timer for race avoidance (may already be dying). */
			conntrack->master->timeout.expires = jiffies + ip_ct_tcp_timeout_established;
			add_timer(&conntrack->master->timeout);
		}
	#endif
		write_unlock_bh(&tcp_lock);

	#if defined(CONFIG_IP_NF_CT_NAT_SESSION)
		int refresh;
		/*
		 * REQ-6: The NAT mapping Refresh Direction MUST have a "NAT Outbound refresh 
		 * behavior" of "True".
		 *
		 * NETGEAR Requirements (UDP/TCP) --- 1. NAT Outbound Refresh Behavior: MUST; 
		 * 2. Inbound Refresh Behavior: MUST NOT.
		 *
		 * Do some optimization from `CTINFO2DIR':
		 * #define CTINFO2DIR(ctinfo)   \
		 *     ((ctinfo) >= IP_CT_IS_REPLY ? IP_CT_DIR_REPLY : IP_CT_DIR_ORIGINAL)
		 *
		 * Apply it only to TCP-Established connection state ?
		 * yes,We had better check TCP-Established state, because the tcp state imeout is usd by tcp
		 * protocal,it may affect much to that using tcp.if we do not check TCP-Established state,
		 * it may cause a bug 13787 Sometimes DUT's FTP ALG has issue.
		 *
		 */
		if ((old_state == TCP_CONNTRACK_ESTABLISHED) && (new_state == TCP_CONNTRACK_ESTABLISHED))
		{
			if (conntrack->status & IPS_SRC_NAT)
				refresh = ctinfo < IP_CT_IS_REPLY;
			else if (conntrack->status & IPS_DST_NAT)
				refresh = ctinfo >= IP_CT_IS_REPLY;
			else
				refresh = 1;
		}
		else
			refresh = 1;

		if (refresh)
	#endif
		ip_ct_refresh_acct(conntrack, ctinfo, skb, *tcp_timeouts[new_state]);
	}

	return NF_ACCEPT;
}
 
/* Called when a new connection for this protocol found. */
static int tcp_new(struct ip_conntrack *conntrack,
		   const struct sk_buff *skb)
{
	enum tcp_conntrack new_state;
	struct iphdr *iph = skb->nh.iph;
	struct tcphdr *th, _tcph;

	th = skb_header_pointer(skb, iph->ihl * 4,
				sizeof(_tcph), &_tcph);
	BUG_ON(th == NULL);
	
	/* Don't need lock here: this conntrack not in circulation yet */
	new_state
		= tcp_conntracks[0][get_conntrack_index(th)]
		[TCP_CONNTRACK_NONE];

	/* Invalid: delete conntrack */
	if (new_state >= TCP_CONNTRACK_MAX) {
		DEBUGP("ip_ct_tcp: invalid new deleting.\n");
		return 0;
	}

	conntrack->proto.tcp.state = new_state;
#if defined(CONFIG_IP_NF_CT_NAT_RESERVE)
        conntrack->status |= IPS_CT_TCP_RESERVE;
#endif
	return 1;
}
  
struct ip_conntrack_protocol ip_conntrack_protocol_tcp =
{
	.proto 			= IPPROTO_TCP,
	.name 			= "tcp",
	.pkt_to_tuple 		= tcp_pkt_to_tuple,
	.invert_tuple 		= tcp_invert_tuple,
	.print_tuple 		= tcp_print_tuple,
	.print_conntrack 	= tcp_print_conntrack,
	.packet 		= tcp_packet,
	.new 			= tcp_new,
	.error			= tcp_error,
#if defined(CONFIG_IP_NF_CONNTRACK_NETLINK) || \
    defined(CONFIG_IP_NF_CONNTRACK_NETLINK_MODULE)
	.to_nfattr		= tcp_to_nfattr,
	.from_nfattr		= nfattr_to_tcp,
	.tuple_to_nfattr	= ip_ct_port_tuple_to_nfattr,
	.nfattr_to_tuple	= ip_ct_port_nfattr_to_tuple,
#endif
};


/* Exporting symbol for 'ipt_spiadvDoS' using */
EXPORT_SYMBOL(ip_ct_tcp_timeout_syn_recv);
EXPORT_SYMBOL(ip_ct_tcp_timeout_syn_sent);
EXPORT_SYMBOL(ip_conntrack_protocol_tcp);

