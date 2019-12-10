#ifndef _SCRIPT_H
#define _SCRIPT_H

#define CONFLICT

void run_script(struct dhcpMessage *packet, const char *name);
#ifdef  CONFLICT
 int check_ip_conflict(struct dhcpMessage *packet);
#endif

#endif
