#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include <iptables.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/netfilter_ipv4/ipt_spiDoS.h>


#define MODULENAME      "SPITCPSCAN"
#define SPIDOS_VER      "v1.0"

static void help(void)
{
        printf("spiTcpScan v%s options:\n"
		"\t\t--WANIfName <ifname>\t\tnetwork wan interface name.\n"
		"\t\t[--check | --count ]\t\tto check syn packet or count syn-ack packet\n"
		MODULENAME " " SPIDOS_VER ": DNI\n", IPTABLES_VERSION);
}

static struct option opts[] = {
	{ "WANIfName", 1, 0, '1' },
	{ "check", 0, 0, '2' },
	{ "count", 0, 0, '3' },
	{0}
};

static void init(struct ipt_entry_match *m, unsigned int *nfcache)
{
	struct ipt_spiTcpScan *info = (struct ipt_spiTcpScan *)m->data;
	info->iface[0] = '\0';
	info->state = 0;
}

#define IPT_SPIDOS_IF		0x01
#define IPT_SPIDOS_STATE	0x02

static int parse(int c, char **argv, int invert, unsigned int *flags,
		const struct ipt_entry *entry,
		unsigned int *nfcache,
		struct ipt_entry_match **match)
{
	struct ipt_spiTcpScan *spi_info = (struct ipt_spiTcpScan *)(*match)->data;

	switch(c) {
		case '1':
			if (invert)
				exit_error(PARAMETER_PROBLEM,"unexpected '!' with --ifname");
			if (*flags & IPT_SPIDOS_IF)
				exit_error(PARAMETER_PROBLEM,"Can't specify --ifname twice");

			strcpy(spi_info->iface, optarg);
			*flags |= IPT_SPIDOS_IF;
			break;

		case '2':
			if (invert)
				exit_error(PARAMETER_PROBLEM,"unexpected '!' with --ifname");
			if (*flags & IPT_SPIDOS_STATE)
				exit_error(PARAMETER_PROBLEM,"Can't specify --check/--count twice");
			spi_info->state = TCP_SCAN_CHECK;
			*flags |= IPT_SPIDOS_STATE;

			break;

		case '3':
			if (invert)
				exit_error(PARAMETER_PROBLEM,"unexpected '!' with --ifname");
			if (*flags & IPT_SPIDOS_STATE)
				exit_error(PARAMETER_PROBLEM,"Can't specify --check/--count twice");
			spi_info->state = TCP_SCAN_COUNT;
			*flags |= IPT_SPIDOS_STATE;

			break;

		default:
			return 0;
	}	
	return 1;
}

static void final_check(unsigned int flags)
{
	if (!(flags & IPT_SPIDOS_IF))
		exit_error(PARAMETER_PROBLEM,
			   MODULENAME" needs --%s", opts[0].name);
	if (!(flags & IPT_SPIDOS_STATE))
		exit_error(PARAMETER_PROBLEM,
			   MODULENAME" needs --%s or --%s", opts[1].name, opts[2].name);
}

static void print(const struct ipt_ip *ip,
		const struct ipt_entry_match *match,
		int numeric)
{
	struct ipt_spiTcpScan *dos;
	dos = (struct ipt_spiTcpScan *)match->data;
	printf("spiTcpScan wan:%s --%s",dos->iface, 
		(dos->state==TCP_SCAN_CHECK)? opts[1].name: opts[2].name);
}

/* Saves the union ipt_matchinfo in parseable form to stdout. */
static void save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	struct ipt_spiTcpScan *info = (struct ipt_spiTcpScan *)match->data;
	printf("Save spiTcpScan\n");
	printf("Interface \t: %s --%s\n",\
			info->iface, (info->state==TCP_SCAN_CHECK)? opts[1].name: opts[2].name);
}

static struct iptables_match spiTcpScan = { 
	.next			= NULL,
	.name			= "spiTcpScan",
	.version		= IPTABLES_VERSION,
	.size			= IPT_ALIGN(sizeof(struct ipt_spiTcpScan)),
	.userspacesize		= IPT_ALIGN(sizeof(struct ipt_spiTcpScan)),
	.help			= &help,
	.init           	= &init,
	.parse          	= &parse,
	.final_check		= &final_check,
	.print			= &print,
	.save			= &save,
    	.extra_opts		= opts
};

void _init(void)
{
	register_match(&spiTcpScan);
}

