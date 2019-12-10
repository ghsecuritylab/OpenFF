#ifndef _IPT_LOGS_H
#define _IPT_LOGS_H

#define LOG_PORT	0x0001
#define LOG_SPORT	0x0002	/* we only care the source port */
#define LOG_NOPORT      0x0004
#define PREFIX_LENGTH   64

struct ipt_logs_info {
	unsigned short flags;

	char prefix[PREFIX_LENGTH];
	char destip[32]; /* Local IP-Address --> DestIP:Port */
};

#endif /*_IPT_LOGS_H*/
