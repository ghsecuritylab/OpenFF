/* vi: set sw=4 ts=4: */
/*
 * Mini reboot implementation for busybox
 *
 * Copyright (C) 1999-2004 by Erik Andersen <andersen@codepoet.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/reboot.h>
#include "busybox.h"
#include "init_shared.h"


extern int reboot_main(int argc, char **argv)
{
	char *delay; /* delay in seconds before rebooting */
	int flags;

	/* Parse and handle arguments */
	flags = bb_getopt_ulflags(argc, argv, "d:nf", &delay);
	if(flags&1) {
		sleep(atoi(delay));
	}
	if (!(flags&2)) sync();
	
#ifndef RB_AUTOBOOT
#define RB_AUTOBOOT		0x01234567
#endif

	/* Perform action. */
	if (!(flags & 4)) {
#ifndef CONFIG_INIT
	return(bb_shutdown_system(RB_AUTOBOOT));
#else
	return kill_init(SIGTERM);
#endif
	} else reboot(RB_AUTOBOOT);
}

/*
Local Variables:
c-file-style: "linux"
c-basic-offset: 4
tab-width: 4
End:
*/
