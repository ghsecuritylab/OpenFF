#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

#include <iptables.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/netfilter_ipv4/ipt_spiDoS.h>


#define MODULENAME	"SPIDOS"
#define SPIDOS_VER	"v1.0"

static void help(void)
{
	printf("spiDoS v%s options:\n"
		"\t\t--WANIfName <ifname>\t\tnetwork wan interface name.\n"
		"\t\t--echo-chargen\t\ttakes no values.\n"
		"\t\t--lan-addr/mask\t\t<LAN IP/LAN Mask>\n"
		MODULENAME " " SPIDOS_VER ": DNI\n",IPTABLES_VERSION);
}

static struct option opts[] = {
	{ "WANIfName", 1, 0, '1' },
	{ "echo-chargen", 0, 0, '2'},
	{ "lan-addr/mask", 1, 0, '3' },
	{0}
};

static void init(struct ipt_entry_match *m, unsigned int *nfcache)
{
	struct ipt_spiDoS *info = (struct ipt_spiDoS *)m->data;
	info->iface[0]='\0';
	info->echochargen = 0;
	info->lan_netaddr = 0;
	info->lan_netmask = 0;
}

/* Parses network address */
static void parse_net(char *arg, struct ipt_spiDoS *spi)
{
	char *slash;
	struct in_addr *ip;
	u_int32_t netmask;
	unsigned int bits;

	slash = strchr(arg, '/');
	if (slash)
		*slash = '\0';
	else
		exit_error(PARAMETER_PROBLEM, "Bad LAN IP/Mask '%s'\n",arg);

	ip = dotted_to_addr(arg);
	if (!ip)
		exit_error(PARAMETER_PROBLEM, "Bad IP address '%s'\n",arg);

	spi->lan_netaddr = ip->s_addr;
	
	if (string_to_number(slash+1, 1, 32, &bits) == -1)
		exit_error(PARAMETER_PROBLEM, "Bad netmask `%s'\n", slash+1);
	spi->lan_netmask = 0xffffffff << (32 - bits);
}

#define IPT_SPIDOS_IF		0x01
#define	IPT_SPIDOS_ECHOCHARGEN	0x02
#define IPT_SPIDOS_IPNMASK      0x04

static int parse(int c, char **argv, int invert, unsigned int *flags,
		const struct ipt_entry *entry,
		unsigned int *nfcache,
		struct ipt_entry_match **match)
{
	struct ipt_spiDoS *spi_info = (struct ipt_spiDoS *)(*match)->data;
	//printf("use net-wall instead!\n");

	switch(c) {

		/* ifname */
		case '1':
			if (invert)
				exit_error(PARAMETER_PROBLEM,
					"unexpected '!' with --WANIfName");
			if (*flags & IPT_SPIDOS_IF)
				exit_error(PARAMETER_PROBLEM,
					"Can't specify --WANIfName twice");

			strcpy(spi_info->iface, optarg);
			//printf("parse() : libipt_spiDoS: --ifname is %s\n", spi_info->iface);
			*flags |= IPT_SPIDOS_IF;
			break;
		case '2':
			if (invert)
				exit_error(PARAMETER_PROBLEM,
					"unexpected '!' with --echo-chargen");
			if (*flags & IPT_SPIDOS_ECHOCHARGEN)
				exit_error(PARAMETER_PROBLEM,
					"Can't specify --echo-chargen twice");

			spi_info->echochargen = 1;
			//printf("parse() : libipt_spiDoS: --echochargen is enable\n");
			*flags |= IPT_SPIDOS_ECHOCHARGEN;
			break;

		case '3':
			if (invert)
                                exit_error(PARAMETER_PROBLEM,
                                        "unexpected '!' with --lan-addr/mask");
                        if (*flags & IPT_SPIDOS_IPNMASK)
                                exit_error(PARAMETER_PROBLEM,
                                        "Can't specify --lan-addr/mask twice");

                        parse_net(optarg, spi_info);
                        *flags |= IPT_SPIDOS_IPNMASK;

			break;
		default:
			return 0;
	}	
	return 1;
}

static void final_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM,
			   MODULENAME" needs --%s", opts[0].name);
}

static void print(const struct ipt_ip *ip,
		const struct ipt_entry_match *match,
		int numeric)
{
	struct ipt_spiDoS *dos;

	dos = (struct ipt_spiDoS *)match->data;

	printf("spiDoS wan:%s %s",
			dos->iface,
			dos->echochargen ? ",echo-chargen enable":"");
}

/* Saves the union ipt_matchinfo in parseable form to stdout. */
static void save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	struct ipt_spiDoS *info = (struct ipt_spiDoS *)match->data;
	printf("Save spiDoS\n");
	printf("Interface \t: %s\n echo_chargen\t: %d\n", \
			info->iface, info->echochargen);
}

static struct iptables_match spiDoS = { 
	.next			= NULL,
	.name			= "spiDoS",
	.version		= IPTABLES_VERSION,
	.size			= IPT_ALIGN(sizeof(struct ipt_spiDoS)),
	.userspacesize		= IPT_ALIGN(sizeof(struct ipt_spiDoS)),
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
	register_match(&spiDoS);
}

