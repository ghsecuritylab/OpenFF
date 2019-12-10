/*-
 * Copyright (c) 2003-2005 Sam Leffler, Errno Consulting
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	$Id: //depot/sw/releases/7.3_AP/wlan/linux/net80211/ieee80211_linux.h#11 $
 */
#ifndef _NET80211_IEEE80211_LINUX_H_
#define _NET80211_IEEE80211_LINUX_H_

#ifdef CONFIG_NET_WIRELESS
#include <linux/wireless.h>
#endif

#ifdef ATH_WLAN_COMBINE
#define EXPORT_SYMBOL_C(_sym)
#define PRIVATE_C
#else
#define EXPORT_SYMBOL_C(_sym) EXPORT_SYMBOL(_sym)
#define PRIVATE_C static
#endif /* #ifdef ATH_WLAN_COMBINE */

/*
 * Beacon handler locking definitions.
 */
typedef spinlock_t ieee80211com_lock_t;

#define	IEEE80211_LOCK_INIT(_ic, _name)				\
	spin_lock_init(&(_ic)->ic_comlock)

#define	IEEE80211_LOCK_DESTROY(_ic)

#define	IEEE80211_LOCK(_ic) do {				\
	unsigned long __ilockflags;				\
	spin_lock_irqsave(&(_ic)->ic_comlock, __ilockflags);

#define	IEEE80211_UNLOCK(_ic)					\
	spin_unlock_irqrestore(&(_ic)->ic_comlock, __ilockflags);	\
} while (0)

/* NB: beware, spin_is_locked() is unusable for !SMP */
#if defined(CONFIG_SMP)
#define	IEEE80211_LOCK_ASSERT(_ic) \
	KASSERT(spin_is_locked(&(_ic)->ic_comlock),("ieee80211com not locked!"))
#else
#define	IEEE80211_LOCK_ASSERT(_ic)
#endif

#define IEEE80211_VAPS_LOCK_INIT(_ic, _name)		\
	spin_lock_init(&(_ic)->ic_vapslock)
#define IEEE80211_VAPS_LOCK_DESTROY(_ic)
#define IEEE80211_VAPS_LOCK(_ic) do {				\
	unsigned long __vlockflags=0;				\
	unsigned int __vlocked=0;  				\
	__vlocked=spin_is_locked(&(_ic)->ic_vapslock);		\
	if(!__vlocked) spin_lock_irqsave(&(_ic)->ic_vapslock, __vlockflags);
#define IEEE80211_VAPS_UNLOCK(_ic)					\
	if(!__vlocked) spin_unlock_irqrestore(&(_ic)->ic_vapslock, __vlockflags);	\
} while (0)
/* NB: beware, spin_is_locked() is unusable for !SMP */
#if defined(CONFIG_SMP)
#define IEEE80211_VAPS_LOCK_ASSERT(_ic) \
	KASSERT(spin_is_locked(&(_ic)->ic_vapslock),("ieee80211com_vaps not locked!"))
#else
#define IEEE80211_VAPS_LOCK_ASSERT(_ic)
#endif

/*
 * Beacon locking definitions; piggyback on com lock.
 */
#define	IEEE80211_BEACON_LOCK(_ic)	   IEEE80211_LOCK(_ic)
#define	IEEE80211_BEACON_UNLOCK(_ic)	   IEEE80211_UNLOCK(_ic)
#define	IEEE80211_BEACON_LOCK_ASSERT(_ic)  IEEE80211_LOCK_ASSERT(_ic)

/*
 * UAPSD locking definitions; piggyback on com lock.
 */
#define	IEEE80211_UAPSD_LOCK(_ic)	   IEEE80211_LOCK(_ic)
#define	IEEE80211_UAPSD_UNLOCK(_ic)	   IEEE80211_UNLOCK(_ic)
#define	IEEE80211_UAPSD_LOCK_ASSERT(_ic)   IEEE80211_LOCK_ASSERT(_ic)

/*
 * Node locking definitions.
 */
typedef rwlock_t ieee80211_node_lock_t;
#define	IEEE80211_NODE_LOCK_INIT(_nt, _name)	rwlock_init(&(_nt)->nt_nodelock)
#define	IEEE80211_NODE_LOCK_DESTROY(_nt)
#ifndef ATH_SUPPORT_UAPSD
#define	IEEE80211_NODE_LOCK(_nt)	write_lock(&(_nt)->nt_nodelock)
#define	IEEE80211_NODE_UNLOCK(_nt)	write_unlock(&(_nt)->nt_nodelock)
#define	IEEE80211_NODE_LOCK_BH(_nt)			\
	if (irqs_disabled()) {				\
		write_lock(&(_nt)->nt_nodelock);	\
	} else {					\
		write_lock_bh(&(_nt)->nt_nodelock); 	\
	}
#define	IEEE80211_NODE_UNLOCK_BH(_nt)			\
	if (irqs_disabled()) {				\
		write_unlock(&(_nt)->nt_nodelock);	\
	} else {					\
		write_unlock_bh(&(_nt)->nt_nodelock);	\
	}
#else
#define	IEEE80211_NODE_LOCK(_nt)	\
	write_lock_irqsave((&(_nt)->nt_nodelock), (_nt)->nt_node_lock_flags)
#define	IEEE80211_NODE_UNLOCK(_nt)	\
	write_unlock_irqrestore((&(_nt)->nt_nodelock), (_nt)->nt_node_lock_flags)
#define IEEE80211_NODE_LOCK_BH(_nt) IEEE80211_NODE_LOCK(_nt)
#define IEEE80211_NODE_UNLOCK_BH(_nt) IEEE80211_NODE_UNLOCK(_nt)
#endif
/* NB: beware, *_is_locked() are boguly defined for UP+!PREEMPT */
#if (defined(CONFIG_SMP) || defined(CONFIG_PREEMPT)) && defined(rwlock_is_locked)
#define	IEEE80211_NODE_LOCK_ASSERT(_nt) \
	KASSERT(rwlock_is_locked(&(_nt)->nt_nodelock), \
		("802.11 node table not locked!"))
#else
#define	IEEE80211_NODE_LOCK_ASSERT(_nt)
#endif

/*
 * Node table scangen locking definitions.
 */
typedef spinlock_t ieee80211_scan_lock_t;
#define	IEEE80211_SCAN_LOCK_INIT(_nt, _name) spin_lock_init(&(_nt)->nt_scanlock)
#define	IEEE80211_SCAN_LOCK_DESTROY(_nt)
#define	IEEE80211_SCAN_LOCK_IRQ(_nt)		\
	spin_lock_irqsave((&(_nt)->nt_scanlock), (_nt)->nt_scan_lock_flags)
#define	IEEE80211_SCAN_UNLOCK_IRQ(_nt)		\
	spin_unlock_irqrestore((&(_nt)->nt_scanlock), (_nt)->nt_scan_lock_flags)
#define	IEEE80211_SCAN_LOCK(_nt)  IEEE80211_SCAN_LOCK_IRQ(_nt)
#define	IEEE80211_SCAN_UNLOCK(_nt)  IEEE80211_SCAN_UNLOCK_IRQ(_nt)

/* NB: beware, spin_is_locked() is unusable for !SMP */
#if defined(CONFIG_SMP)
#define	IEEE80211_SCAN_LOCK_ASSERT(_nt) \
	KASSERT(spin_is_locked(&(_nt)->nt_scanlock), ("scangen not locked!"))
#else
#define	IEEE80211_SCAN_LOCK_ASSERT(_nt)
#endif

/*
 * 802.1x MAC ACL database locking definitions.
 */
typedef spinlock_t acl_lock_t;
#define	ACL_LOCK_INIT(_as, _name)	spin_lock_init(&(_as)->as_lock)
#define	ACL_LOCK_DESTROY(_as)
#define	ACL_LOCK(_as)			spin_lock_bh(&(_as)->as_lock)
#define	ACL_UNLOCK(_as)			spin_unlock_bh(&(_as)->as_lock)
/* NB: beware, spin_is_locked() is unusable for !SMP */
#if defined(CONFIG_SMP)
#define	ACL_LOCK_ASSERT(_as) \
	KASSERT(spin_is_locked(&(_as)->as_lock), ("ACL not locked!"))
#else
#define	ACL_LOCK_ASSERT(_as)
#endif

/*
 * Per-node power-save queue definitions.  Beware of control
 * flow with IEEE80211_NODE_SAVEQ_LOCK/IEEE80211_NODE_SAVEQ_UNLOCK.
 */
#define	IEEE80211_NODE_SAVEQ_INIT(_ni, _name) do {		\
	skb_queue_head_init(&(_ni)->ni_savedq);			\
} while (0)
#define	IEEE80211_NODE_SAVEQ_DESTROY(_ni)
#define	IEEE80211_NODE_SAVEQ_QLEN(_ni)	skb_queue_len(&(_ni)->ni_savedq)
#define	IEEE80211_NODE_SAVEQ_LOCK(_ni) do {			\
	unsigned long __sqlockflags;				\
	spin_lock_irqsave(&(_ni)->ni_savedq.lock, __sqlockflags);
#define	IEEE80211_NODE_SAVEQ_UNLOCK(_ni)			\
	spin_unlock_irqrestore(&(_ni)->ni_savedq.lock, __sqlockflags);\
} while (0)
#define	IEEE80211_NODE_SAVEQ_DEQUEUE(_ni, _skb, _qlen) do {	\
	IEEE80211_NODE_SAVEQ_LOCK(_ni);				\
	_skb = __skb_dequeue(&(_ni)->ni_savedq);		\
	(_qlen) = skb_queue_len(&(_ni)->ni_savedq);		\
	IEEE80211_NODE_SAVEQ_UNLOCK(_ni);			\
} while (0)
#define	_IEEE80211_NODE_SAVEQ_ENQUEUE(_ni, _skb, _qlen, _age) do {\
	struct sk_buff *tail = skb_peek_tail(&(_ni)->ni_savedq);\
	if (tail != NULL) {					\
		_age -= M_AGE_GET(tail);			\
		__skb_append(tail, _skb);			\
	} else { 						\
		__skb_queue_head(&(_ni)->ni_savedq, _skb);	\
	}							\
	M_AGE_SET(_skb, _age);					\
	(_qlen) = skb_queue_len(&(_ni)->ni_savedq); 		\
} while (0)


/*
 * Per-vap scan -save queue definitions.
 */
#define	IEEE80211_SCAN_SAVEQ_INIT(_vap, _name) do {		\
	skb_queue_head_init(&(_vap)->iv_scan_savedq);			\
} while (0)
#define	IEEE80211_SCAN_SAVEQ_DESTROY(_vap)
#define	IEEE80211_SCAN_SAVEQ_QLEN(_vap)	skb_queue_len(&(_vap)->iv_scan_savedq)

#define	IEEE80211_SCAN_SAVEQ_LOCK(_vap) do {			\
	unsigned long __sqlockflags;				\
	spin_lock_irqsave(&(_vap)->iv_scan_savedq.lock, __sqlockflags);
#define	IEEE80211_SCAN_SAVEQ_UNLOCK(_vap)			\
	spin_unlock_irqrestore(&(_vap)->iv_scan_savedq.lock, __sqlockflags);\
} while (0)

#define	IEEE80211_SCAN_SAVEQ_DEQUEUE(_vap, _skb, _qlen) do {	\
	IEEE80211_SCAN_SAVEQ_LOCK(_vap);				\
	_skb = __skb_dequeue(&(_vap)->iv_scan_savedq);		\
	(_qlen) = skb_queue_len(&(_vap)->iv_scan_savedq);		\
	IEEE80211_SCAN_SAVEQ_UNLOCK(_vap);			\
} while (0)

/*
 * Transmitted frames have the following information
 * held in the sk_buff control buffer.  This is used to
 * communicate various inter-procedural state that needs
 * to be associated with the frame for the duration of
 * it's existence.
 *
 * NB: sizeof(cb) == 48 and the vlan code grabs the first
 *     8 bytes so we reserve/avoid it.
 */
struct ieee80211_cb {
	u_int8_t		vlan[8];	/* reserve for vlan tag info */
	struct ieee80211_node	*ni;
	void			*context;	/* pointer to context area */
	u_int32_t		flags;
#define	M_LINK0		0x01			/* frame needs WEP encryption */
#define	M_FF		0x02			/* fast frame */
#define	M_PWR_SAV	0x04			/* bypass power save handling */
#define M_UAPSD		0x08			/* frame flagged for u-apsd handling */
#define M_EAPOL		0x10			/* frame flagged for EAPOL handling */
#define M_AMSDU		0x20			/* frame flagged for AMSDU handling */
#define	M_NULL_PWR_SAV	0x40		/* null data with power save bit on */
#define M_PROBING	0x80			/* frame flagged as a probing one */
#define M_DHCP  	0x100			/* frame flagged as a DHCP  */
	u_int8_t		u_tid;		    /* user priority from vlan/ip tos   */
    /* Do not move these 2 fields - their relative location from 
     * beginning of the structure has to be 21 
     */
    u_int8_t       offset;          /* offset at which ccmp pn is put in wbuf */
    u_int8_t       pktnum[8];       /* copy ccmp pn put in wbuf */
#ifdef LLC_HDR_WAR
	u_int8_t		u_tid_in;	/* incoming user priority from .11 qos field   */
#define CB_TID_IN_MARK  0x80        /* incoming tid set */
#define CB_TID_IN_MASK  0x0f        /* incoming tid value mask */
#endif
};

#define M_FLAG_SET(_skb, _flag) \
	(((struct ieee80211_cb *)(_skb)->cb)->flags |= (_flag))
#define	M_FLAG_CLR(_skb, _flag) \
	(((struct ieee80211_cb *)(_skb)->cb)->flags &= ~(_flag))
#define	M_FLAG_GET(_skb, _flag) \
	(((struct ieee80211_cb *)(_skb)->cb)->flags & (_flag))
#define M_FLAG_KEEP_ONLY(_skb, _flag) \
	(((struct ieee80211_cb *)(_skb)->cb)->flags &= (_flag))

#define	M_PWR_SAV_SET(skb) M_FLAG_SET((skb), M_PWR_SAV)
#define	M_PWR_SAV_CLR(skb) M_FLAG_CLR((skb), M_PWR_SAV)
#define	M_PWR_SAV_GET(skb) M_FLAG_GET((skb), M_PWR_SAV)

#define	M_NULL_PWR_SAV_SET(skb) M_FLAG_SET((skb), M_NULL_PWR_SAV)
#define	M_NULL_PWR_SAV_CLR(skb) M_FLAG_CLR((skb), M_NULL_PWR_SAV)
#define	M_NULL_PWR_SAV_GET(skb) M_FLAG_GET((skb), M_NULL_PWR_SAV)

/*
 * Skbufs on the power save queue are tagged with an age and
 * timed out.  We reuse the hardware checksum field in the
 * mbuf packet header to store this data.
 * XXX use private cb area
 */
#define	M_AGE_SET(skb,v)	(skb->csum = v)
#define	M_AGE_GET(skb)		(skb->csum)
#define	M_AGE_SUB(skb,adj)	(skb->csum -= adj)

struct ieee80211com;
struct ieee80211vap;

int	ieee80211_load_module(const char *);

/*
 * Node reference counting definitions.
 *
 * ieee80211_node_initref	initialize the reference count to 1
 * ieee80211_node_incref	add a reference
 * ieee80211_node_decref	remove a reference
 * ieee80211_node_dectestref	remove a reference and return 1 if this
 *				is the last reference, otherwise 0
 * ieee80211_node_refcnt	reference count for printing (only)
 */
#define ieee80211_node_initref(_ni) \
	atomic_set(&(_ni)->ni_refcnt, 1)
#define ieee80211_node_incref(_ni) \
	atomic_inc(&(_ni)->ni_refcnt)
#define	ieee80211_node_decref(_ni) \
	atomic_dec(&(_ni)->ni_refcnt)
#define	ieee80211_node_dectestref(_ni) \
	atomic_dec_and_test(&(_ni)->ni_refcnt)
#define	ieee80211_node_refcnt(_ni)	(_ni)->ni_refcnt.counter

#define	le16toh(_x)	le16_to_cpu(_x)
#define	htole16(_x)	cpu_to_le16(_x)
#define	le32toh(_x)	le32_to_cpu(_x)
#define	htole32(_x)	cpu_to_le32(_x)
#define	be32toh(_x)	be32_to_cpu(_x)
#define	htobe32(_x)	cpu_to_be32(_x)

/*
 * Linux has no equivalents to malloc types so null these out.
 */
#define	MALLOC_DEFINE(type, shortdesc, longdesc)
#define	MALLOC_DECLARE(type)

/*
 * flags to malloc.
 */
#define	M_NOWAIT	0x0001		/* do not block */
#define	M_WAITOK	0x0002		/* ok to block */
#define	M_ZERO		0x0100		/* bzero the allocation */

static __inline void *
ieee80211_malloc(size_t size, int flags)
{
	void *p = kmalloc(size, flags & M_NOWAIT ? GFP_ATOMIC : GFP_KERNEL);
	if (p && (flags & M_ZERO))
		memset(p, 0, size);
	return p;
}
#define	MALLOC(_ptr, cast, _size, _type, _flags) \
	((_ptr) = (cast)ieee80211_malloc(_size, _flags))
#define	FREE(addr, type)	kfree((addr))

/*
 * This unlikely to be popular but it dramatically reduces diffs.
 */
#define	printf(...)	printk(__VA_ARGS__)
struct ieee80211com;
extern	void if_printf(struct net_device *, const char *, ...);
extern	const char *ether_sprintf(const u_int8_t *);

/*
 * Queue write-arounds and support routines.
 */
extern	struct sk_buff *ieee80211_getmgtframe(u_int8_t **frm, u_int pktlen);
#define	IF_ENQUEUE(_q,_skb)	skb_queue_tail(_q,_skb)
#define	IF_DEQUEUE(_q,_skb)	(_skb = skb_dequeue(_q))
#define	_IF_QLEN(_q)		skb_queue_len(_q)
#define	IF_DRAIN(_q)		skb_queue_drain(_q)
extern	void skb_queue_drain(struct sk_buff_head *q);

#ifndef __MOD_INC_USE_COUNT
#define	_MOD_INC_USE(_m, _err)						\
	if (!try_module_get(_m)) {					\
		printk(KERN_WARNING "%s: try_module_get failed\n",	\
			__func__); \
		_err;							\
	}
#define	_MOD_DEC_USE(_m)		module_put(_m)
#else
#define	_MOD_INC_USE(_m, _err)	MOD_INC_USE_COUNT
#define	_MOD_DEC_USE(_m)	MOD_DEC_USE_COUNT
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
static __inline u_int64_t
get_jiffies_64(void)
{
	return (u_int64_t) jiffies;		/* XXX not right */
}
#endif
/* XXX not in 2.6.6 but is in 2.6.10; might have appeared earlier */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,9)
static __inline unsigned long msecs_to_jiffies(const unsigned int m)
{
	return (m * HZ + 999) / 1000;
}
#endif

#ifndef CLONE_KERNEL
/*
 * List of flags we want to share for kernel threads,
 * if only because they are not used by them anyway.
 */
#define CLONE_KERNEL	(CLONE_FS | CLONE_FILES | CLONE_SIGHAND)
#endif

#include <linux/mm.h>
#ifndef offset_in_page
#define	offset_in_page(p) ((unsigned long) (p) & ~PAGE_MASK)
#endif

#ifndef module_put_and_exit
#define module_put_and_exit(code) do {	\
	_MOD_DEC_USE(THIS_MODULE);	\
	do_exit(code);			\
} while (0)
#endif

/*
 * Linux uses __BIG_ENDIAN and __LITTLE_ENDIAN while BSD uses _foo
 * and an explicit _BYTE_ORDER.  Sorry, BSD got there first--define
 * things in the BSD way...
 */
#define	_LITTLE_ENDIAN	1234	/* LSB first: i386, vax */
#define	_BIG_ENDIAN	4321	/* MSB first: 68000, ibm, net */
#include <asm/byteorder.h>
#if defined(__LITTLE_ENDIAN)
#define	_BYTE_ORDER	_LITTLE_ENDIAN
#elif defined(__BIG_ENDIAN)
#define	_BYTE_ORDER	_BIG_ENDIAN
#else
#error "Please fix asm/byteorder.h"
#endif

#ifdef CONFIG_SYSCTL
/*
 * Deal with the sysctl handler api changing.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,8)
#define	IEEE80211_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
	f(ctl_table *ctl, int write, struct file *filp, void *buffer, \
		size_t *lenp)
#define	IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
	proc_dointvec(ctl, write, filp, buffer, lenp)
#define IEEE80211_SYSCTL_PROC_DOSTRING(ctl, write, filp, buffer, lenp, ppos) \
	proc_dostring(ctl, write, filp, buffer, lenp)
#else
#define	IEEE80211_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
	f(ctl_table *ctl, int write, struct file *filp, void *buffer,\
		size_t *lenp, loff_t *ppos)
#define	IEEE80211_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
	proc_dointvec(ctl, write, filp, buffer, lenp, ppos)
#define IEEE80211_SYSCTL_PROC_DOSTRING(ctl, write, filp, buffer, lenp, ppos) \
	proc_dostring(ctl, write, filp, buffer, lenp, ppos)
#endif

void	ieee80211_sysctl_vattach(struct ieee80211vap *);
void	ieee80211_sysctl_vdetach(struct ieee80211vap *);
#endif /* CONFIG_SYSCTL */

#if defined(CONFIG_VLAN_8021Q) || defined(CONFIG_VLAN_8021Q_MODULE)
#define IEEE80211_VLAN_TAG_USED 1

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,4,20)
#define	vlan_hwaccel_receive_skb(skb, grp, tag)	vlan_hwaccel_rx(skb, grp, tag)
#else  /* for 2.6 and above kernels */
#define	vlan_hwaccel_receive_skb(skb, grp, tag)	vlan_hwaccel_rx(skb, grp, tag)
#endif
#else
#define IEEE80211_VLAN_TAG_USED 0
#endif
void	ieee80211_vlan_vattach(struct ieee80211vap *);
void	ieee80211_vlan_vdetach(struct ieee80211vap *);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
#define	free_netdev(dev)	kfree(dev)
#endif

void	ieee80211_ioctl_vattach(struct ieee80211vap *);
void	ieee80211_ioctl_vdetach(struct ieee80211vap *);
struct ifreq;
int	ieee80211_ioctl_create_vap(struct ieee80211com *, struct ifreq *);

/*
 * For packet capture, define the same physical layer packet header 
 * structure as used in the wlan-ng driver 
 */
typedef struct {
	u_int32_t did;
	u_int16_t status;
	u_int16_t len;
	u_int32_t data;
} p80211item_uint32_t;
typedef struct {
	u_int32_t msgcode;
	u_int32_t msglen;
#define WLAN_DEVNAMELEN_MAX 16
	u_int8_t devname[WLAN_DEVNAMELEN_MAX];
	p80211item_uint32_t hosttime;
	p80211item_uint32_t mactime;
	p80211item_uint32_t channel;
	p80211item_uint32_t rssi;
	p80211item_uint32_t sq;
	p80211item_uint32_t signal;
	p80211item_uint32_t noise;
	p80211item_uint32_t rate;
	p80211item_uint32_t istx;
	p80211item_uint32_t frmlen;
} wlan_ng_prism2_header;

#endif /* _NET80211_IEEE80211_LINUX_H_ */
