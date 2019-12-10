/*
 *     Copyright (C) 2008 Delta Networks Inc.
 */

extern void NmrpConfig(void);

extern void NmrpHandler(uchar * pkt, unsigned dest, unsigned src, unsigned len);
extern int NmrpState;
extern ulong Nmrp_active_start;
/* NMRP codes */
enum _nmrp_codes_ {
	NMRP_CODE_ADVERTISE = 0x01,
	NMRP_CODE_CONF_REQ = 0x02,
	NMRP_CODE_CONF_ACK = 0x03,
	NMRP_CODE_CLOSE_REQ = 0x04,
	NMRP_CODE_CLOSE_ACK = 0x05,
	NMRP_CODE_KEEP_ALIVE_REQ = 0x06,
	NMRP_CODE_KEEP_ALIVE_ACK = 0x07,
	NMRP_CODE_TFTP_UL_REQ = 0x10
};

/* NMRP option types */
enum _nmrp_option_types_ {
	NMRP_OPT_MAGIC_NO = 0x0001,
	NMRP_OPT_DEV_IP = 0x0002,
	NMRP_OPT_FW_UP = 0x0101
};

/*  NMRP REQ max retries */
enum _nmrp_req_max_retries_ {
	NMRP_MAX_RETRY_CONF = 5,
	NMRP_MAX_RETRY_CLOSE = 2,
	NMRP_MAX_RETRY_TFTP_UL = 4
};

/* NMRP timeouts */
enum _nmrp_timeouts_ {
	NMRP_TIMEOUT_REQ = 1,	/* 0.5 sec */
	NMRP_TIMEOUT_LISTEN = 3,	/* 3 sec */
	NMRP_TIMEOUT_ACTIVE = 60,	/* 1 minute */
	NMRP_TIMEOUT_CLOSE = 6,	/* 6 sec */
	NMRP_TIMEOUT_ADVERTISE = 1	/* 0.5 sec */
};

#define STATE_LISTENING 1
#define STATE_CONFIGING 2
#define STATE_TFTPWAITING 3
#define STATE_TFTPUPLOADING 4
#define STATE_CLOSING 5
#define STATE_CLOSED 6
#define STATE_KEEP_ALIVE 7
#define MAGICNO "NTGR"
#define IP_LEN 4
#define MAGIC_NO_LEN 4

#define NMRP_MAX_OPT_PER_MSG 6
typedef struct {
	ushort type;
	ushort len;
	union {
		uchar magicno[MAGIC_NO_LEN];
		struct{
			uchar addr[IP_LEN];
			uchar mask[IP_LEN];
		}ip;
	} value;
}__attribute__ ((packed)) NMRP_PARSED_OPT;

typedef struct {
	ushort reserved;
	uchar code;
	uchar id;
	ushort length;
	int numOptions;
	NMRP_PARSED_OPT options[NMRP_MAX_OPT_PER_MSG];
}__attribute__ ((packed)) NMRP_PARSED_MSG;
