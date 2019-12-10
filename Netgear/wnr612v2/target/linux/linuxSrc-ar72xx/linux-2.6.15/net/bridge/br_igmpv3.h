#ifdef IPTV
#define	IGMP_REPORT_ADD		1
#define	IGMP_REPORT_LEAVE	2
#define IGMPv3_REPORT_SUCCESS -1
#define IGMP_REPORT_ERROR	-1
#define MHASH_NUM (16)
#define MHASH_MASK (0xf)
#define MAX_GROUP_NUM (16)
#define MAX_SOURCE_NUM (8)
#define MAX_PORT_NUM (2)
#define PORT_INITIAL_STATE (-1)
#define HW_INDEX_MAX (129)
#define WLMAC_CACHE_SIZE 10
#define MAX_CLEAN_COUNT 5

#define MAX_STRING_LENGTH 256
#define OP_ADD 1
#define OP_DEL 0
#define INCLUDE_MODE 1
#define EXCLUDE_MODE 0
#define LOCAL_CONTROL_START 0xE0000000
#define LOCAL_CONTROL_END   0XE00000FF
#define SSDP    0XEFFFFFFA
#define MULTICAST(x)	(((x) & htonl(0xf0000000)) == htonl(0xe0000000))
extern struct net_device *Wandev;
extern struct net_device *Brdev;
extern struct net_bridge *Br;
extern struct net_device *Eth1dev;
extern struct net_device *Eth2dev;

//#define DEBUG_MEM
#ifdef DEBUG_MEM
#define UNUSED_SIZE (1024)
#endif

struct source_set
{
	int num;
	int mode;
	__u32 list[MAX_SOURCE_NUM];
#ifdef DEBUG_MEM
	char unused[UNUSED_SIZE];
#endif
};

struct mtgroup_info
{
	__u32 group;
#ifdef DEBUG_MEM
	char unused[UNUSED_SIZE*32];
#endif
	struct __wl_mbr *wl_list;
	struct source_set port_source[MAX_PORT_NUM+1];
	struct mtgroup_info *next;
};

enum group_state
{
	st_is_ex,
	st_is_in
};

typedef struct {
//__u32 uSip;
unsigned char mac[ETH_ALEN];
unsigned char used;
unsigned char ucPortMap;	//0x80 is wirelss, b0 -b3 means port 0 -3
struct net_device *dev;
struct source_set set;
__u32 uTimeStamp;
} stIgmpMbr;

typedef struct igmpGroup{
__u32 group;
stIgmpMbr member[MAX_SOURCE_NUM];
char cPortUsed[MAX_PORT_NUM+1];
unsigned char ucPortMap;
unsigned char pad[2];
__u32 uTimeStamp;
struct igmpGroup *next; 
} stIgmpGroup;

struct server_info
{
	__u32 serverip;
	int index;
	int port_list;
#ifdef DEBUG_MEM
	char unused[UNUSED_SIZE*8];
#endif
	struct server_info *next;
};

struct multicast_cache
{
	__u32 group;
#ifdef DEBUG_MEM
	char unused[UNUSED_SIZE*16];
#endif
	struct server_info *list;
	struct multicast_cache *next;
};




struct __wl_mbr
{
	unsigned char mac[6];
	char noused[2]; // just for aligned
	unsigned long sip; // client ip address
#ifdef DEBUG_MEM
	char unused[UNUSED_SIZE*4];
#endif
	struct source_set set;
	struct net_device *dev;
	struct __wl_mbr *next;
};

static inline int get_hash_index(__u32 group)
{
	__u8 *addr = (__u8 *)&group;
	return (addr[1] + addr[2] + addr[3])&MHASH_MASK;
}

static inline int is_control(__u32 mip)
{
    if (mip >= LOCAL_CONTROL_START && mip <= LOCAL_CONTROL_END) {
        return 1;
    }
    else if (mip == SSDP) {
        return 1;
    }
    return 0;
}

static inline int include_check(struct source_set *source, __u32 serverip)
{
	int i;
	for (i = 0; i < source->num; i++)
		if (serverip == source->list[i])
			return 1;
	return 0;
}

static inline int exclude_check(struct source_set *source, __u32 serverip)
{
	int i;
	for (i = 0; i < source->num; i++)
		if (serverip == source->list[i])
			return 0;
	return 1;
}

/*
 * pass_check : return 1 for pass, return 0 for drop
 */
static inline int pass_check(struct source_set *source, __u32 serverip)
{
	if (source->mode)
		return include_check(source, serverip);
	else
		return exclude_check(source, serverip);
}

static inline unsigned char bitmask_to_id(unsigned char val)
{
	int i;
	
	for (i=0; i<8; i++) {
		if (val & (1 <<i))
			break;
	}
	return ((unsigned char)i);
}

extern void iptv_ProcessIgmpFromLan(struct sk_buff *skb);
extern void br_iptv_multicast_forward(struct net_bridge *br, struct sk_buff *skb, int clone);
extern void iptv_br_multicast(struct net_bridge *br, struct sk_buff *skb, int clone, 
		  int (*__packet_hook)(struct sk_buff *skb));
extern void iptv_igmp_gc(void);
#endif

