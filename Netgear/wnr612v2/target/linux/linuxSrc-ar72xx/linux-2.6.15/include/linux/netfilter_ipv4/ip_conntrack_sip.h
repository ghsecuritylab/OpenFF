#ifndef __IP_CONNTRACK_DNISIP_H__
#define __IP_CONNTRACK_DNISIP_H__
#ifdef __KERNEL__

#define SIP_PORT	5060
#define SIP_TIMEOUT	3600
/*
*   To help us to trace the code segments that we modified. 
*/
#define DNI_SIP_ALG_PATCH 1
/*
*   for RFC3261 section 8.1.1.7 Via
*   we should use via header port instead of udp source port
*   for cd-router sip alg test case #45 : 
*   Verify Via header port is used for response instead of UDP source port
*   Because some poorly implemented SIP clients use a Via header that does not 
*   match the actual source packet of SIP request packets.
*   We should use the Via header port to mangle (NAT) the incoming SIP response.
*   ex. 
*   UDP source port is 12345 and Via header port is 5060
*   connection track :
*   192.168.1.2:12345 => 1.1.1.1:5060  1.1.1.1:5060 => 1.1.1.2:12345
*   
*   Actually the SIP response is send to 192.168.1.2:5060 but not 192.168.1.2:12345.
*   We need to use the Via header port as the destination port of incoming SIP reponse.
*/
#define USE_VIA_PORT 1
/*
*   If we want to check branch field is legal or not, we must consider SIP clients have two operation modes.
*   One is peer to peer mode and the other one is SIP proxy mode.
*   In these different modes, SIP clients have one or more Via fields.
*   That means we may have one or more branch info but current codes only process for SIP proxy mode.
*   For simplification to mark these code segments temporarily.
*/
#define CHECK_BRANCH 0

#if (DNI_SIP_ALG_PATCH)
enum sip_header_pos {
	POS_REG_REQ_URI,
	POS_REQ_URI,
	POS_FROM,
	POS_TO,
	POS_VIA,
	POS_CONTACT,
	POS_CONTENT,
	POS_MEDIA,
	POS_OWNER,
	POS_CONNECTION,
	POS_SDP_HEADER,
	POS_CALL_ID,
};
#else
#define POS_VIA		0
#define POS_CONTACT	1
#define POS_CONTENT	2
#define POS_MEDIA	3
#define POS_OWNER	4
#define POS_CONNECTION	5
#define POS_REQ_HEADER	6
#define POS_SDP_HEADER	7
#endif

struct sip_header_nfo {
	const char	*lname;
	const char	*sname;
	const char	*ln_str;
	size_t		lnlen;
	size_t		snlen;
	size_t		ln_strlen;
	int		(*match_len)(const char *, const char *, int *);
};

extern unsigned int (*ip_nat_sip_hook)(struct sk_buff **pskb,
				       enum ip_conntrack_info ctinfo,
				       struct ip_conntrack *ct,
				       const char **dptr);
extern unsigned int (*ip_nat_sdp_hook)(struct sk_buff **pskb,
				       enum ip_conntrack_info ctinfo,
				       struct ip_conntrack *ct,
				       struct ip_conntrack_expect *exp,
				       const char *dptr);

extern int ct_sip_get_info(const char *dptr, size_t dlen,
			   unsigned int *matchoff,
			   unsigned int *matchlen,
			   struct sip_header_nfo *hnfo);
extern int ct_sip_lnlen(const char *line, const char *limit);
extern const char *ct_sip_search(const char *needle, const char *haystack,
                                 size_t needle_len, size_t haystack_len);

extern int parse_ipaddr(const char *cp,	const char **endp,
			__be32 *ipaddr, const char *limit);
#endif /* __KERNEL__ */
#endif /* __IP_CONNTRACK_DNISIP_H__ */
