/* String matching match for iptables
 * 
 * (C) 2005 Pablo Neira Ayuso <pablo@eurodev.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

/* 
 * Copyright (C) 2007 Delta Networks Inc
 * Modifed according to NETGEAR Block Site
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_string.h>
#include <linux/textsearch.h>
#include <linux/tcp.h>

MODULE_AUTHOR("Pablo Neira Ayuso <pablo@eurodev.net>");
MODULE_DESCRIPTION("IP tables string match module");
MODULE_LICENSE("GPL");

void my_printk(char *str, int len, int allowed_flag)
{
	int i = 0;

	if (allowed_flag == 1)
		printk("[site allowed:");
	else if (allowed_flag == 0)
		printk("[site blocked:");
	
	for (i = 0; i < len; i++)
		printk("%c",*(str + i));
	
	printk("]");
}

/* Linear string search based on memcmp() */
char *search(char *needle, char *haystack, int needle_len, int haystack_len) 
{
	char *k = haystack + (haystack_len-needle_len);
	char *t = haystack;
	
	while (t <= k) {
		if (memcmp(t, needle, needle_len) == 0)
			return t;
		t++;
	}

	return NULL;
}

/* Search string based on string1 and string2 */
char *my_search (char *string, char *string1, char *string2, int len, int len1, int len2) 
{
	char *k = NULL;
	char *kk = NULL;
	char *t = string1;
	char *tt = string2;
	int count = 0;
	
	if (len > len1+len2)
		return NULL;
	
	if (len1 >= len) {
		k = string1 + (len1-len);
		while (t <= k) {
			if (memcmp(t, string, len) == 0)
				return t;
			t++;
		}
		
		count = len - 1;
	}else 
		count = len1;

	while ((count > 0) && (len2 >= len-count)) {
		if (memcmp(t, string, count) == 0 && (memcmp(tt, string+count, len-count) == 0)) 
			return t;
		
		t++;
		count--;
	}

	if (len2 >= len) {
		kk = string2 + (len2-len);
		while (tt <= kk) {
			if (memcmp(tt, string, len) == 0) 
				return tt;
			tt++;
		}
	}
	
	return NULL;
}


int my_strstr(char *dest, char *str, int dest_len)
{
	int len;
	int ret = 0;

	len = strlen(str);
	if (!len)
		return -1;
	
	while (dest_len >= len) {
		dest_len--;
		if (!memcmp(dest,str,len))
			return ret;
		dest++;
		ret++;
	}
	
	return -1;
}

static int match(const struct sk_buff *skb,
		 const struct net_device *in,
		 const struct net_device *out,
		 const void *matchinfo,
		 int offset,
		 int *hotdrop)
{
	const struct ipt_string_info *info = matchinfo;
	struct iphdr *ip = skb->nh.iph;
	int hlen, nlen;
	char *needle, *haystack;

	if (!ip)
		return 0;

	/* get lenghts, and validate them */
	nlen = info->len;
	hlen = ntohs(ip->tot_len) - (ip->ihl * 4);
	if (nlen > hlen)
		return 0;

	needle = (char *)&info->string;
	haystack = (char *)ip + (ip->ihl * 4);

	char *http_method = "GET"; /* http_method_len:3 */
	char *http_head = "http://"; /* http_head_len:7 */
	char *http_version_a = "HTTP/1.0\r\n"; /* http_version_len:10 */
	char *http_version_b = "HTTP/1.1\r\n"; /* http_host_head_len:10 */
	char *http_host_head = "Host:"; /* http_host_head_len:5 */
	char *http_host_end = "\r\n"; /* http_host_end_len:2 */
	char *nntp_head = "GROUP"; /* nntp_head_len:5 */
	char *nntp_end = "\r\n"; /* nntp_end_len:2 */
	int http_method_len = 3;
	int http_head_len = 7;
	int http_version_len = 10;
	int http_host_head_len = 5;
	int nntp_head_len = 5;
	char *tcp_data = NULL;
	char *http_data = NULL;
	char *host_data = NULL;
	char *nntp_data = NULL;
	int http_data_len = 0;
	int host_data_len = 0;
	int nntp_data_len = 0;
	int temp = 0;
	int absolute_URI = 0;
	struct tcphdr *tcp_head;

	tcp_head = (struct tcphdr *)(skb->data + (skb->nh.iph->ihl*4));
	tcp_data= skb->data + (skb->nh.iph->ihl*4) + (tcp_head->doff*4);

	/* Block Site for NNTP */
	if (memcmp(tcp_data, nntp_head, nntp_head_len) == 0) {
		nntp_data = tcp_data + nntp_head_len + 1;
		nntp_data_len = my_strstr(nntp_data, nntp_end, hlen-nntp_head_len-1);

		if (nntp_data_len < 0)
			return (0 ^ info->invert);

		if (info->allow_flag) {
			my_printk(nntp_data, nntp_data_len, 1);
			printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
			return 1;
		}

		if ((search(needle, nntp_data, nlen, nntp_data_len) != NULL) ^ info->invert) {
			my_printk(nntp_data, nntp_data_len, 0);
			printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
			return 1;
		} else
			return 0;
	}

	/* Block Site for HTTP */
	if (memcmp(tcp_data, http_method, http_method_len) != 0)
		return (0 ^ info->invert);

	http_data = tcp_data + http_method_len + 1;
	http_data_len = my_strstr(http_data, http_version_a, hlen-http_method_len-1);

	if (http_data_len < 0)
		http_data_len = my_strstr(http_data, http_version_b, hlen-http_method_len-1);

	if (http_data_len < 0)
		return (0 ^ info->invert);

	if (memcmp(http_data, http_head, http_head_len) == 0) {
		http_data = http_data + http_head_len;
		http_data_len = http_data_len - http_head_len;
		absolute_URI = 1;
	}

	temp = my_strstr(http_data+http_data_len+http_version_len, http_host_head,
	hlen-http_method_len-1-http_data_len-http_version_len);

	if (temp < 0)
		goto match_log;

	host_data = http_data + http_data_len + http_version_len + temp + http_host_head_len;
	host_data_len = my_strstr(host_data, http_host_end,
	hlen-http_method_len-1-http_data_len-http_version_len-temp-http_host_head_len);

	if (host_data_len < 0)
		goto match_log;

	if (absolute_URI == 1) {
		if (info->allow_flag) {
			my_printk(host_data, host_data_len, 1);
			printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
			return 1;
		}

		if ((search(needle, http_data, nlen, http_data_len) != NULL) ^ info->invert) {
			my_printk(host_data, host_data_len, 0);
			printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
			return 1;
		} else
			return 0;
	}

	if (info->allow_flag) {
		my_printk(host_data, host_data_len, 1);
		printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
		return 1;
	}

	if ((my_search(needle, host_data, http_data, nlen, host_data_len, http_data_len) != NULL) ^ info->invert) {
		my_printk(host_data, host_data_len, 0);
		printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
		return 1;
	} else
		return 0;

match_log:
	if (info->allow_flag) {
		my_printk(http_data, http_data_len, 1);
		printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
		return 1;
	}

	if ((search(needle, http_data, nlen, http_data_len) != NULL) ^ info->invert) {
		my_printk(http_data, http_data_len, 0);
		printk(" from source %u.%u.%u.%u,\n", NIPQUAD((skb->nh.iph)->saddr));
		return 1;
	} else
		return 0;
}

static int checkentry(const char *tablename,
		      const struct ipt_ip *ip,
		      void *matchinfo,
		      unsigned int matchsize,
		      unsigned int hook_mask)
{
	if (matchsize != IPT_ALIGN(sizeof(struct ipt_string_info)))
		return 0;

	return 1;
}

static struct ipt_match string_match = {
	.name 		= "string",
	.match 		= match,
	.checkentry	= checkentry,
	.me 		= THIS_MODULE
};

static int __init init(void)
{
	return ipt_register_match(&string_match);
}

static void __exit fini(void)
{
	ipt_unregister_match(&string_match);
}

module_init(init);
module_exit(fini);
