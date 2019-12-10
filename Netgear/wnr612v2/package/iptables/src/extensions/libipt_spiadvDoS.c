#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include <iptables.h>

struct ipt_spiadvDoS
{
	void *unused;
};

static void help(void)
{
	printf(
"spiadvDoS match v%s takes no options\n",
IPTABLES_VERSION);}

static struct option opts[] = {
	{0}
};

static void init(struct ipt_entry_match *m, unsigned int *nfcache)
{
}

static int parse(int c, char **argv, int invert, unsigned int *flags,
		const struct ipt_entry *entry,
		unsigned int *nfcache,
		struct ipt_entry_match **match)
{
	return 0;
}

static void final_check(unsigned int flags)
{
}

static void print(const struct ipt_ip *ip,
		const struct ipt_entry_match *match,
		int numeric)
{
	printf("spiadvDoS : SYN Flood");
}

/* Saves the union ipt_matchinfo in parseable form to stdout. */
static void save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
}


static struct iptables_match spiadvDoS = { 
	.next			= NULL,
	.name			= "spiadvDoS",
	.version			= IPTABLES_VERSION,
	.size			= IPT_ALIGN(0),
	.userspacesize	= IPT_ALIGN(0),
	.help			= &help,
	.init			= &init,
	.parse			= &parse,
	.final_check		= &final_check,
	.print			= &print,
	.save			= &save,
    .extra_opts		= opts
};

void _init(void)
{
	register_match(&spiadvDoS);
}