/* CONENAT target. 
 *
 * Copyright  2007-5-11, DNI
 * All Rights Reserved.
 */

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_vlan_mark.h>

static void
help(void)
{
	printf(
"vlan-mark v%s options:\n"
" --vid (0~4095)\n"
" --priority (0-7)\n"
IPTABLES_VERSION);
}

static struct option opts[] = {
	{ "priority", 1, 0, '1' },
	{ "vid", 1, 0, '2' },
	{ 0 }
};

/* Initialize the target. */
static void
init(struct ipt_entry_target *t, unsigned int *nfcache)
{
	/* Can't cache this */
	*nfcache |= NFC_UNKNOWN;
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      struct ipt_entry_target **target)
{
	struct ipt_vlan_info *info = (struct ipt_vlan_info *)(*target)->data;
	int timeout = 10;
    int temp;
	switch (c) {
    case '1':       //priority
        temp = atoi(optarg);
        if(temp >= PRIORITY_MIN && temp <= PRIORITY_MAX)
            info->priority = temp;
		else
			exit_error(PARAMETER_PROBLEM,
				   "priority %s out of range", optarg);
		return 1;

	case '2':
        temp = atoi(optarg);
        if(temp >= VID_MIN && temp <= VID_MAX)
            info->vid = temp;
		else
			exit_error(PARAMETER_PROBLEM,
				   "priority %s out of range", optarg);
		return 1;
	default:
		return 0;
	}
}

/* Final check; don't care. */
static void final_check(unsigned int flags)
{
}

/* Prints out the targinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_target *target,
      int numeric)
{
	struct ipt_vlan_info *info = (struct ipt_vlan_info *)target->data;

	printf("priority = %d\n",info->priority);
	printf("vid = %d\n",info->vid);
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
	struct ipt_vlan_info *info = (struct ipt_vlan_info *)target->data;
	printf("Save VLAN\n");
	printf("priority = %d\n",info->priority);
	printf("vid = %d\n",info->vid);
}

static struct iptables_target vlan = {
	.next           = NULL,
    	.name           = "vlan-mark",
    	.version        = IPTABLES_VERSION,
    	.size           = IPT_ALIGN(sizeof(struct ipt_vlan_info)),
    	.userspacesize  = IPT_ALIGN(sizeof(struct ipt_vlan_info)),
    	.help           = &help,
    	.init           = &init,
    	.parse          = &parse,
    	.final_check    = &final_check,
    	.print          = &print,
    	.save           = &save,
    	.extra_opts     = opts
};

void _init(void)
{
	register_target(&vlan);
}
