#ifndef _NF_CONNTRACK_TCP_H
#define _NF_CONNTRACK_TCP_H
/* TCP tracking. */

/* This is exposed to userspace (ctnetlink) */
enum tcp_conntrack {
	TCP_CONNTRACK_NONE,
	TCP_CONNTRACK_ESTABLISHED,
	TCP_CONNTRACK_SYN_SENT,
	TCP_CONNTRACK_SYN_RECV,
	TCP_CONNTRACK_FIN_WAIT,
	TCP_CONNTRACK_TIME_WAIT,
	TCP_CONNTRACK_CLOSE,
	TCP_CONNTRACK_CLOSE_WAIT,
	TCP_CONNTRACK_LAST_ACK,
	TCP_CONNTRACK_LISTEN,
	TCP_CONNTRACK_MAX
};

#ifdef __KERNEL__

struct ip_ct_tcp
{
	u_int8_t	state;		/* state of the connection (enum tcp_conntrack) */

	/* Poor man's window tracking: sequence number of valid ACK
           handshake completion packet */
	u_int32_t handshake_ack;
};

#endif /* __KERNEL__ */

#endif /* _NF_CONNTRACK_TCP_H */
