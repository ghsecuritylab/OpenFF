/* SIP extension for UDP NAT alteration.
 *
 * (C) 2005 by Christian Hentschel <chentschel@arnet.com.ar>
 * based on RR's ip_nat_ftp.c and other modules.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/udp.h>

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_nat.h>
#include <linux/netfilter_ipv4/ip_nat_helper.h>
#include <linux/netfilter_ipv4/ip_conntrack_helper.h>
#include <linux/netfilter_ipv4/ip_conntrack_sip.h>
#if (DNI_SIP_ALG_PATCH)
#include <linux/netfilter_ipv4/ip_nat_core.h> // for ip_nat_packet
#include <linux/inet.h> // in_aton
#endif
#include <linux/ctype.h>
#include <linux/netfilter_ipv4/ip_conntrack_core.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Christian Hentschel <chentschel@arnet.com.ar>");
//MODULE_DESCRIPTION("DNISIP NAT helper");

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

extern struct sip_header_nfo ct_sip_hdrs[];

static unsigned int mangle_sip_packet(struct sk_buff **pskb,
				      enum ip_conntrack_info ctinfo,
				      struct ip_conntrack *ct,
				      const char **dptr, size_t dlen,
				      char *buffer, int bufflen,
				      struct sip_header_nfo *hnfo)
{
	unsigned int matchlen, matchoff;
#if (DNI_SIP_ALG_PATCH)
	char *ptr;
	char buf[sizeof("nnn.nnn.nnn.nnn:nnnnn")];
	u_int32_t ip;
	u_int16_t port;
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
#endif
	if (ct_sip_get_info(*dptr, dlen, &matchoff, &matchlen, hnfo) <= 0)
		return 0;

#if (DNI_SIP_ALG_PATCH)
	/*
	 *  the check if we need to mangle the ip or not
	 *  Because in sip proxy mode the contact (Via) field of incoming SIP packet may 
	 *  contain remote ip information, we can not mangle it to lan ip.
	*/
	if (dir == IP_CT_DIR_REPLY) { 
		ip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip;
		port = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port;
	} else {
		ip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;
		port = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port;
	}
	sprintf(buf, "%u.%u.%u.%u:%u", NIPQUAD(ip), ntohs(port));

	ptr = (char *)*dptr + matchoff;
	if ((hnfo == &ct_sip_hdrs[POS_VIA]) ||(hnfo == &ct_sip_hdrs[POS_CONTACT]))
	{
		char *s = ptr, *e = ptr + matchlen;
		int len = 0;
		while ((*s != ':') && (s != e))
			s++;
		if (*s == ':')
			len = s - ptr;
		if (memcmp(buf, ptr, len))
			return 1;
	}
	else if ((hnfo != &ct_sip_hdrs[POS_MEDIA])&& memcmp(buf, ptr, matchlen)) // media field only has port info
		return 1;
	// Call-id field has no port info
	if (hnfo == &ct_sip_hdrs[POS_CALL_ID])
	{
		ptr = buffer;
		while ((*ptr != ':') && (ptr != (buffer+bufflen)))
			ptr++;
		if (*ptr == ':')
			bufflen = ptr - buffer;
	}
#endif
	if (!ip_nat_mangle_udp_packet(pskb, ct, ctinfo,
				      matchoff, matchlen, buffer, bufflen))
		return 0;

	/* We need to reload this. Thanks Patrick. */
	*dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);

	return 1;
}

#if (CHECK_BRANCH)
static unsigned int record_sip_branch(struct sk_buff **pskb,
				      enum ip_conntrack_info ctinfo,
				      struct ip_conntrack *ct,
				      const char **dptr, size_t dlen,
				      struct sip_header_nfo *hnfo)
{
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	unsigned int matchlen, matchoff;
	const char *limit, *aux;
	char *branch;
	struct dnisip_branch_info *entry;

	aux = *dptr;

	limit = *dptr + (dlen - hnfo->lnlen);
	while (aux <= limit) {
		if ((memcmp(aux, hnfo->lname, hnfo->lnlen) == 0)
		    ||(memcmp(aux, hnfo->sname, hnfo->snlen) == 0))
			break;
		aux++;
	}
	if (limit < aux) {
		DEBUGP("record_sip_branch: Not found '%s' or '%s'\n", hnfo->lname, hnfo->sname);
		return 0;
	}

	limit = *dptr + (dlen - hnfo->ln_strlen);
	while (aux <= limit) {
		if ((memcmp(aux, hnfo->ln_str, hnfo->ln_strlen) == 0))
			break;
		aux++;
	}
	if (limit < aux) {
		DEBUGP("record_sip_branch: Not found '%s'\n", hnfo->ln_str);
		return 0;
	}

	if (dir == IP_CT_DIR_REPLY) {
		aux += hnfo->ln_strlen;
		while (aux <= limit) {
			if ((memcmp(aux, hnfo->ln_str, hnfo->ln_strlen) == 0))
				break;
			aux++;
		}
		if (limit < aux) {
			DEBUGP("record_sip_branch: Not found the second '%s'\n",
					hnfo->ln_str);
			return 0;
		}
	}

	aux += hnfo->ln_strlen;
	limit = *dptr + dlen;
	matchoff = aux - (*dptr);
	matchlen = 0;

	while (aux <= limit && (*aux) != '\r' && (*aux) != '\n') {
		aux++;
		matchlen++;
	}

	branch = kmalloc(matchlen + 1, GFP_ATOMIC);
	if (branch == NULL) {
		DEBUGP("record_sip_branch: malloc err!\n");
		return 0;
	}

	memcpy(branch, *dptr + matchoff, matchlen);
	branch[matchlen] = '\0';

	write_lock_bh(&ip_conntrack_lock);
	if (!ct->dnisip_branch) {
		entry = (struct dnisip_branch_info *)
			kmalloc(sizeof(struct dnisip_branch_info), GFP_ATOMIC);

		entry->branch[dir] = branch;
		entry->branch[!dir] = NULL;
		entry->port = 0;
		ct->dnisip_branch = entry;
	} else {
		if (ct->dnisip_branch->branch[dir])
			kfree(ct->dnisip_branch->branch[dir]);
		ct->dnisip_branch->branch[dir] = branch;
	}
	write_unlock_bh(&ip_conntrack_lock);

	DEBUGP("record sip_branch(len:%u):%s\n", matchlen, branch);
	*dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);

	return 1;
}

static unsigned int check_sip_branch(struct sk_buff **pskb,
				     enum ip_conntrack_info ctinfo,
				     struct ip_conntrack *ct,
				     const char **dptr, size_t dlen,
				     struct sip_header_nfo *hnfo)
{
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	unsigned int matchlen, matchoff, bufflen;
	const char *limit, *aux, *tmp;
	char buffer[16];

	/* "CSeq: xxx REGISTER\r\n" or "CSeq: xxx OPTIONS\r\n" */
	limit = *dptr + dlen;
	aux = ct_sip_search("CSeq:", *dptr, sizeof("CSeq:") - 1, dlen);
	if (!aux)
		return 0;

	aux += (sizeof("CSeq:") - 1);
	while ((isdigit(*aux) || (*aux == ' ')) && (aux <= limit))
		aux++;

	*dptr = aux;
	tmp = ct_sip_search("\r\n", *dptr, sizeof("\r\n") - 1, limit - aux);
	if (!tmp)
		return 0;

	matchlen = tmp - aux;
	if (((matchlen == 8) && (memcmp(aux, "REGISTER", 8) == 0))
	    || ((matchlen == 7) && (memcmp(aux, "OPTIONS", 7) == 0)))
		goto end;

	DEBUGP("check_sip_branch: there is no 'REGISTER' and 'OPTIONS' in 'CSeq:'\n");
	*dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
	
	aux = *dptr;

	limit = *dptr + (dlen - hnfo->lnlen);
	while (aux <= limit) {
		if ((memcmp(aux, hnfo->lname, hnfo->lnlen) == 0)
		    || (memcmp(aux, hnfo->sname, hnfo->snlen) == 0))
			break;
		aux++;
	}
	if (limit < aux) {
		DEBUGP("check_sip_branch: Not found '%s'\n", hnfo->lname);
		return 0;
	}

	limit = *dptr + (dlen - hnfo->ln_strlen);
	while (aux <= limit) {
		if ((memcmp(aux, hnfo->ln_str, hnfo->ln_strlen) == 0))
			break;
		aux++;
	}
	if (limit < aux) {
		DEBUGP("check_sip_branch: Not found '%s'\n", hnfo->ln_str);
		return 0;
	}

	if (dir == IP_CT_DIR_ORIGINAL) {
		aux += hnfo->ln_strlen;
		while (aux <= limit) {
			if ((memcmp(aux, hnfo->ln_str, hnfo->ln_strlen) == 0))
				break;
			aux++;
		}
		if (limit < aux) {
			DEBUGP("check_sip_branch: Not found the second '%s'\n",
					hnfo->ln_str);
			return 0;
		}
	}

	limit = *dptr + dlen;
	aux += hnfo->ln_strlen;
	matchoff = aux - (*dptr);
	matchlen = 0;
	*dptr = aux;

	tmp = ct_sip_search(";branch=", *dptr, sizeof(";branch=") - 1, dlen - matchoff);
	if (tmp) {
		DEBUGP("check_sip_branch: ';branch=' is found in 'Via'.\n");
		goto end;
	}

	while (aux <= limit && (*aux) != '\r' && (*aux) != '\n') {
		aux++;
		matchlen++;
	}

	if (ct->dnisip_branch) {
		if (!ip_nat_mangle_udp_packet(pskb, ct, ctinfo, matchoff, matchlen,
					      ct->dnisip_branch->branch[!dir],
					      strlen(ct->dnisip_branch->branch[!dir])))
			return 0;
		DEBUGP("check_sip_branch: add sip_branch(len: %u):%s\n", 
			strlen(ct->dnisip_branch->branch[!dir]), ct->dnisip_branch->branch[!dir]);

		if (dir == IP_CT_DIR_REPLY) {
			*dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
			aux = ct_sip_search(";rport", *dptr, sizeof(";rport") - 1, dlen);
			*dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
			if (aux) {
				matchoff = aux - *dptr;
				tmp = aux;
				aux += (sizeof(";rport") - 1);
				if (*aux == '=') {
					do {
						aux++;
					} while (aux <= limit && isdigit(*aux));
				}

				matchlen = aux -tmp;
			} else {
				DEBUGP("';rport' not found.\n");
				goto end;
			}

			bufflen = sprintf(buffer, ";rport=%u",
					  ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port));

			if (!ip_nat_mangle_udp_packet(pskb, ct, ctinfo, matchoff,
						      matchlen, buffer, bufflen))
				return 0;

			DEBUGP("check_sip_branch: change rport to '%s'\n", buffer);
		}
	}
end:
	*dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
	return 1;
}
#endif


#if (USE_VIA_PORT)
static inline void
use_via_port(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       const char *dptr)
{
	int matchoff, matchlen;
	int dataoff;
	char *s,*e, line[16];
	int n = 0;
	u_int16_t via_port = 0;

	dataoff = (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);

	if (ct_sip_get_info(dptr, (*pskb)->len - dataoff, &matchoff, &matchlen, &ct_sip_hdrs[POS_VIA]) <= 0)
		return ;

	s = e = (char *)(dptr + matchoff);
	while (*e != ';')
		e++;
	while (*s!= ':' && s!=e)
		s++;
	if (*s == ':')
		s++;
	n= e-s;
	if (n)
	{
		memcpy(line, s, n);
		line[n] = 0;
		via_port = simple_strtoul(line, NULL, 10);
	}

	DEBUGP ("%s: get via port info [%u]\n", __FUNCTION__, via_port);

	if (via_port && ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port != via_port) 
	{
		DEBUGP("PRE_ROUTING from %d changed to %d\n", ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port, via_port);
		ct->via_port = via_port;
	}
	else
		ct->via_port = 0;

	return;
}
#endif

#if (DNI_SIP_ALG_PATCH)
static inline void
mangle_contact_check(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       const char *dptr)
{
	/*
	  *  In order to mangle the contact field we need to consider three conditions
	  *  case 1: inside sip client calls outside sip client
	  * 	   we need to mangle the contact field of the outgoing/incoming sip packet for the inside sip client
	  *  case 2: outside sip client calls inside sip client
	  * 	   we do not need to mangle the contact field of incoming/outgoing sip packet for the inside sip client
	  *  case 3: inside sip client calls another inside sip client
	  * 	   we need to mangle the contact field of the outgoing/incoming sip packet for the calling party and 
	  * 	   we do not need to mangle the contact field of the incoming/outgoing sip packet for the called party
	  */
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	int matchoff, matchlen;
	int dataoff;
	char *s,*e, *end, line[16];
	int n = 0;
	u_int16_t contact_port = 0;
	u_int32_t conact_ip = 0;
	ct->mangle_contact = 0;

	if ((dir == IP_CT_DIR_ORIGINAL) && (ctinfo == IP_CT_NEW))
	{
		ct->mangle_contact = 1;
		return;
	}

	dataoff = (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
	dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);

	if (ct_sip_get_info(dptr, (*pskb)->len - dataoff, &matchoff, &matchlen, &ct_sip_hdrs[POS_CONTACT]) <= 0)
		return ;

	s = e = (char *)(dptr + matchoff);
	end = e + matchlen;
	while ((*e != ':')&&(*e != ';')&&(*e != '\r')&&(e != end))
		e++;
	n = e - s;
	if (n < 16)
		memcpy(line, s, n);
	else
		return;
	line[n] = 0;
	conact_ip = in_aton(line);

	s = e;
	while ((*e != ';')&&(e != end))
		e++;
	while ((*s!= ':') && (s!= e) && (s != end))
		s++;
	if (*s == ':')
		s++;
	n= e-s;
	if (n)
	{
		memcpy(line, s, n);
		line[n] = 0;
		contact_port = simple_strtoul(line, NULL, 10);
	}

	DEBUGP("%s :: get contact info [%u.%u.%u.%u:%u]\n", __FUNCTION__, NIPQUAD(conact_ip), contact_port);

	if (dir == IP_CT_DIR_ORIGINAL)
	{
		if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip == conact_ip)
			ct->mangle_contact = 1;
		else
			ct->mangle_contact = 0;
	}
	else
	{
		if (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip == conact_ip)
		{
			if (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all == contact_port)
				ct->mangle_contact = 1;
			else
				ct->mangle_contact = 0;
		}
		else
			ct->mangle_contact = 0;		
	}
	DEBUGP("%s :: mangle contact : %s \n", __FUNCTION__, (ct->mangle_contact)?"Yes":"No");
	dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
}
#endif

static unsigned int ip_nat_sip(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       const char **dptr)
{
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	char buffer[sizeof("nnn.nnn.nnn.nnn:nnnnn")];
	unsigned int bufflen, dataoff;
	u_int32_t ip;
	u_int16_t port;

	dataoff = (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);

	/* short packet ? */
	if (((*pskb)->len - dataoff) < (sizeof("SIP/2.0") - 1))
		return 0;

#if (USE_VIA_PORT)
	if ((dir == IP_CT_DIR_ORIGINAL) && (ctinfo == IP_CT_NEW))
		use_via_port(pskb, ctinfo, ct, *dptr);
	else if (dir == IP_CT_DIR_REPLY && ct->via_port)
	{
		// if Via header port is different from UDP source port, to mangle UDP source port to Via header port
		// and keep connection track is the same
		u_int16_t org_source_port = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port;
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port = ct->via_port;
		ip_nat_packet(ct, ctinfo, NF_IP_PRE_ROUTING, pskb);
		ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port = org_source_port;
	}
#endif
#if (DNI_SIP_ALG_PATCH)
	mangle_contact_check(pskb, ctinfo, ct, *dptr);
#endif
	if (dir == IP_CT_DIR_ORIGINAL) { 
		ip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip;
		port = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port;
	} else {
		ip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;
#if (USE_VIA_PORT)
		if (ct->via_port)
			port= ct->via_port;
		else
			port = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port;
#else
		port = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port;
#endif
	}
	bufflen = sprintf(buffer, "%u.%u.%u.%u:%u", NIPQUAD(ip), ntohs(port));

#if 0
	if (strnicmp(*dptr, "REGISTER" , 8) == 0)
		DEBUGP("\n*****ip_nat_sip***** REGISTER\n");
	else if (strnicmp(*dptr, "INVITE" , 6) == 0)
		DEBUGP("\n*****ip_nat_sip***** INVITE\n");
	else if (strnicmp(*dptr, "ACK" , 3) == 0)
		DEBUGP("\n*****ip_nat_sip***** ACK\n");
	else if (strnicmp(*dptr, "BYE", 3) == 0)
		DEBUGP("\n*****ip_nat_sip***** BYE\n");
	else if (strnicmp(*dptr, "CANCEL", 6) == 0)
		DEBUGP("\n*****ip_nat_sip***** BYE\n");
	else if (strnicmp(*dptr, "SIP/2.0 200", 11) == 0)
		DEBUGP("\n*****ip_nat_sip***** 200 OK\n");
	else if (strnicmp(*dptr, "SIP/2.0 100", 11) == 0)
		DEBUGP("\n*****ip_nat_sip***** 100 Trying\n");
	else if (strnicmp(*dptr, "SIP/2.0 180", 11) == 0)
		DEBUGP("\n*****ip_nat_sip***** 180 Ringing\n");
	else
		DEBUGP("\n*****ip_nat_sip***** Others\n");

	DEBUGP("dir:%s ctinfo:%d buffer:%s.\n", 
		dir == 0 ? "IP_CT_DIR_ORIGINAL" : "IP_CT_DIR_REPLY", (ctinfo), buffer);
	DEBUGP("ct: %u.%u.%u.%u:%u --> %u.%u.%u.%u:%u\n"
		"\t%u.%u.%u.%u:%u --> %u.%u.%u.%u:%u\n",
		NIPQUAD(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip),
		ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.udp.port),
		NIPQUAD(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.ip),
		ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.udp.port),
		NIPQUAD(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.ip),
		ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port),
		NIPQUAD(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip),
		ntohs(ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port));
#endif

	/* Status Packet */
	if (memcmp(*dptr, "SIP/2.0", sizeof("SIP/2.0") - 1) == 0) {
#if (CHECK_BRANCH)
		if (!check_sip_branch(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff, &ct_sip_hdrs[POS_VIA]))
			return 0;
#endif
		if (dir == IP_CT_DIR_REPLY) {
			if (!mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
					       buffer, bufflen, &ct_sip_hdrs[POS_VIA]))
				return 0;
			DEBUGP("#(Status)# IP_CT_DIR_REPLY Via is changed to %s.\n", buffer);
		}
#if (DNI_SIP_ALG_PATCH)
		if (ct->mangle_contact)
			mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				  buffer, bufflen, &ct_sip_hdrs[POS_CONTACT]);

		mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
			  buffer, bufflen, &ct_sip_hdrs[POS_CALL_ID]);

		*dptr = (*pskb)->data + (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
#else
		mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				  buffer, bufflen, &ct_sip_hdrs[POS_CONTACT]);
#endif
		return 1;
	}

	/* Request Packet:
	 * REGISTER,OPTIONS (client ---> server)
	 * INVITE,ACK,BYE,CANCEL,SUBSCRIBLE,REFER,NOTIFY,MESSAGE,INFO (client1 -> server -> client2)
	 */
#if (CHECK_BRANCH)
	if (memcmp(*dptr, "REGISTER", sizeof("REGISTER") - 1) != 0
	    && memcmp(*dptr, "OPTIONS", sizeof("OPTIONS") - 1) != 0)
		if (!record_sip_branch(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				       &ct_sip_hdrs[POS_VIA]))
			return 0;
#endif
	if (dir == IP_CT_DIR_ORIGINAL) { // client ---> server
#if (DNI_SIP_ALG_PATCH)
		ip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.ip;
		port = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.udp.port;
		bufflen = sprintf(buffer, "%u.%u.%u.%u:%u", NIPQUAD(ip), ntohs(port));
		if (memcmp(*dptr, "REGISTER", sizeof("REGISTER") - 1)==0)
			mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				buffer, bufflen, &ct_sip_hdrs[POS_REQ_URI]);

		mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				       buffer, bufflen, &ct_sip_hdrs[POS_FROM]);
		ip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.ip;
		port = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.udp.port;
		bufflen = sprintf(buffer, "%u.%u.%u.%u:%u", NIPQUAD(ip), ntohs(port));

#endif
		if (!mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				       buffer, bufflen, &ct_sip_hdrs[POS_VIA]))
			return 0;

		DEBUGP("#(Request)# IP_CT_DIR_ORIGINAL Via is changed to %s.\n", buffer);

		/* Mangle Contact if exists only. */
		mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				  buffer, bufflen, &ct_sip_hdrs[POS_CONTACT]);
	} else {
#if (DNI_SIP_ALG_PATCH)
		mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
			buffer, bufflen, &ct_sip_hdrs[POS_REQ_URI]);
#else
		mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
				  buffer, bufflen, &ct_sip_hdrs[POS_REQ_HEADER]);
#endif
	}

#if (DNI_SIP_ALG_PATCH)
	mangle_sip_packet(pskb, ctinfo, ct, dptr, (*pskb)->len - dataoff,
		  buffer, bufflen, &ct_sip_hdrs[POS_CALL_ID]);
#endif

	return 1;
}

static int mangle_content_len(struct sk_buff **pskb,
			      enum ip_conntrack_info ctinfo,
			      struct ip_conntrack *ct,
			      const char *dptr)
{
	unsigned int dataoff, matchoff, matchlen;
	char buffer[sizeof("65536")];
	int bufflen;

	dataoff = (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);

	/* Get actual SDP lenght */
	if (ct_sip_get_info(dptr, (*pskb)->len - dataoff, &matchoff,
	                    &matchlen, &ct_sip_hdrs[POS_SDP_HEADER]) > 0) {

		/* since ct_sip_get_info() give us a pointer passing 'v='
		   we need to add 2 bytes in this count. */
		int c_len = (*pskb)->len - dataoff - matchoff + 2;

		/* Now, update SDP lenght */
		if (ct_sip_get_info(dptr, (*pskb)->len - dataoff, &matchoff,
		                    &matchlen, &ct_sip_hdrs[POS_CONTENT]) > 0) {

			bufflen = sprintf(buffer, "%u", c_len);

			return ip_nat_mangle_udp_packet(pskb, ct, ctinfo,
							matchoff, matchlen,
							buffer, bufflen);
		}
	}
	return 0;
}

static unsigned int mangle_sdp(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       u_int32_t newip, u_int16_t port,
			       const char *dptr)
{
	char buffer[sizeof("nnn.nnn.nnn.nnn")];
	unsigned int dataoff, bufflen;

	dataoff = (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);

	DEBUGP("mangle_sdp newip:%u.%u.%u.%u port:%u\n",
			NIPQUAD(newip), ntohs(port));

	/* Mangle owner and contact info. */
	bufflen = sprintf(buffer, "%u.%u.%u.%u", NIPQUAD(newip));
	if (!mangle_sip_packet(pskb, ctinfo, ct, &dptr, (*pskb)->len - dataoff,
			       buffer, bufflen, &ct_sip_hdrs[POS_OWNER]))
		return 0;

	if (!mangle_sip_packet(pskb, ctinfo, ct, &dptr, (*pskb)->len - dataoff,
			       buffer, bufflen, &ct_sip_hdrs[POS_CONNECTION]))
		return 0;

	/* Mangle media port. */
	bufflen = sprintf(buffer, "%u", port);
	if (!mangle_sip_packet(pskb, ctinfo, ct, &dptr, (*pskb)->len - dataoff,
			       buffer, bufflen, &ct_sip_hdrs[POS_MEDIA]))
		return 0;

	return mangle_content_len(pskb, ctinfo, ct, dptr);
}

static int kill_sdp_nat_ct(struct ip_conntrack *i, void *data)
{
	return ip_ct_tuple_equal(
		(const struct ip_conntrack_tuple *)&i->tuplehash[IP_CT_DIR_ORIGINAL].tuple,
		(const struct ip_conntrack_tuple *)data);
}


void ip_nat_sdp_expect(struct ip_conntrack *ct,
		       struct ip_conntrack_expect *exp)
{
	struct ip_nat_range range;

	/* This must be a fresh one. */
	BUG_ON(ct->status & IPS_NAT_DONE_MASK);

	/* Change src to where master sends to */
	range.flags = IP_NAT_RANGE_MAP_IPS;
	range.min_ip = range.max_ip
		= ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;
	/* hook doesn't matter, but it has to do source manip */
	ip_nat_setup_info(ct, &range, NF_IP_POST_ROUTING);

	/* For DST manip, map port here to where it's expected. */
	range.flags = (IP_NAT_RANGE_MAP_IPS | IP_NAT_RANGE_PROTO_SPECIFIED);
	range.min = range.max = exp->saved_proto;
	range.min_ip = range.max_ip
		= ct->master->tuplehash[!exp->dir].tuple.src.ip;
	/* hook doesn't matter, but it has to do destination manip */
	ip_nat_setup_info(ct, &range, NF_IP_PRE_ROUTING);

	ip_ct_iterate_cleanup(kill_sdp_nat_ct, &ct->tuplehash[IP_CT_DIR_REPLY].tuple);
}

/* So, this packet has hit the connection tracking matching code.
   Mangle it, and change the expectation to match the new version. */
static unsigned int ip_nat_sdp(struct sk_buff **pskb,
			       enum ip_conntrack_info ctinfo,
			       struct ip_conntrack *ct,
			       struct ip_conntrack_expect *exp,
			       const char *dptr)
{
	enum ip_conntrack_dir dir = CTINFO2DIR(ctinfo);
	u_int32_t newip;
	u_int16_t port = 0;
#if (DNI_SIP_ALG_PATCH)
	unsigned char flag = 0;
#endif
	DEBUGP("#####ip_nat_sdp##### ctinfo:%u\n", ctinfo);

	if (dir == IP_CT_DIR_REPLY) {
		unsigned int dataoff, datalen;
		int matchoff, matchlen;

		dataoff = (*pskb)->nh.iph->ihl*4 + sizeof(struct udphdr);
		datalen = (*pskb)->len - dataoff;

		if (ct_sip_get_info(dptr, datalen, &matchoff, &matchlen,
				    &ct_sip_hdrs[POS_CONNECTION]) <= 0)
			return NF_ACCEPT;

		if (parse_ipaddr(dptr + matchoff, NULL, &newip, dptr + datalen) < 0)
			return NF_DROP;

		if (ct_sip_get_info(dptr, datalen, &matchoff, &matchlen,
				    &ct_sip_hdrs[POS_MEDIA]) <= 0)
			return NF_ACCEPT;

		port = simple_strtoul(dptr + matchoff, NULL, 10);
		if (port < 1024)
			return NF_DROP;

		struct ip_conntrack_expect *e;
		struct ip_conntrack_tuple tuple =
				{ { ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.ip, { 0 } },
				  { newip, { htons(port) }, IPPROTO_UDP } };

		DEBUGP("IP_CT_DIR_REPLY tuple %u.%u.%u.%u:%u -> %u.%u.%u.%u:%u\n",
			NIPQUAD(tuple.src.ip), ntohs(tuple.src.u.udp.port),
			NIPQUAD(tuple.dst.ip), ntohs(tuple.dst.u.udp.port));

		e =  ip_conntrack_expect_find(&tuple);
		if (e) {
			port = ntohs(e->saved_proto.udp.port);
			newip = e->master->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.ip;

			if (!mangle_sdp(pskb, ctinfo, ct, newip, port, dptr)) {
				ip_conntrack_unexpect_related(e);
				return NF_DROP;
			}

			ip_conntrack_unexpect_related(e);
		}

		return NF_ACCEPT;
	}

	if (!exp) {
		DEBUGP("Error, there is no exp!\n");
		return NF_DROP;
	}

	/* Connection will come from reply */
	newip = ct->tuplehash[!dir].tuple.dst.ip;

	exp->tuple.dst.ip = newip;
	exp->saved_proto.udp.port = exp->tuple.dst.u.udp.port;
	exp->dir = !dir;

	/* When you see the packet, we need to NAT it the same as the
	   this one. */
	exp->expectfn = ip_nat_sdp_expect;

	DEBUGP("exp tuple: "); DUMP_TUPLE(&exp->tuple);
	DEBUGP("mask:  "); DUMP_TUPLE(&exp->mask);

	/* Try to get same port: if not, try to change it. */
	for (port = ntohs(exp->saved_proto.udp.port); port != 0; port += 2) {
		exp->tuple.dst.u.udp.port = htons(port);
		if (ip_conntrack_expect_related(exp) == 0)
			break;
#if (DNI_SIP_ALG_PATCH)
		if (port == 65534)
		{
			switch(flag)
			{
				case 0:
					port = 49152;
					break;
				case 1:
					port = 1026;
					break;
				default:
					port = 0;
					break;
			}
			flag++;
		}
#endif
	}

	if (port == 0)
		return NF_DROP;

	DEBUGP("%s :: RTP port %d\n", __FUNCTION__, port);

	if (!mangle_sdp(pskb, ctinfo, ct, newip, port, dptr)) {
		ip_conntrack_unexpect_related(exp);
		return NF_DROP;
	}

	return NF_ACCEPT;
}

static void __exit fini(void)
{
	ip_nat_sip_hook = NULL;
	ip_nat_sdp_hook = NULL;
	/* Make sure noone calls it, meanwhile. */
	synchronize_net();
}

static int __init init(void)
{
	BUG_ON(ip_nat_sip_hook);
	BUG_ON(ip_nat_sdp_hook);
	ip_nat_sip_hook = ip_nat_sip;
	ip_nat_sdp_hook = ip_nat_sdp;
	return 0;
}

module_init(init);
module_exit(fini);
