/**
 * This file implements a target in userspacce with an vlan-mark
 * extension target.
 * 
 * 
 * Ares.Yeh 2008.3.19
 */





#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "../include/ebtables_u.h"
#include <linux/netfilter_bridge/ebt_vlan_mark_t.h>
#include <linux/if_ether.h>


#define VLAN_MARK_VID '1'
#define OPT_VLAN_MARK_VID 0x01

#define VLAN_MARK_PRIORITY '2'
#define OPT_VLAN_MARK_PRIORITY 0x02

static struct option opts[] =
{
	{ EBT_VLAN_MARK_TARGET_PARAM_VID  , required_argument, 0, VLAN_MARK_VID},
	{ EBT_VLAN_MARK_TARGET_PARAM_PRIORITY  , required_argument, 0, VLAN_MARK_PRIORITY},
	{ 0}
};

static void print_help()
{
	printf( EBT_VLAN_MARK_TARGET "target options:\n"
			" --" EBT_VLAN_MARK_TARGET_PARAM_VID " value      : set outgoing vid \n"
			" --" EBT_VLAN_MARK_TARGET_PARAM_PRIORITY " value      : set outgoing user priority \n");
}

static void init(struct ebt_entry_target *target)
{
	struct ebt_vlan_mark_t_info *markinfo =
	(struct ebt_vlan_mark_t_info *)target->data;


	markinfo->vid = 0;
	markinfo->priority = 0;

}


static int parse(int c, char **argv, int argc,
				 const struct ebt_u_entry *entry, unsigned int *flags,
				 struct ebt_entry_target **target)
{
	struct ebt_vlan_mark_t_info *markinfo =
	(struct ebt_vlan_mark_t_info *)(*target)->data;
	char *end;




	switch (c)
	{
	
	case VLAN_MARK_VID:
		ebt_check_option2(flags, OPT_VLAN_MARK_VID);
		markinfo->vid = strtoul(optarg, &end, 0);
		if (*end != '\0' || end == optarg)
			ebt_print_error2("Bad --vid value '%s'", optarg);

		markinfo->vid = markinfo->vid & 0x0FFF;/* only 12-bit usable.*/



		break;
	case VLAN_MARK_PRIORITY:
		ebt_check_option2(flags, OPT_VLAN_MARK_PRIORITY);
		markinfo->priority = strtoul(optarg, &end, 0);
		if (*end != '\0' || end == optarg)
			ebt_print_error2("Bad --priority value '%s'", optarg);

		markinfo->priority = markinfo->priority & 0x07;/* only 3-bit usable.*/



		break;
	default:
		return 0;
	}
	return 1;
}

static void final_check(const struct ebt_u_entry *entry,
						const struct ebt_entry_target *target, const char *name,
						unsigned int hookmask, unsigned int time)
{
	struct ebt_vlan_mark_t_info *markinfo =
	(struct ebt_vlan_mark_t_info *)target->data;




}

static void print(const struct ebt_u_entry *entry,
				  const struct ebt_entry_target *target)
{

	struct ebt_vlan_mark_t_info *markinfo =
	(struct ebt_vlan_mark_t_info *)target->data;

	printf(" --" EBT_VLAN_MARK_TARGET_PARAM_VID " %d", markinfo->vid );
	printf(" --" EBT_VLAN_MARK_TARGET_PARAM_PRIORITY " %d", markinfo->priority );

}

/* Compare whether two entry have the same target.*/
static int compare(const struct ebt_entry_target *t1,
				   const struct ebt_entry_target *t2)
{
	struct ebt_vlan_mark_t_info *markinfo1 =
	(struct ebt_vlan_mark_t_info *)t1->data;
	struct ebt_vlan_mark_t_info *markinfo2 =
	(struct ebt_vlan_mark_t_info *)t2->data;



	return markinfo1->vid == markinfo2->vid && markinfo1->priority == markinfo2->priority ;

}




static struct ebt_u_target mark_target =
{
	.name       = EBT_VLAN_MARK_TARGET,
	.size       = sizeof(struct ebt_vlan_mark_t_info),
				  .help       = print_help,
	.init       = init,
	.parse      = parse,
	.final_check    = final_check,
	.print      = print,
	.compare    = compare,
	.extra_ops  = opts,
};


void _init(void)
{
	ebt_register_target(&mark_target);

}

