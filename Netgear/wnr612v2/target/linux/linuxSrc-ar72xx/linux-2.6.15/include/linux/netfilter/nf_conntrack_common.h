#ifndef _NF_CONNTRACK_COMMON_H
#define _NF_CONNTRACK_COMMON_H
#define CONFIG_IP_NF_CT_SCAN    1
#define CONFIG_LAN_PORT_RESERVED	1
#define CONFIG_DEL_UNUSED_CONNTRACK	1

/* Connection state tracking for netfilter.  This is separated from,
   but required by, the NAT layer; it can also be used by an iptables
   extension. */
enum ip_conntrack_info
{
	/* Part of an established connection (either direction). */
	IP_CT_ESTABLISHED,

	/* Like NEW, but related to an existing connection, or ICMP error
	   (in either direction). */
	IP_CT_RELATED,

	/* Started a new connection to track (only
           IP_CT_DIR_ORIGINAL); may be a retransmission. */
	IP_CT_NEW,

	/* >= this indicates reply direction */
	IP_CT_IS_REPLY,

	/* Number of distinct IP_CT types (no NEW in reply dirn). */
	IP_CT_NUMBER = IP_CT_IS_REPLY * 2 - 1
};

/* Bitset representing status of connection. */
enum ip_conntrack_status {
	/* It's an expected connection: bit 0 set.  This bit never changed */
	IPS_EXPECTED_BIT = 0,
	IPS_EXPECTED = (1 << IPS_EXPECTED_BIT),

	/* We've seen packets both ways: bit 1 set.  Can be set, not unset. */
	IPS_SEEN_REPLY_BIT = 1,
	IPS_SEEN_REPLY = (1 << IPS_SEEN_REPLY_BIT),

	/* Conntrack should never be early-expired. */
	IPS_ASSURED_BIT = 2,
	IPS_ASSURED = (1 << IPS_ASSURED_BIT),

	/* Connection is confirmed: originating packet has left box */
	IPS_CONFIRMED_BIT = 3,
	IPS_CONFIRMED = (1 << IPS_CONFIRMED_BIT),

	/* Connection needs src nat in orig dir.  This bit never changed. */
	IPS_SRC_NAT_BIT = 4,
	IPS_SRC_NAT = (1 << IPS_SRC_NAT_BIT),

	/* Connection needs dst nat in orig dir.  This bit never changed. */
	IPS_DST_NAT_BIT = 5,
	IPS_DST_NAT = (1 << IPS_DST_NAT_BIT),

	/* Both together. */
	IPS_NAT_MASK = (IPS_DST_NAT | IPS_SRC_NAT),

	/* Connection needs TCP sequence adjusted. */
	IPS_SEQ_ADJUST_BIT = 6,
	IPS_SEQ_ADJUST = (1 << IPS_SEQ_ADJUST_BIT),

	/* NAT initialization bits. */
	IPS_SRC_NAT_DONE_BIT = 7,
	IPS_SRC_NAT_DONE = (1 << IPS_SRC_NAT_DONE_BIT),

	IPS_DST_NAT_DONE_BIT = 8,
	IPS_DST_NAT_DONE = (1 << IPS_DST_NAT_DONE_BIT),

	/* Both together */
	IPS_NAT_DONE_MASK = (IPS_DST_NAT_DONE | IPS_SRC_NAT_DONE),

	/* Connection is dying (removed from lists), can not be unset. */
	IPS_DYING_BIT = 9,
	IPS_DYING = (1 << IPS_DYING_BIT),

	IPS_TRIGGER_BIT = 10,
	IPS_TRIGGER = (1 << IPS_TRIGGER_BIT),

#if defined(CONFIG_IP_NF_TARGET_SNATP2P)
	IPS_SNATP2P_SRC_BIT = 11,
	IPS_SNATP2P_SRC = (1 << IPS_SNATP2P_SRC_BIT),

	IPS_SNATP2P_DST_BIT = 12,
	IPS_SNATP2P_DST = (1 << IPS_SNATP2P_DST_BIT),

	/* Both together. */
	IPS_SNATP2P_MASK = (IPS_SNATP2P_DST | IPS_SNATP2P_SRC),

	IPS_SNATP2P_SRC_DONE_BIT = 13,
	IPS_SNATP2P_SRC_DONE = (1 << IPS_SNATP2P_SRC_DONE_BIT),

	IPS_SNATP2P_DST_DONE_BIT = 14,
	IPS_SNATP2P_DST_DONE = (1 << IPS_SNATP2P_DST_DONE_BIT),

	/* Both together. */
	IPS_SNATP2P_DONE_MASK = (IPS_SNATP2P_DST_DONE | IPS_SNATP2P_SRC_DONE),
#endif

	IPS_CONENAT_BIT = 15,
	IPS_CONENAT = (1<< IPS_CONENAT_BIT),

	/* This conntrack has been monitored under Advanced DoS module. */
	IPS_SPI_DoS_BIT = 16,
	IPS_SPI_DoS = (1 << IPS_SPI_DoS_BIT),

#if defined(CONFIG_IP_NF_ALG_REFRESH)
	IPS_ALG_REFRESH_BIT = 17,
	IPS_ALG_REFRESH = (1 << IPS_ALG_REFRESH_BIT),
#endif

#if defined(CONFIG_IP_NF_CT_NAT_RESERVE)
       IPS_CT_ICMP_RESERVE_BIT = 18,
       IPS_CT_ICMP_RESERVE = (1 << IPS_CT_ICMP_RESERVE_BIT),
       IPS_CT_TCP_RESERVE_BIT = 19,
       IPS_CT_TCP_RESERVE = (1 << IPS_CT_TCP_RESERVE_BIT),
#endif

#if (CONFIG_DEL_UNUSED_CONNTRACK)
	IPS_TRIGGERING_BIT = 19,
	IPS_TRIGGERING = (1 << IPS_TRIGGERING_BIT),

	IPS_FORWARDING_BIT = 20,
	IPS_FORWARDING = (1 << IPS_FORWARDING_BIT),

	IPS_UPNP_BIT = 21,
	IPS_UPNP = (1 << IPS_UPNP_BIT),

	IPS_DMZ_BIT = 22,
	IPS_DMZ = (1 << IPS_DMZ_BIT),
#endif
#if CONFIG_IP_NF_CT_SCAN
	IPS_LAN_IN_BIT = 23,
	IPS_LAN_IN = (1 << IPS_LAN_IN_BIT),
	IPS_WAN_IN_BIT = 24,
	IPS_WAN_IN = (1 << IPS_WAN_IN_BIT),
#endif
#ifdef CONFIG_DNI_LIMIT_P2P_SESSION
	IPS_P2P_SESSION_BIT = 25,
	IPS_P2P_SESSION = (1 << IPS_P2P_SESSION_BIT),
#endif

};

/* Connection tracking event bits */
enum ip_conntrack_events
{
	/* New conntrack */
	IPCT_NEW_BIT = 0,
	IPCT_NEW = (1 << IPCT_NEW_BIT),

	/* Expected connection */
	IPCT_RELATED_BIT = 1,
	IPCT_RELATED = (1 << IPCT_RELATED_BIT),

	/* Destroyed conntrack */
	IPCT_DESTROY_BIT = 2,
	IPCT_DESTROY = (1 << IPCT_DESTROY_BIT),

	/* Timer has been refreshed */
	IPCT_REFRESH_BIT = 3,
	IPCT_REFRESH = (1 << IPCT_REFRESH_BIT),

	/* Status has changed */
	IPCT_STATUS_BIT = 4,
	IPCT_STATUS = (1 << IPCT_STATUS_BIT),

	/* Update of protocol info */
	IPCT_PROTOINFO_BIT = 5,
	IPCT_PROTOINFO = (1 << IPCT_PROTOINFO_BIT),

	/* Volatile protocol info */
	IPCT_PROTOINFO_VOLATILE_BIT = 6,
	IPCT_PROTOINFO_VOLATILE = (1 << IPCT_PROTOINFO_VOLATILE_BIT),

	/* New helper for conntrack */
	IPCT_HELPER_BIT = 7,
	IPCT_HELPER = (1 << IPCT_HELPER_BIT),

	/* Update of helper info */
	IPCT_HELPINFO_BIT = 8,
	IPCT_HELPINFO = (1 << IPCT_HELPINFO_BIT),

	/* Volatile helper info */
	IPCT_HELPINFO_VOLATILE_BIT = 9,
	IPCT_HELPINFO_VOLATILE = (1 << IPCT_HELPINFO_VOLATILE_BIT),

	/* NAT info */
	IPCT_NATINFO_BIT = 10,
	IPCT_NATINFO = (1 << IPCT_NATINFO_BIT),

	/* Counter highest bit has been set */
	IPCT_COUNTER_FILLING_BIT = 11,
	IPCT_COUNTER_FILLING = (1 << IPCT_COUNTER_FILLING_BIT),
};

enum ip_conntrack_expect_events {
	IPEXP_NEW_BIT = 0,
	IPEXP_NEW = (1 << IPEXP_NEW_BIT),
};

#ifdef __KERNEL__
struct ip_conntrack_counter
{
	u_int32_t packets;
	u_int32_t bytes;
};

struct ip_conntrack_stat
{
	unsigned int searched;
	unsigned int found;
	unsigned int new;
	unsigned int invalid;
	unsigned int ignore;
	unsigned int delete;
	unsigned int delete_list;
	unsigned int insert;
	unsigned int insert_failed;
	unsigned int drop;
	unsigned int early_drop;
	unsigned int error;
	unsigned int expect_new;
	unsigned int expect_create;
	unsigned int expect_delete;
};

#endif /* __KERNEL__ */

#if defined(CONFIG_IP_NF_TARGET_SNATP2P) && defined(CONFIG_LAN_PORT_RESERVED)
#define LAN_PORT_RESERVATION_SIZE 150  //upnp(100), port_forward(20), port_trigger(20), remote_management(1)
#define TRIGGER_ANY_IP 0xffffffff // 255.255.255.255
struct lan_port_resv_t {
	uint32_t ip;
	uint16_t proto;
	uint16_t	ip_changed;	/* port trigger timeout used only */
	uint16_t min_port, max_port;
};
#endif

#endif /* _NF_CONNTRACK_COMMON_H */
