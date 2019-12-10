/*
 *	Handle incoming frames
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	$Id: //depot/sw/releases/7.3_AP/linux/kernels/mips-linux-2.6.15/net/bridge/br_input.c#1 $
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netfilter_bridge.h>
#include "br_private.h"
#ifdef CONFIG_DNI_MCAST_TO_UNICAST
#include <linux/ip.h>
#include <linux/igmp.h>
#include <linux/in.h>
#include <linux/compiler.h>
#ifdef IPTV
#include "br_igmpv3.h"

extern unsigned short int iptv_on;
extern unsigned short int iEth1;
extern unsigned short int iEth2;
extern unsigned short int iAth0;
extern unsigned short int iAth1;
extern unsigned short int iAth2;
extern unsigned short int iAth3;
extern unsigned short int iAth4;
extern struct net_device *Wandev;
extern struct net_device *Brdev;
extern struct net_bridge *Br;
extern unsigned int igmp_ready;
extern void remove_wl_all(__u32 mip);
#endif

static inline void 
add_mac_cache(struct sk_buff *skb)
{
	unsigned char i, num = 0xff;
	unsigned char *src, check = 1;
	struct iphdr *iph;

	iph = skb->nh.iph;
	src = skb->mac.ethernet->h_source;

	for (i = 0; i < MCAST_ENTRY_SIZE; i++)
	{
		if (mac_cache[i].valid)
			if ((++mac_cache[i].count) == MAX_CLEAN_COUNT)
				//mac_cache[i].valid = 0;
				memset(&mac_cache[i],0,sizeof(mac_cache[i]));//jonathan modified
	}

	for (i = 0; i < MCAST_ENTRY_SIZE; i++)
	{
		if (mac_cache[i].valid)
		{
			if (mac_cache[i].sip==iph->saddr)
			{
				num = i;
				break;
			}
		}
		else if (check)
		{
			num=i;
			check = 0;
		}
	}

	if (num < MCAST_ENTRY_SIZE)
	{
		mac_cache[num].valid = mac_cache[num].count = 1;
		memcpy(mac_cache[num].mac, src, 6);
		mac_cache[num].sip = iph->saddr;
		mac_cache[num].dev = skb->dev;
	}
}

#endif
const unsigned char bridge_ula[6] = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x00 };

static int br_pass_frame_up_finish(struct sk_buff *skb)
{
	netif_receive_skb(skb);
	return 0;
}

static void br_pass_frame_up(struct net_bridge *br, struct sk_buff *skb)
{
	struct net_device *indev;
#ifdef CONFIG_DNI_MCAST_TO_UNICAST
	unsigned char *dest;
	struct iphdr *iph;
	unsigned char proto=0;
	// if skb come from wireless interface, ex. ath0, ath1, ath2...
	if (skb->dev->name[0] == 'a')
	{
		iph = skb->nh.iph;
		proto =  iph->protocol;
		dest = skb->mac.ethernet->h_dest;
		if ( igmp_snoop_enable && MULTICAST_MAC(dest) 
			 && (skb->mac.ethernet->h_proto == ETH_P_IP))
		{
			if (proto == IPPROTO_IGMP)
				add_mac_cache(skb);
		}
	}
#endif
	br->statistics.rx_packets++;
	br->statistics.rx_bytes += skb->len;

	indev = skb->dev;
	skb->dev = br->dev;

	NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, indev, NULL,
			br_pass_frame_up_finish);
}

/* note: already called with rcu_read_lock (preempt_disabled) */
int br_handle_frame_finish(struct sk_buff *skb)
{
	const unsigned char *dest = eth_hdr(skb)->h_dest;
	struct net_bridge_port *p = skb->dev->br_port;
	struct net_bridge *br = p->br;
	struct net_bridge_fdb_entry *dst;
	int passedup = 0;

	/* insert into forwarding database after filtering to avoid spoofing */
	br_fdb_update(p->br, p, eth_hdr(skb)->h_source);

	if (br->dev->flags & IFF_PROMISC) {
		struct sk_buff *skb2;

		skb2 = skb_clone(skb, GFP_ATOMIC);
		if (skb2 != NULL) {
			passedup = 1;
			br_pass_frame_up(br, skb2);
		}
	}

	if (dest[0] & 1) {
#ifdef IPTV
		struct iphdr *iph;
		unsigned int ipaddr=0;
		unsigned char proto=0;
		iph = skb->nh.iph;
		ipaddr =  iph->daddr;
		proto =  iph->protocol; 
		
		/* iptv is enable and gets multicast or broadcast packets include ipv6 from eth0 or iptv port
		the packets need to send to other ports of the bridge and send to upper layer*/
		if (iptv_on == 1 && (Wandev != NULL) && (Brdev != NULL) && 
			((!memcmp(skb->dev->name, "eth0", 4)) || 
			((!memcmp(skb->dev->name, "eth1", 4)) && iEth1 == 1) || 
			((!memcmp(skb->dev->name, "eth2", 4)) && iEth2 == 1) || 
			((!memcmp(skb->dev->name, "ath0", 4)) && iAth0 == 1) || 
			((!memcmp(skb->dev->name, "ath1", 4)) && iAth1 == 1) || 
			((!memcmp(skb->dev->name, "ath2", 4)) && iAth2 == 1) || 
			((!memcmp(skb->dev->name, "ath3", 4)) && iAth3 == 1) || 
			((!memcmp(skb->dev->name, "ath4", 4)) && iAth4 == 1)))
		{
			if (MULTICAST_MAC(dest) && (proto != IPPROTO_IGMP) && !is_control(ipaddr))// streaming
			{
				if (igmp_ready == 0)//if iptv is not ready, drop the packet
				{
					remove_wl_all(ipaddr);
					kfree_skb(skb);
					goto out;
				}
				
				if (memcmp(skb->dev->name, "eth0", 4) != 0) // Multicast packet comes from iptv ports, tx a copy to wan side
				{
					struct sk_buff *skb2;

					skb2 = skb_clone(skb, GFP_ATOMIC);
					if (skb2 != NULL)
					{
						skb_push(skb2, ETH_HLEN);
						skb2->dev = Wandev;
						skb2->dev->hard_start_xmit(skb2, skb2->dev );
						skb_pull(skb2, ETH_HLEN);
					}
				}
				
				// Multicast packets come from eth0 or iptv ports
				passedup = 1;
				br_iptv_multicast_forward(br, skb, !passedup);
				goto out;
			}
			else if (proto == IPPROTO_IGMP && memcmp(skb->dev->name, "eth0", 4) != 0)
			{
				// Process IGMP packet from wire and wireless of iptv ports
				// for igmpGroup structure use, cb[3] records the which port receiving
				if (!memcmp(skb->dev->name, "eth1", 4))
					skb->cb[3] = 0x01;
				else if (!memcmp(skb->dev->name, "eth2", 4))
					skb->cb[3] = 0x02;
				
				// Process IGMP group
				iptv_ProcessIgmpFromLan(skb);
				passedup = 1;
				br_flood_forward(br, skb, !passedup);
				goto out;
			}
			else // it's broadcast (noon IGMP or Multicast packets)
			{
				br_flood_forward(br, skb, !passedup);

				/* mark cb[10]=1 and pass this packet to eth0. eth0 gets it and check cb[10], 
				    if cb[10]=1 then pass it to upper layer */
				skb->cb[10] = 1;
				br->statistics.rx_packets++;
				br->statistics.rx_bytes += skb->len;
				skb->dev = Brdev;
				br_pass_frame_up_finish(skb);
				goto out;
			}
		}
		else
		{
			if (igmp_ready == 0)//if iptv is not ready, drop the packet
			{
				kfree_skb(skb);
				goto out;
			}
up_proto:
			br_flood_forward(br, skb, !passedup);
			if (!passedup)
				br_pass_frame_up(br, skb);
			goto out;
		}
	}
#else
up_proto:
		br_flood_forward(br, skb, !passedup);
		if (!passedup)
			br_pass_frame_up(br, skb);
		goto out;
	}
#endif

	dst = __br_fdb_get(br, dest);
	if (dst != NULL && dst->is_local) {
#ifdef IPTV
		/* iptv is enable and gets unicast packets from iptv port and the destination is in local. 
		The packets need to send to upper layer*/
		if (iptv_on == 1 && (Brdev != NULL) && 
			(((!memcmp(skb->dev->name, "eth1", 4)) && iEth1 == 1) || 
			((!memcmp(skb->dev->name, "eth2", 4)) && iEth2 == 1) ||  
			((!memcmp(skb->dev->name, "ath0", 4)) && iAth0 == 1) || 
			((!memcmp(skb->dev->name, "ath1", 4)) && iAth1 == 1) || 
			((!memcmp(skb->dev->name, "ath2", 4)) && iAth2 == 1) || 
			((!memcmp(skb->dev->name, "ath3", 4)) && iAth3 == 1) || 
			((!memcmp(skb->dev->name, "ath4", 4)) && iAth4 == 1)))
		{
			//printk("dst->is_local: skb->dev->name = %s ------------------------------\n", skb->dev->name);
			skb->cb[10] = 1;
			br->statistics.rx_packets++;
			br->statistics.rx_bytes += skb->len;
			skb->dev = Brdev;
			//printk("dst->is_local: skb->dev->name = %s ------------------------------\n", skb->dev->name);
			br_pass_frame_up_finish(skb);
		}
		else if (!passedup)
#else
		if (!passedup)
#endif
			br_pass_frame_up(br, skb);
		else
			kfree_skb(skb);
		goto out;
	}

	if (dst != NULL) { // unicast packet and found the dst in bridge
		skb->cb[12] = 1;
		br_forward(dst->dst, skb);
		goto out;
	}
#ifdef IPTV
	/* iptv is enable and gets ipv6 unicast packets from eth0 or iptv port, the bridge cannot find 
	the destination in fdb, the packets need to send to other ports of the bridge and send to
	upper layer*/
	else if (iptv_on == 1 && (Brdev != NULL) && 
		((!memcmp(skb->dev->name, "eth0", 4)) || 
		((!memcmp(skb->dev->name, "eth1", 4)) && iEth1 == 1) || 
		((!memcmp(skb->dev->name, "eth2", 4)) && iEth2 == 1) || 
		((!memcmp(skb->dev->name, "ath0", 4)) && iAth0 == 1) || 
		((!memcmp(skb->dev->name, "ath1", 4)) && iAth1 == 1) || 
		((!memcmp(skb->dev->name, "ath2", 4)) && iAth2 == 1) || 
		((!memcmp(skb->dev->name, "ath3", 4)) && iAth3 == 1) || 
		((!memcmp(skb->dev->name, "ath4", 4)) && iAth4 == 1)) && 
		skb->protocol == __constant_htons(ETH_P_IPV6))
	{
		//printk("Get IPv6 unicast packet! -----------------------\n");
	    br_flood_forward(br, skb, !passedup);

		/* mark cb[10]=1 and pass this packet to eth0. eth0 gets it and check cb[10], 
			if cb[10]=1 then pass it to upper layer */
		skb->cb[10] = 1;
		br->statistics.rx_packets++;
		br->statistics.rx_bytes += skb->len;
		skb->dev = Brdev;
		br_pass_frame_up_finish(skb);
		goto out;
	}
#endif
#ifdef CONFIG_DNI_IPV6_PASSTHROUGH
	else if (skb->protocol == __constant_htons(ETH_P_IPV6)) { //unicast from lan and the bridge cannot find the destination in fdb
	    goto up_proto;
	}
#endif

	br_flood_forward(br, skb, 0);

out:
	return 0;
}

/*
 * Called via br_handle_frame_hook.
 * Return 0 if *pskb should be processed furthur
 *	  1 if *pskb is handled
 * note: already called with rcu_read_lock (preempt_disabled) 
 */
int br_handle_frame(struct net_bridge_port *p, struct sk_buff **pskb)
{
	struct sk_buff *skb = *pskb;
	const unsigned char *dest = eth_hdr(skb)->h_dest;

	if (p->state == BR_STATE_DISABLED)
		goto err;

	if (!is_valid_ether_addr(eth_hdr(skb)->h_source))
		goto err;

	if (p->state == BR_STATE_LEARNING)
		br_fdb_update(p->br, p, eth_hdr(skb)->h_source);

	if (p->br->stp_enabled &&
	    !memcmp(dest, bridge_ula, 5) &&
	    !(dest[5] & 0xF0)) {
		if (!dest[5]) {
			NF_HOOK(PF_BRIDGE, NF_BR_LOCAL_IN, skb, skb->dev, 
				NULL, br_stp_handle_bpdu);
			return 1;
		}
	}

	else if (p->state == BR_STATE_FORWARDING) {
#ifdef IPTV
		/* eth0 gets the multicast, broadcast or ipv6 packet and check cb[10], 
		if cb[10]=1 then pass it to upper layer */
		if (iptv_on == 1 && (!memcmp(skb->dev->name, "eth0", 4)))
		{
			if (skb->cb[10] == 1){
				//printk("Get the packet that has marked cb[10] = 1 from device %s \n", skb->dev->name);
				return 0;
			}
		}
#endif

		if (br_should_route_hook) {
			if (br_should_route_hook(pskb)) 
				return 0;
			skb = *pskb;
			dest = eth_hdr(skb)->h_dest;
		}

		if (!compare_ether_addr(p->br->dev->dev_addr, dest))
			skb->pkt_type = PACKET_HOST;

		NF_HOOK(PF_BRIDGE, NF_BR_PRE_ROUTING, skb, skb->dev, NULL,
			br_handle_frame_finish);
		return 1;
	}

err:
	kfree_skb(skb);
	return 1;
}
