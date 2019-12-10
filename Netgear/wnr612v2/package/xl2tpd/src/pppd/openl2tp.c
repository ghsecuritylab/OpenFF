/*****************************************************************************
 * Copyright (C) 2006,2007 Katalix Systems Ltd
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 *
 *****************************************************************************/

/* pppd plugin for interfacing to openl2tpd */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "pppd.h"
#include "pathnames.h"
#include "fsm.h"
#include "lcp.h"
#include "ccp.h"
#include "ipcp.h"
#include <sys/stat.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <linux/version.h>
#include <linux/sockios.h>
#ifndef aligned_u64
/* should be defined in sys/types.h */
#define aligned_u64 unsigned long long __attribute__((aligned(8)))
#endif
#include <linux/types.h>
#include <linux/if_ether.h>
#include <linux/ppp_defs.h>
#include <linux/if_ppp.h>
#include <linux/if_pppox.h>
#include <linux/if_pppol2tp.h>

#include "l2tp_event_rpc.h"

extern void (*pppol2tp_send_accm_hook)(int tunnel_id, int session_id, uint32_t send_accm, uint32_t recv_accm);
extern void (*pppol2tp_ip_updown_hook)(int tunnel_id, int session_id, int up);

const char pppd_version[] = VERSION;

static CLIENT *openl2tp_client = NULL;

static void (*old_pppol2tp_send_accm_hook)(int tunnel_id, int session_id, uint32_t send_accm, uint32_t recv_accm) = NULL;
static void (*old_pppol2tp_ip_updown_hook)(int tunnel_id, int session_id, int up) = NULL;

/*****************************************************************************
 * OpenL2TP RPC interface.
 * We send a PPP_ACCM_IND to openl2tpd to report ACCM values. 
 * Async RPC is used so that we don't stall waiting for openl2tpd to send a 
 * reply. Since we're running on the same host as openl2tpd, UDP is reliable,
 * hence there's no need to worry about whether our message arrives.
 *****************************************************************************/

static int openl2tp_rpc_client_create(void)
{
	if (openl2tp_client == NULL) {
		openl2tp_client = clnt_create("localhost", L2TP_EVENT_PROG, L2TP_EVENT_VERSION, "udp");
		if (openl2tp_client == NULL) {
			char *err_msg = clnt_spcreateerror("openl2tp connection");
			err_msg[strlen(err_msg) - 1] = '\0';
			error(err_msg);
			return -ENOTCONN;
		}
	}

	return 0;
}

static void openl2tp_rpc_client_delete(void)
{
	if (openl2tp_client != NULL) {
		clnt_destroy(openl2tp_client);
		openl2tp_client = NULL;
	}
}


static void openl2tp_send_accm_ind(int tunnel_id, int session_id, uint32_t send_accm, uint32_t recv_accm)
{
	int result;
	struct timeval timeout = { 0, 0 };
	struct l2tp_session_ppp_accm_ind_1_argument args;

	if (openl2tp_client == NULL) {
		result = openl2tp_rpc_client_create();
		if (result < 0) {
			goto out;
		}
	}

	memset(&args, 0, sizeof(args));
	args.tunnel_id = tunnel_id;
	args.session_id = session_id;
	args.send_accm = ntohl(send_accm);
	args.recv_accm = ntohl(recv_accm);
	result = clnt_call(openl2tp_client, L2TP_SESSION_PPP_ACCM_IND, 
			   (xdrproc_t) xdr_l2tp_session_ppp_accm_ind_1_argument,
			   (char *) &args, NULL, NULL, timeout);
	if ((result != RPC_SUCCESS) && (result != RPC_TIMEDOUT)) {
		char *err_msg = clnt_sperror(openl2tp_client, "ppp_accm_ind");
		err_msg[strlen(err_msg) - 1] = '\0';
		fatal(err_msg);
	}
out:
	if (old_pppol2tp_send_accm_hook != NULL) {
		(*old_pppol2tp_send_accm_hook)(tunnel_id, session_id, send_accm, recv_accm);
	}
	return;
}

static void openl2tp_ppp_updown_ind(int tunnel_id, int session_id, int up)
{
	int result;
	struct timeval timeout = { 0, 0 };
	struct l2tp_session_ppp_updown_ind_1_argument args;

	if (openl2tp_client == NULL) {
		result = openl2tp_rpc_client_create();
		if (result < 0) {
			goto out;
		}
	}

	memset(&args, 0, sizeof(args));
	args.tunnel_id = tunnel_id;
	args.session_id = session_id;
	args.msg.up = up;
	args.msg.unit = ifunit;
	args.msg.ifname = strdup(ifname);
	if (peer_authname[0] != '\0') {
		args.msg.user_name = strdup(peer_authname);
	} else {
		args.msg.user_name = strdup("");
	}
	result = RPC_FAILED;
	if ((args.msg.ifname != NULL) && (args.msg.user_name != NULL)) {
		result = clnt_call(openl2tp_client, L2TP_SESSION_PPP_UPDOWN_IND, 
				   (xdrproc_t) xdr_l2tp_session_ppp_updown_ind_1_argument,
				   (char *) &args, NULL, NULL, timeout);
	}
	if ((result != RPC_SUCCESS) && (result != RPC_TIMEDOUT)) {
		char *err_msg = clnt_sperror(openl2tp_client, "ppp_updown_ind");
		err_msg[strlen(err_msg) - 1] = '\0';
		fatal(err_msg);
		/* NOTREACHED */
	}
out:
	if (old_pppol2tp_ip_updown_hook != NULL) {
		(*old_pppol2tp_ip_updown_hook)(tunnel_id, session_id, up);
	}

	return;
}

/*****************************************************************************
 * Application init
 *****************************************************************************/

void plugin_init(void)
{
	old_pppol2tp_send_accm_hook = pppol2tp_send_accm_hook;
	pppol2tp_send_accm_hook = openl2tp_send_accm_ind;

	old_pppol2tp_ip_updown_hook = pppol2tp_ip_updown_hook;
	pppol2tp_ip_updown_hook = openl2tp_ppp_updown_ind;
}

