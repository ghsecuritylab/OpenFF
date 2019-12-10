/* Shared library add-on to iptables to add multiple TCP port support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ipt_multiip.h>
#include <arpa/inet.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"mport v%s options:\n"
" --source-ips ip[,ip:ip,ip...]\n"
" --src_ips ...\n"
"				match source ip(s)\n"
" --destination-ips ip[,ip:ip,ip...]\n"
" --dst_ips ...\n"
"				match destination ip(s)\n"
" --ips ip[,ip:ip,ip]\n"
"				match both source and destination ip(s)\n",
IPTABLES_VERSION);
}


static struct option opts[] = {
	{ "source-ips", 1, 0, '1' },
	{ "src_ips", 1, 0, '1' }, /* synonym */
	{ "destination-ips", 1, 0, '2' },
	{ "dst_ips", 1, 0, '2' }, /* synonym */
	{ "ips", 1, 0, '3' },
	{0}
};





/* Initialize the match. */
static void
init(struct ipt_entry_match *m, unsigned int *nfcache)
{
}



static u_int32_t
parse_ip(const char *ip)
{
	struct in_addr *ipaddr;

	ipaddr = dotted_to_addr(ip);
	if (!ip)
		exit_error(PARAMETER_PROBLEM, "multiip match: Bad IP address `%s'\n", ip);

	return ipaddr->s_addr;
}



static unsigned int
parse_multi_ips(const char *ipstring, u_int32_t *ips)
{
	char *buffer, *cp, *next;
	unsigned int i;

	buffer = strdup(ipstring);
	if (!buffer) exit_error(OTHER_PROBLEM, "strdup failed");

	for (cp=buffer, i=0; cp && i<IPT_MULTI_IPS; cp=next,i++)
	{
		next=strchr(cp, ',');
		if (next) *next++='\0';
		ips[i] = parse_ip(cp);
	}
	if (cp) exit_error(PARAMETER_PROBLEM, "too many ips specified");
	free(buffer);
	return i;
}



/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      unsigned int *nfcache,
      struct ipt_entry_match **match)
{
	struct ipt_multiip *multiinfo
		= (struct ipt_multiip *)(*match)->data;

	switch (c) {
	case '1':
		multiinfo->count = parse_multi_ips(argv[optind-1], multiinfo->ips);
		multiinfo->flags = IPT_MULTIIP_SOURCE;
		break;

	case '2':
		multiinfo->count = parse_multi_ips(argv[optind-1], multiinfo->ips);
		multiinfo->flags = IPT_MULTIIP_DESTINATION;
		break;

	default:
		return 0;
	}

	if (*flags)
		exit_error(PARAMETER_PROBLEM,
			   "multiip can only have one option");
	*flags = 1;
	return 1;
}





/* Final check; must specify something. */
static void
final_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM, "multiip expection an option");
}




/* Prints out the matchinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_match *match,
      int numeric)
{
	const struct ipt_multiip *multiinfo
		= (const struct ipt_multiip *)match->data;
	unsigned int i;
	struct in_addr addr;

	printf("multiip ");

	switch (multiinfo->flags) {
	case IPT_MULTIIP_SOURCE:
		printf("src_ips ");
		break;

	case IPT_MULTIIP_DESTINATION:
		printf("dst_ips ");
		break;

	default:
		printf("ERROR ");
		break;
	}

	for (i=0; i < multiinfo->count; i++) {
		printf("%s", i ? "," : "");
		addr.s_addr = multiinfo->ips[i];
		printf("%s", inet_ntoa(addr));
	}
	printf(" ");
}



/* Saves the union ipt_matchinfo in parsable form to stdout. */
static void save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	const struct ipt_multiip *multiinfo
		= (const struct ipt_multiip *)match->data;
	unsigned int i;
	struct in_addr addr;

	switch (multiinfo->flags) {
	case IPT_MULTIIP_SOURCE:
		printf("--src_ips ");
		break;

	case IPT_MULTIIP_DESTINATION:
		printf("--dst_ips ");
		break;
	}

	for (i=0; i < multiinfo->count; i++) {
		printf("%s", i ? "," : "");
		addr.s_addr = multiinfo->ips[i];
		printf("%s", inet_ntoa(addr));
	}
	printf(" ");
}




struct iptables_match multiip
= { .next 		= NULL,
    .name           = "multiip",
    .version        = IPTABLES_VERSION,
    .size           = IPT_ALIGN(sizeof(struct ipt_multiip)),
    .userspacesize  = IPT_ALIGN(sizeof(struct ipt_multiip)),
    .help           = &help,
    .init           = &init,
    .parse          = &parse,
    .final_check    = &final_check,
    .print          = &print,
    .save           =&save,
    .extra_opts     = opts
};

void
_init(void)
{
	register_match(&multiip);
}
