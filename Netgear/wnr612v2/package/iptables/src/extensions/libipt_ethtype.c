/* Shared library add-on to iptables to add IP range matching support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include <iptables.h>
#include <linux/netfilter_ipv4/ipt_ethtype.h>



static void
help(void)
{
	printf(
"ethtype v%s options:\n"
" --proto \n"
IPTABLES_VERSION);
}



static struct option opts[] = {
	{ "proto", 1, 0, '1' },
	{0}
};


static char c2i(char etype)
{
	if ((etype >= 48) && (etype <= 57))
	{
		return (etype-48);
	}
	else if ((etype >= 65) && (etype <= 70))
	{
		return (etype-55);
	}
	else if ((etype >= 97) && (etype <= 102))
	{
		return (etype-87);
	}

    return -1;
}


/* Function which parses command options; returns true if it
   ate an option */
static int
parse(int c, char **argv, int invert, unsigned int *flags,
      const struct ipt_entry *entry,
      unsigned int *nfcache,
      struct ipt_entry_match **match)
{
	struct ipt_ethtype_info *info = (struct ipt_ethtype_info *)(*match)->data;
	char *etype, digit[4];

	switch (c) {
	case '1':
        etype = optarg;
		if (strlen(etype) != 4)
		{
			printf("ethernet type format is wrong %s\n", optarg);
			exit_error(PARAMETER_PROBLEM, "ethernet type format is wrong %s", optarg);
		}

		digit[0] = c2i(etype[0]);
		digit[1] = c2i(etype[1]);
		digit[2] = c2i(etype[2]);
		digit[3] = c2i(etype[3]);

		if (digit[0] == -1 || digit[1] == -1 || digit[2] == -1 || digit[3] == -1)
		{
			printf("ethernet type format is wrong %s\n", optarg);
			exit_error(PARAMETER_PROBLEM, "ethernet type format is wrong %s", optarg);
		}

		info->ethtype = digit[0]*16*16*16 + digit[1]*16*16 + digit[2]*16 + digit[3];

		//printf("%d %d %d %d, value = %d\n", digit[0], digit[1], digit[2], digit[3], info->ethtype);

		break;
    
	default:
		return 0;
	}
	return 1;
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
	struct ipt_ethtype_info *info = (struct ipt_ethtype_info *)target->data;

	printf("ethtype = %d\n",info->ethtype);
}


/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
	struct ipt_ethtype_info *info = (struct ipt_ethtype_info *)target->data;
	printf("Save Ethtype\n");
	printf("ethtype = %d\n",info->ethtype);
}


static struct iptables_match ethtype = { 
	.next		= NULL,
	.name		= "ethtype",
	.version	= IPTABLES_VERSION,
	.size		= IPT_ALIGN(sizeof(struct ipt_ethtype_info)),
	.userspacesize	= IPT_ALIGN(sizeof(struct ipt_ethtype_info)),
	.help		= &help,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts
};

void _init(void)
{
	register_match(&ethtype);
}
