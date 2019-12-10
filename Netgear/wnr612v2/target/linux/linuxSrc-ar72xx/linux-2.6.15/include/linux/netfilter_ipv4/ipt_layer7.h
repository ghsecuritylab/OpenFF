/* 
  By Matthew Strait <quadong@users.sf.net>, Dec 2003.
  http://l7-filter.sf.net

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version
  2 of the License, or (at your option) any later version.
  http://www.gnu.org/licenses/gpl.txt
*/

#ifndef _IPT_LAYER7_H
#define _IPT_LAYER7_H

#define MAX_PATTERN_LEN 8192
#define MAX_PROTOCOL_LEN 256

#define L7_SUCCESS 1
#define L7_ERROR 0

typedef char *(*proc_ipt_search) (char *, char, char *);
extern int (*qos_layer7_hook)(struct sk_buff *skb, int flag);
extern int gQosEnabled;

struct ipt_layer7_info {
    char protocol[MAX_PROTOCOL_LEN];
    char pattern[MAX_PATTERN_LEN];
	int prio;
	int su_prio;
#ifdef CONFIG_DNI_LIMIT_P2P_SESSION
	int p2p_flag;
#endif
};

#endif /* _IPT_LAYER7_H */
