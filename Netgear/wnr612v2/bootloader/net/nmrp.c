/*
 *     Copyright (C) 2008 Delta Networks Inc.
 */
#include <common.h>
#include <command.h>
#include <net.h>
#include "tftp.h"
#include "bootp.h"
#include "nmrp.h"

typedef void nmrp_thand_f(void);
ulong NmrpOuripaddr;
ulong NmrpOuripSubnetMask;
ulong NmrpFwOption;
int NmrpState = 0;
ulong Nmrp_active_start=0;
static ulong NmrpBlock;
static ulong NmrpLastBlock = 0;
int Nmrp_Listen_TimeoutCount = 0;
int Nmrp_REQ_TimeoutCount = 0;
static ulong NmrptimeStart;
static ulong NmrptimeDelta;
static nmrp_thand_f *NmrptimeHandler;
static uchar Nmrpproto;
static int Nmrp_Closing_TimeoutCount = 0;
IPaddr_t NmrpClientIP = 0;
uchar NmrpClientEther[6] = { 0, 0, 0, 0, 0, 0 };
uchar NmrpServerEther[6] = { 0, 0, 0, 0, 0, 0 };

static void Nmrp_Listen_Timeout(void);
void NmrpSend(void);

void NmrpStart(void);

void NmrpSetTimeout(unchar, ulong, nmrp_thand_f *);

void Nmrp_Led_Flashing_Timeout();

static int MyNetSetEther(volatile uchar * xet, uchar * addr, uint prot)
{
	Ethernet_t *et = (Ethernet_t *) xet;
	ushort myvlanid;
	myvlanid = ntohs(NetOurVLAN);
	if (myvlanid == (ushort) - 1)
		myvlanid = VLAN_NONE;
	memcpy(et->et_dest, addr, 6);
	memcpy(et->et_src, NetOurEther, 6);
	if ((myvlanid & VLAN_IDMASK) == VLAN_NONE) {
		et->et_protlen = htons(prot);
		return ETHER_HDR_SIZE;
	} else {
		VLAN_Ethernet_t *vet = (VLAN_Ethernet_t *) xet;
		vet->vet_vlan_type = htons(PROT_VLAN);
		vet->vet_tag = htons((0 << 5) | (myvlanid & VLAN_IDMASK));
		vet->vet_type = htons(prot);
		return VLAN_ETHER_HDR_SIZE;
	}

	return ETHER_HDR_SIZE;
}

static void Nmrp_Conf_Timeout(void)
{
	if (++Nmrp_REQ_TimeoutCount > NMRP_MAX_RETRY_CONF) {
		puts("\n retry conf count exceeded;\n");
		Nmrp_REQ_TimeoutCount = 0;
		NmrpStart();
	} else {
		puts("T");
		NmrpSetTimeout(NMRP_CODE_CONF_ACK,
			       (NMRP_TIMEOUT_REQ * CFG_HZ) / 2,
			       Nmrp_Conf_Timeout);
		NmrpSend();
	}
}

void Nmrp_Closing_Timeout()
{

	if (++Nmrp_Closing_TimeoutCount > NMRP_MAX_RETRY_CLOSE) {
		puts("\n close retry count exceed;stay idle and blink\n");
		Nmrp_Closing_TimeoutCount = 0;
		reset_default();

		puts("\npress ctrl+C to continue.....\n");

		Nmrp_Led_Flashing_Timeout();

		ctrlc();
		console_assign(stdout, "nulldev");
		console_assign(stderr, "nulldev");
		NmrpState = STATE_CLOSED;
		NetState = NETLOOP_SUCCESS;
	} else {
		puts("T");
		NmrpSetTimeout(NMRP_CODE_CLOSE_ACK,
			       (CFG_HZ * NMRP_TIMEOUT_REQ) / 2,
			       Nmrp_Closing_Timeout);
		NmrpSend();
	}
}

void Nmrp_Led_Flashing_Timeout()
{
	static int NmrpLedCount = 0;
	while (1) {

		if (ctrlc())
			break;

		NmrpLedCount++;
		if ((NmrpLedCount % 2) == 1) {
			test_led(0);
			udelay(500000);
		} else {
			test_led(1);
			udelay(500000);
		}
	}
	/*press ctl+c, turn on test led,then normally boot*/
	test_led(0);
}

extern void NmrpSend(void)
{
	volatile uchar *pkt;
	volatile uchar *xp;
	int len = 0;
	int eth_len = 0;
	pkt = (uchar *) NetTxPacket;
	pkt += MyNetSetEther(pkt, NmrpServerEther, PROT_NMRP);
	eth_len = pkt - NetTxPacket;

	switch (NmrpState) {
	case STATE_LISTENING:
		xp = pkt;
		*((ushort *) pkt)++ = 0;
		*((uchar *) pkt)++ = (NMRP_CODE_CONF_REQ);
		*((uchar *) pkt)++ = 0;
		*((ushort *) pkt)++ = htons(6);

		len = pkt - xp;
		(void)NetSendPacket((uchar *) NetTxPacket, eth_len + len);
		NetSetTimeout((NMRP_TIMEOUT_REQ * CFG_HZ) / 2,
			      Nmrp_Conf_Timeout);
		NmrpSetTimeout(NMRP_CODE_CONF_ACK,
			       (NMRP_TIMEOUT_REQ * CFG_HZ) / 2,
			       Nmrp_Conf_Timeout);
		break;
	case STATE_CONFIGING:
		xp = pkt;
		*((ushort *) pkt)++ = 0;
		*((uchar *) pkt)++ = (NMRP_CODE_TFTP_UL_REQ);
		*((uchar *) pkt)++ = 0;
		*((ushort *) pkt)++ = htons(6);
		len = pkt - xp;
		(void)NetSendPacket((uchar *) NetTxPacket, eth_len + len);
#ifdef CONFIG_WNR612
		UpgradeFirmwareFromNmrpServer();
#else
		StartTftpServerToRecoveFirmware();
#endif
		break;
	case STATE_KEEP_ALIVE:
		printf("NMRP Send Keep alive REQ\n");
		xp = pkt;
		*((ushort *) pkt)++ = 0;
		*((uchar *) pkt)++ = (NMRP_CODE_KEEP_ALIVE_REQ);
		*((uchar *) pkt)++ = 0;
		*((ushort *) pkt)++ = htons(6);
		len = pkt - xp;
		(void)NetSendPacket((uchar *) NetTxPacket, eth_len + len);
		break;
	case STATE_CLOSING:
		printf("NMRP Send Closing REQ\n");
		xp = pkt;
		*((ushort *) pkt)++ = 0;
		*((uchar *) pkt)++ = (NMRP_CODE_CLOSE_REQ);
		*((uchar *) pkt)++ = 0;
		*((ushort *) pkt)++ = htons(6);
		len = pkt - xp;
		NmrpSetTimeout(NMRP_CODE_CLOSE_ACK, (1 * CFG_HZ),
			       Nmrp_Closing_Timeout);
		(void)NetSendPacket((uchar *) NetTxPacket, eth_len + len);
		break;

	case STATE_CLOSED:
		reset_default();
		NmrptimeHandler=NULL;
		puts("\npress ctrl+C to continue.....\n");

		Nmrp_Led_Flashing_Timeout();

		ctrlc();
		console_assign(stdout, "nulldev");
		console_assign(stderr, "nulldev");
		NetState = NETLOOP_SUCCESS;
		break;
	default:
		break;

	}

}

static void Nmrp_Listen_Timeout(void)
{
extern int iTestLEDOff;	// 1 means OFF
	
	if (++Nmrp_Listen_TimeoutCount > NMRP_TIMEOUT_LISTEN) {
		puts("\nRetry count exceeded; boot the image as usual\n");
		Nmrp_Listen_TimeoutCount = 0;
		ResetBootup_usual();
	} else {
		test_led(iTestLEDOff++%2);
		puts("T");
		NetSetTimeout(CFG_HZ, Nmrp_Listen_Timeout);
	}
}

static NMRP_PARSED_OPT *Nmrp_Parse(uchar * pkt, ushort optType)
{
	NMRP_PARSED_MSG *msg = (NMRP_PARSED_MSG *) pkt;
	NMRP_PARSED_OPT *opt, *optEnd;
	optEnd = &msg->options[msg->numOptions];
	for (opt = msg->options; opt != optEnd; opt++)
		if (opt->type == optType)
			break;
	return msg->numOptions == 0 ? NULL : (opt == optEnd ? NULL : opt);
}

void NmrpSetTimeout(unchar proto, ulong iv, nmrp_thand_f * f)
{
	if (iv == 0) {
		NmrptimeHandler = (nmrp_thand_f *) 0;
		Nmrpproto = 0;
	} else {

		NmrptimeHandler = f;
		NmrptimeStart = get_timer(0);
		NmrptimeDelta = iv;
		Nmrpproto = proto;
	}
}
/*
  Function to parse the NMRP options inside packet.
  If all options are parsed correctly, it returns 0.
 */
static int Nmrp_Parse_Opts(uchar *pkt, NMRP_PARSED_MSG *nmrp_parsed)
{
	nmrp_t *nmrphdr= (nmrp_t*) pkt;
	NMRP_OPT *nmrp_opt;
	int remain_len, opt_index = 0;

	nmrp_parsed->reserved = nmrphdr->reserved;
	nmrp_parsed->code     = nmrphdr->code;
	nmrp_parsed->id       = nmrphdr->id;
	nmrp_parsed->length   = nmrphdr->length;

	remain_len = nmrphdr->length - NMRP_HDR_LEN;

	nmrp_opt = &nmrphdr->opt;
	while (remain_len > 0){
		memcpy(&nmrp_parsed->options[opt_index], nmrp_opt, nmrp_opt->len);
		remain_len -= nmrp_opt->len;
		nmrp_opt = ((uchar *)nmrp_opt) + nmrp_opt->len;
		opt_index++;
	}
	nmrp_parsed->numOptions=opt_index;
	return remain_len;
}

void NmrpHandler(uchar * pkt, unsigned dest, unsigned src, unsigned len)
{
	nmrp_t *nmrphdr= (nmrp_t*) pkt;
	uchar proto;
	unchar *xp = pkt;
	int fwUpgrade;
	NMRP_PARSED_MSG nmrp_parsed;
	NMRP_PARSED_OPT *opt;
	proto = nmrphdr->code;

	/* check for timeout,and run the timeout handler
	   if we have one
	 */

	if (NmrptimeHandler && ((get_timer(0) - NmrptimeStart) > NmrptimeDelta)
	    && (proto != Nmrpproto)) {
		nmrp_thand_f *x;
		x = NmrptimeHandler;
		NmrptimeHandler = (nmrp_thand_f *) 0;
		(*x) ();
	}

	/*
	   Check if Reserved field is zero. Per the specification, the reserved
	   must be all zero in a valid NMRP packet.
	 */
	if (nmrphdr->reserved != 0){
		return;
	}
	memset(&nmrp_parsed, 0, sizeof(NMRP_PARSED_MSG));

	/*
	   Parse the options inside the packet and save it into nmrp_parsed for
	   future reference.
	 */
	if (Nmrp_Parse_Opts(pkt, &nmrp_parsed) != 0){
		/* Some wrong inside the packet, just discard it */
		return;
	}

	NmrpBlock = proto;

	// ignore same packet
	if (NmrpBlock == NmrpLastBlock)
		return;
	NmrpLastBlock = NmrpBlock;

	switch (proto) {
	case NMRP_CODE_ADVERTISE:	/*listening state * */
		if (NmrpState == 0) {
			/*
			   Check if we get the MAGIC-NO option and the content is match
			   with the MAGICNO.
			 */
			if ((opt = Nmrp_Parse(&nmrp_parsed, NMRP_OPT_MAGIC_NO)) != NULL){
				int opt_hdr_len = sizeof(opt->type) + sizeof(opt->len);
				if (memcmp(opt->value.magicno, MAGICNO, opt->len - opt_hdr_len) == 0){
					NmrpState = STATE_LISTENING;
					test_led(0);
					printf("\nNMRP CONFIGING");
					NmrpSend();
				}
			}
		}
		break;
	case NMRP_CODE_CONF_ACK:
		if (NmrpState == STATE_LISTENING) {
			/*
			   If there is no DEV-IP option inside the packet, it must be
			   something wrong in the packet, so just ignore this packet
			   without any action taken.
			 */
			if ((opt = Nmrp_Parse(&nmrp_parsed, NMRP_OPT_DEV_IP)) != NULL){
				memcpy(NetOurTftpIP, opt->value.ip.addr,IP_ADDR_LEN);
				/* Do we need the subnet mask? */
				memcpy(&NmrpOuripSubnetMask, opt->value.ip.mask,IP_ADDR_LEN);
				/*
				   FW-UP option is optional for CONF-ACK and it has no effect no
				   matter what is the content of this option, so we just skip the
				   process of this option for now, and will add it back when
				   this option is defined as mandatory.
				   The process for FW-UP would be similar as the action taken for
				   DEV-IP and MAGIC-NO.
				 */

				NmrpState = STATE_CONFIGING;
				printf("\nNMRP WAITING FOR UPLOAD FIRMWARE!\n");
				NmrpSend();
			}else
				break;
		}
		break;
	case NMRP_CODE_CLOSE_ACK:
		if (NmrpState == STATE_CLOSING) {
			NmrpState = STATE_CLOSED;
			printf("\nNMRP CLOSED");
			NmrpSend();
		}
		break;
	default:
		break;
	}

}

void NmrpStart(void)
{
	printf("\n Client starts...[Listening] for ADVERTISE...");

	NetSetTimeout(CFG_HZ / 10, Nmrp_Listen_Timeout);
	NetSetHandler(NmrpHandler);

	NmrpState = 0;
	Nmrp_Listen_TimeoutCount = 0;
	memset(NmrpClientEther, 0, 6);
	NmrpClientIP = 0;
}
