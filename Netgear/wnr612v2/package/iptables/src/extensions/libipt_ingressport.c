/* Shared library add-on to iptables to add packet length matching support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <iptables.h>
#include <linux/netfilter_ipv4/ipt_ingressport.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf(
"ingressport v%s options:\n"
"--ingressport PortNumber		Match physical port based QOS port number \n"
"\n",
IPTABLES_VERSION);

}
  
static struct option opts[] = {
	{ "ingressport", 1, 0, '1' },
	{0}
};


/* If a single value is provided, min and max are both set to the value */
static void
parse_port(const char *s, struct ipt_ingressport_info *info)
{
	unsigned int len;

	if (string_to_number(s, 0, 0xFFFF, &len) == -1)
		exit_error(PARAMETER_PROBLEM, "port number invalid: `%s'\n", s);

	if ((len < 0 ) || (len > PHYSICAL_PORT_NUMBER))
		exit_error(PARAMETER_PROBLEM, "port number specify invalid: `%s', must between 1 - %d. \n", s, PHYSICAL_PORT_NUMBER);
	else
		info->port_num = (u_int8_t)len;
}

/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      unsigned int *nfcache,
      struct ipt_entry_match **match)
{
	struct ipt_ingressport_info *info = (struct ipt_ingressport_info *)(*match)->data;

	switch (c) {
		case '1':
			if (*flags)
				exit_error(PARAMETER_PROBLEM,
				           "ingressport: `--ingressport' may only be "
				           "specified once");
			parse_port(argv[optind-1], info);
			*flags = 1;
			break;
			
		default:
			return 0;
	}
	return 1;
}

/* Final check; must have specified --length. */
static void
final_check(unsigned int flags)
{
	if (!flags)
		exit_error(PARAMETER_PROBLEM,
			   "ingressport: You must specify `--ingressport'");
}

/* Common match printing code. */
static void
print_port(struct ipt_ingressport_info *info)
{
	printf("%u ", info->port_num);
}

/* Prints out the matchinfo. */
static void
print(const struct ipt_ip *ip,
      const struct ipt_entry_match *match,
      int numeric)
{
	printf("ingressport ");
	print_port((struct ipt_ingressport_info *)match->data);
}

/* Saves the union ipt_matchinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_match *match)
{
	printf("--ingressport ");
	print_port((struct ipt_ingressport_info *)match->data);
}

static struct iptables_match ingressport = { 
	.next		= NULL,
	.name		= "ingressport",
	.version	= IPTABLES_VERSION,
	.size		= IPT_ALIGN(sizeof(struct ipt_ingressport_info)),
	.userspacesize	= IPT_ALIGN(sizeof(struct ipt_ingressport_info)),
	.help		= &help,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts
};

void _init(void)
{
	register_match(&ingressport);
}
