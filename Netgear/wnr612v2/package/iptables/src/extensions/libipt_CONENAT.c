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
#include <linux/netfilter_ipv4/ip_nat_rule.h>

/* Function which prints out usage message. */
enum ipt_conenat_step
{
        IPT_CONENAT_DNAT = 1,
        IPT_CONENAT_IN = 2,
};
enum ipt_conenat_type
{
        IPT_CONENAT_FULL = 1,
        IPT_CONENAT_RESTRICT = 2,
};

struct ipt_conenat_info {
        enum ipt_conenat_step step;
        enum ipt_conenat_type type;     /* Related protocol */
};

static void
help(void)
{
	printf(
"CONENAT v%s options:\n"
" --conenat-step (dnat|in)\n"
"				Cone nat step\n"
" --conenat-type type (full | restrict)\n"
"				Cone nat type\n"
IPTABLES_VERSION);
}

static struct option opts[] = {
	{ "conenat-step", 1, 0, '1' },
	{ "conenat-type", 1, 0, '2' },
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
	struct ipt_conenat_info *info = (struct ipt_conenat_info *)(*target)->data;
	int timeout = 10;

	switch (c) {
	case '1':
		if (!strcasecmp(optarg, "dnat"))
			info->step = IPT_CONENAT_DNAT;
		else if (!strcasecmp(optarg, "in"))
			info->step = IPT_CONENAT_IN;
		else
			exit_error(PARAMETER_PROBLEM,
				   "unknown type `%s' specified", optarg);
		return 1;

	case '2':
		if (!strcasecmp(optarg, "full"))
			info->type = IPT_CONENAT_FULL;
		else if (!strcasecmp(optarg, "restrict"))
			info->type = IPT_CONENAT_RESTRICT;
		else
			exit_error(PARAMETER_PROBLEM,
				   "unknown protocol `%s' specified", optarg);
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
	struct ipt_conenat_info *info = (struct ipt_conenat_info *)target->data;

	printf("conenat ");
	if (info->step == IPT_CONENAT_DNAT)
		printf("step:dnat ");
	else if (info->step == IPT_CONENAT_IN)
		printf("step:in ");

	if (info->type == IPT_CONENAT_FULL)
		printf("type:full");
	else if (info->type == IPT_CONENAT_RESTRICT)
		printf("type:restricted");
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
	struct ipt_conenat_info *info = (struct ipt_conenat_info *)target->data;

	printf("--cone nat ");
	if (info->type == IPT_CONENAT_FULL)
		printf("FULL CONE NAT UDP ");
	else if (info->type == IPT_CONENAT_RESTRICT)
		printf("RESTRICTED CONE NAT UDP ");
}

static struct iptables_target conenat = {
	.next           = NULL,
    	.name           = "CONENAT",
    	.version        = IPTABLES_VERSION,
    	.size           = IPT_ALIGN(sizeof(struct ipt_conenat_info)),
    	.userspacesize  = IPT_ALIGN(sizeof(struct ipt_conenat_info)),
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
	register_target(&conenat);
}
