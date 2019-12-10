/* CONENAT target. 
 *
 * Copyright  2008-11-11, DNI
 * All Rights Reserved.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/netfilter_ipv4/ipt_DNS_HIJACK.h>

static void
help(void)
{
	printf(
"DNS HIJACK v%s options:\n"
" --url string1[,string2][,string3]\n",
IPTABLES_VERSION);
}


static struct option opts[] = {
	{ "url", 1, 0, '1' },
	{ "state", 1, 0, '2' },
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
	struct ipt_dnshj_info *info = (struct ipt_dnshj_info *)(*target)->data;

        switch(c) {
        case '1':
    		if(URL_MAX_LEN >= strlen(optarg))
    			strcpy(info->url,optarg);
    		else
    			exit_error(PARAMETER_PROBLEM,"Length of url %s is more than %d characters", optarg, URL_MAX_LEN);
    		return 1;
		break;
            case '2':
    		if(strcmp(optarg,"blank") == 0)
    			info->state=BLANK_STATE;	
    		else if (strcmp(optarg,"normal") == 0)
    			info->state=NORMAL_STATE;
    		else 
    			exit_error(PARAMETER_PROBLEM,"unknow state: %s\n", optarg);
    		return 1;
            break;
        default:
                break;
        }
        return 0;
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
	struct ipt_dnshj_info *info = (struct ipt_dnshj_info *)target->data;

	printf("URL: %s state: %s\n",info->url, (info->state == BLANK_STATE)? "blank" : "normal");
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const struct ipt_ip *ip, const struct ipt_entry_target *target)
{
	struct ipt_dnshj_info *info = (struct ipt_dnshj_info *)target->data;
	printf("Save DNS_HJ\n");
	printf("URL: %s state: %s\n",info->url, (info->state == BLANK_STATE)? "blank" : "normal");

}

static struct iptables_target dnshj = {
	.next           = NULL,
    	.name           = "DNS_HIJACK",
    	.version        = IPTABLES_VERSION,
    	.size           = IPT_ALIGN(sizeof(struct ipt_dnshj_info)),
    	.userspacesize  = IPT_ALIGN(sizeof(struct ipt_dnshj_info)),
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
	register_target(&dnshj);
}
