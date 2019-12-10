/***********************************************************************
*
* discovery.c
*
* Perform PPPoE discovery
*
* Copyright (C) 1999 by Roaring Penguin Software Inc.
*
* LIC: GPL
*
***********************************************************************/

static char const RCSID[] =
"$Id: discovery.c,v 1.3 2006/03/20 11:06:07 alva_zhang Exp $";

#include "pppoe.h"

#ifdef HAVE_SYSLOG_H
#include <syslog.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#ifdef HAVE_SYS_UIO_H
#include <sys/uio.h>
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef USE_LINUX_PACKET
#include <sys/ioctl.h>
#include <fcntl.h>
#endif

#include <signal.h>
//#include <board.h>
#include <sys/shm.h>

/* Supplied by pppd if we're a plugin */
extern int persist;

#if (NETGEAR_ROUTER_SPEC)
#define PADT_MAX_COUNT 2
int padt_count = PADT_MAX_COUNT;
#endif

/**********************************************************************
*%FUNCTION: parseForHostUniq
*%ARGUMENTS:
* type -- tag type
* len -- tag length
* data -- tag data.
* extra -- user-supplied pointer.  This is assumed to be a pointer to int.
*%RETURNS:
* Nothing
*%DESCRIPTION:
* If a HostUnique tag is found which matches our PID, sets *extra to 1.
***********************************************************************/
void
parseForHostUniq(UINT16_t type, UINT16_t len, unsigned char *data,
         void *extra)
{
    int *val = (int *) extra;
    if (type == TAG_HOST_UNIQ && len == sizeof(pid_t)) {
    pid_t tmp;
    memcpy(&tmp, data, len);
    if (tmp == getpid()) {
        *val = 1;
    }
    }
}

/**********************************************************************
*%FUNCTION: packetIsForMe
*%ARGUMENTS:
* conn -- PPPoE connection info
* packet -- a received PPPoE packet
*%RETURNS:
* 1 if packet is for this PPPoE daemon; 0 otherwise.
*%DESCRIPTION:
* If we are using the Host-Unique tag, verifies that packet contains
* our unique identifier.
***********************************************************************/
int
packetIsForMe(PPPoEConnection *conn, PPPoEPacket *packet)
{
    int forMe = 0;

    /* If packet is not directed to our MAC address, forget it */
    if (memcmp(packet->ethHdr.h_dest, conn->myEth, ETH_ALEN)) return 0;

    /* If we're not using the Host-Unique tag, then accept the packet */
    if (!conn->useHostUniq) return 1;

    parsePacket(packet, parseForHostUniq, &forMe);
    return forMe;
}

/**********************************************************************
*%FUNCTION: parsePADOTags
*%ARGUMENTS:
* type -- tag type
* len -- tag length
* data -- tag data
* extra -- extra user data.  Should point to a PacketCriteria structure
*          which gets filled in according to selected AC name and service
*          name.
*%RETURNS:
* Nothing
*%DESCRIPTION:
* Picks interesting tags out of a PADO packet
***********************************************************************/
void
parsePADOTags(UINT16_t type, UINT16_t len, unsigned char *data,
          void *extra)
{
    struct PacketCriteria *pc = (struct PacketCriteria *) extra;
    PPPoEConnection *conn = pc->conn;
    int i;

    switch(type) {
    case TAG_AC_NAME:
    pc->seenACName = 1;
    if (conn->printACNames) {
        printf("Access-Concentrator: %.*s\n", (int) len, data);
    }
    if (conn->acName && len == strlen(conn->acName) &&
        !strncmp((char *) data, conn->acName, len)) {
        pc->acNameOK = 1;
    }
    break;
    case TAG_SERVICE_NAME:
    pc->seenServiceName = 1;
    if (conn->printACNames && len > 0) {
        printf("       Service-Name: %.*s\n", (int) len, data);
    }
    if (conn->serviceName && len == strlen(conn->serviceName) &&
        !strncmp((char *) data, conn->serviceName, len)) {
        pc->serviceNameOK = 1;
    }
    break;
    case TAG_AC_COOKIE:
    if (conn->printACNames) {
        printf("Got a cookie:");
        /* Print first 20 bytes of cookie */
        for (i=0; i<len && i < 20; i++) {
        printf(" %02x", (unsigned) data[i]);
        }
        if (i < len) printf("...");
        printf("\n");
    }
    conn->cookie.type = htons(type);
    conn->cookie.length = htons(len);
    memcpy(conn->cookie.payload, data, len);
    break;
    case TAG_RELAY_SESSION_ID:
    if (conn->printACNames) {
        printf("Got a Relay-ID:");
        /* Print first 20 bytes of relay ID */
        for (i=0; i<len && i < 20; i++) {
        printf(" %02x", (unsigned) data[i]);
        }
        if (i < len) printf("...");
        printf("\n");
    }
    conn->relayId.type = htons(type);
    conn->relayId.length = htons(len);
    memcpy(conn->relayId.payload, data, len);
    break;
    case TAG_SERVICE_NAME_ERROR:
    if (conn->printACNames) {
        printf("Got a Service-Name-Error tag: %.*s\n", (int) len, data);
        syslog(LOG_ERR, "=====>PADO FAILED: Tag Service-Name-Error: %.*s", (int) len, data);

    } else {
      syslog(LOG_ERR, "=====>PADO FAILED: Tag Service-Name-Error: %.*s", (int) len, data);
      syslog(LOG_ERR, "PADO: Service-Name-Error: %.*s", (int) len, data);
        exit(1);
    }
    break;
    case TAG_AC_SYSTEM_ERROR:
    if (conn->printACNames) {
         syslog(LOG_ERR, "=====>PADO FAILED: Tag AC_System-Error: %.*s", (int) len, data);
      printf("Got a System-Error tag: %.*s\n", (int) len, data);
    } else {
      syslog(LOG_ERR, "=====>PADO FAILED: Tag AC_System-Error: %.*s", (int) len, data);
        syslog(LOG_ERR, "PADO: System-Error: %.*s", (int) len, data);
        exit(1);
    }
    break;
    case TAG_GENERIC_ERROR:
    if (conn->printACNames) {
        printf("Got a Generic-Error tag: %.*s\n", (int) len, data);
        syslog(LOG_ERR, "=====>PADO FAILED: Tag Generic-Error: %.*s", (int) len, data);
    } else {
        syslog(LOG_ERR, "=====>PADO FAILED: Tag Generic-Error: %.*s", (int) len, data);
        syslog(LOG_ERR, "PADO: Generic-Error: %.*s", (int) len, data);
        exit(1);
    }
    break;
    }
}

/**********************************************************************
*%FUNCTION: parsePADSTags
*%ARGUMENTS:
* type -- tag type
* len -- tag length
* data -- tag data
* extra -- extra user data (pointer to PPPoEConnection structure)
*%RETURNS:
* Nothing
*%DESCRIPTION:
* Picks interesting tags out of a PADS packet
***********************************************************************/
void
parsePADSTags(UINT16_t type, UINT16_t len, unsigned char *data,
          void *extra)
{
    PPPoEConnection *conn = (PPPoEConnection *) extra;
    switch(type) {
    case TAG_SERVICE_NAME:
    syslog(LOG_ERR, "=====>PADS FAILED: Tag Service-Name: '%.*s'", (int) len, data);
    break;
    case TAG_SERVICE_NAME_ERROR:
    syslog(LOG_ERR, "=====>PADS FAILED: Tag Service-Name-Error: %.*s", (int) len, data);
    fprintf(stderr, "PADS: Service-Name-Error: %.*s\n", (int) len, data);
    conn->PADSHadError = 1;
    break;
    case TAG_AC_SYSTEM_ERROR:
    syslog(LOG_ERR, "=====>PADS FAILED: Tag AC_System-Error: %.*s", (int) len, data);
    fprintf(stderr, "PADS: System-Error: %.*s\n", (int) len, data);
    conn->PADSHadError = 1;
    break;
   case TAG_GENERIC_ERROR:
    syslog(LOG_ERR, "=====>PADS FAILED: Tag Generic-Error: %.*s", (int) len, data);
    fprintf(stderr, "PADS: Generic-Error: %.*s\n", (int) len, data);
    conn->PADSHadError = 1;
    break;
    case TAG_RELAY_SESSION_ID:
    conn->relayId.type = htons(type);
    conn->relayId.length = htons(len);
    memcpy(conn->relayId.payload, data, len);
    break;
    }
}

/***********************************************************************
*%FUNCTION: sendPADI
*%ARGUMENTS:
* conn -- PPPoEConnection structure
*%RETURNS:
* Nothing
*%DESCRIPTION:
* Sends a PADI packet
***********************************************************************/
void
sendPADI(PPPoEConnection *conn)
{
    PPPoEPacket packet;
    unsigned char *cursor = packet.payload;
    PPPoETag *svc = (PPPoETag *) (&packet.payload);
    UINT16_t namelen = 0;
    UINT16_t plen;
    int omit_service_name = 0;

    if (conn->serviceName) {
    namelen = (UINT16_t) strlen(conn->serviceName);
    if (!strcmp(conn->serviceName, "NO-SERVICE-NAME-NON-RFC-COMPLIANT")) {
        omit_service_name = 1;
    }
    }

    /* Set destination to Ethernet broadcast address */
    memset(packet.ethHdr.h_dest, 0xFF, ETH_ALEN);
    memcpy(packet.ethHdr.h_source, conn->myEth, ETH_ALEN);

    packet.ethHdr.h_proto = htons(Eth_PPPOE_Discovery);
    packet.ver = 1;
    packet.type = 1;
    packet.code = CODE_PADI;
    packet.session = 0;

    if (!omit_service_name) {
    plen = TAG_HDR_SIZE + namelen;
    CHECK_ROOM(cursor, packet.payload, plen);

    svc->type = TAG_SERVICE_NAME;
    svc->length = htons(namelen);

    if (conn->serviceName) {
        memcpy(svc->payload, conn->serviceName, strlen(conn->serviceName));
    }
    cursor += namelen + TAG_HDR_SIZE;
    } else {
    plen = 0;
    }

    /* If we're using Host-Uniq, copy it over */
    if (conn->useHostUniq) {
    PPPoETag hostUniq;
    pid_t pid = getpid();
    hostUniq.type = htons(TAG_HOST_UNIQ);
    hostUniq.length = htons(sizeof(pid));
    memcpy(hostUniq.payload, &pid, sizeof(pid));
    CHECK_ROOM(cursor, packet.payload, sizeof(pid) + TAG_HDR_SIZE);
    memcpy(cursor, &hostUniq, sizeof(pid) + TAG_HDR_SIZE);
    cursor += sizeof(pid) + TAG_HDR_SIZE;
    plen += sizeof(pid) + TAG_HDR_SIZE;
    }

    packet.length = htons(plen);

    sendPacket(conn, conn->discoverySocket, &packet, (int) (plen + HDR_SIZE));
    if (conn->debugFile) {
    dumpPacket(conn->debugFile, &packet, "SENT");
    fprintf(conn->debugFile, "\n");
    fflush(conn->debugFile);
    }
}

#ifdef MORE_PPPOE_SERVER /*By Aaron add 11/06/2008*/
/*********************************************************************
*   Public Data Definitions
*********************************************************************/
struct server_list  entry_mac[MAX_ENTRY];
int entry_index=0;
/*********************************************************************
*
@(#)Function:   cmpMAC
*
* Description:
*
*   compare mac address str2  str1
*
* Input Parameters:
*
* Output Parameters:
*   none.
*
* Error Return:
*   return 0  --> equality .
*   return 1 --> no equality.
* Note:
*
*********************************************************************/
int
cmpMAC(unsigned char *str1, unsigned char *str2)
{
    if((str1[0] == str2[0])&&
       (str1[1] == str2[1])&&
       (str1[2] == str2[2])&&
       (str1[3] == str2[3])&&
       (str1[4] == str2[4])&&
       (str1[5] == str2[5]))
        return 0;
    return 1;
}
/*********************************************************************
*
@(#)Function:   cpMAC
*
* Description:
*
*   copy mac address str2 -> str1
*
* Input Parameters:
*
* Output Parameters:
*   none.
*
* Error Return:
*   none.
* Note:
*
*********************************************************************/
void
cpMAC(unsigned char *str1, unsigned char *str2)
{
    str1[0] = str2[0];str1[1] = str2[1];
    str1[2] = str2[2];str1[3] = str2[3];
    str1[4] = str2[4];str1[5] = str2[5];
}

/*********************************************************************
*
@(#)Function:   search_entry_table
*
* Description:
*
*   Pursues already mac which joins
*
* Input Parameters:
*
* Output Parameters:
*   none.
*
* Error Return:
*   return 1 --> send PADR.
*   return 0 -->  recive next  PADO.
* Note:
*
*********************************************************************/
/*modified by Kenny 08/26/2009*/
int run_table[3]={0,0,0};
char
search_entry_table(unsigned char *mac_addr)
{
    int i,j;
    char find=-1;

    for(i=0; i<entry_index; i++)
    {
        if(!cmpMAC(mac_addr, entry_mac[i].mac)){
            find=i;
	    if(entry_mac[i].count == 0){
		entry_mac[i].count=run_table[i];
		return 1;
	    }else{
		entry_mac[i].count--;
		return 0;
	    }
        }
    }
    if(find == -1){
        /*add new entry table*/
        if(entry_index < MAX_ENTRY){
            cpMAC(entry_mac[entry_index].mac, mac_addr);
            entry_mac[entry_index].count=1;
	    for(j=0;j<entry_index;j++){
		entry_mac[j].count++;
	    }
            entry_index++;
	    if(entry_index==1){
		run_table[0]=1;
		run_table[1]=0;
		run_table[2]=0;
	   }
	   else if(entry_index==2){
	    	run_table[0]=2;
		run_table[1]=1;
		run_table[2]=0;
	    }
	   else if(entry_index==3){
	   	run_table[0]=3;
		run_table[1]=2;
		run_table[2]=1;
	   }	
            return 1;
        }else{
            return 0;
        }
    }

    return 0;
}
#endif
/**********************************************************************
*%FUNCTION: waitForPADO
*%ARGUMENTS:
* conn -- PPPoEConnection structure
* timeout -- how long to wait (in seconds)
*%RETURNS:
* Nothing
*%DESCRIPTION:
* Waits for a PADO packet and copies useful information
***********************************************************************/
void
waitForPADO(PPPoEConnection *conn, int timeout)
{
    fd_set readable;
    int r;
    struct timeval tv;
    PPPoEPacket packet;
    int len;

    struct PacketCriteria pc;
    pc.conn          = conn;
    pc.acNameOK      = (conn->acName)      ? 0 : 1;
    pc.serviceNameOK = (conn->serviceName) ? 0 : 1;
    pc.seenACName    = 0;
    pc.seenServiceName = 0;

    do {
    if (BPF_BUFFER_IS_EMPTY) {
        tv.tv_sec = timeout;
        tv.tv_usec = 0;

        FD_ZERO(&readable);
        FD_SET(conn->discoverySocket, &readable);

        while(1) {
        r = select(conn->discoverySocket+1, &readable, NULL, NULL, &tv);
        if (r >= 0 || errno != EINTR) break;
        }
        if (r < 0) {
        fatalSys("select (waitForPADO)");
        }
        if (r == 0) return;        /* Timed out */
    }

    /* Get the packet */
    receivePacket(conn->discoverySocket, &packet, &len);
    syslog(LOG_ERR, "===>PPPoE Discovery: receivED Packet PADO and checkING it ...... \n");
    /* Check length */
    if (ntohs(packet.length) + HDR_SIZE > len) {
      syslog(LOG_ERR, "=====>PADO FAILED: bogus PPPoE length field (%u) ", (unsigned int) ntohs(packet.length));
      continue;
    }

#ifdef USE_BPF
    /* If it's not a Discovery packet, loop again */
    if (etherType(&packet) != Eth_PPPOE_Discovery)
    {
        syslog(LOG_ERR, "=====>PADO FAILED: it's NOT a Discovery packet!\n");
        continue;
    }
#endif

    if (conn->debugFile) {
        dumpPacket(conn->debugFile, &packet, "RCVD");
        fprintf(conn->debugFile, "\n");
        fflush(conn->debugFile);
    }
    /* If it's not for us, loop again */
    if (!packetIsForMe(conn, &packet))
    {
        syslog(LOG_ERR, "=====>PADO FAILED: it's NOT for us!\n");
        continue;
    }


    if (packet.code == CODE_PADO) {
        if (NOT_UNICAST(packet.ethHdr.h_source)) {
        printErr("Ignoring PADO packet from non-unicast MAC address");
        syslog(LOG_ERR, "=====>PADO FAILED: it comes from non-unicast MAC address!\n");
        continue;
        }
        parsePacket(&packet, parsePADOTags, &pc);
        if (!pc.seenACName) {
        printErr("Ignoring PADO packet with no AC-Name tag");
        syslog(LOG_ERR, "=====>PADO FAILED: packet with NO AC-Name tag!\n");
        continue;
        }
        if (!pc.seenServiceName) {
        printErr("Ignoring PADO packet with no Service-Name tag");
        syslog(LOG_ERR, "=====>PADO FAILED: packet with NO Service-Name tag!\n");
        continue;
        }
        conn->numPADOs++;
        if (pc.acNameOK && pc.serviceNameOK) {
#ifdef MORE_PPPOE_SERVER /*By Aaron add 11/06/2008*/
            if( search_entry_table(packet.ethHdr.h_source)){
                memcpy(conn->peerEth, packet.ethHdr.h_source, ETH_ALEN);
            }else{
                continue;
            }
#else
            memcpy(conn->peerEth, packet.ethHdr.h_source, ETH_ALEN);
#endif
        if (conn->printACNames) {
            printf("AC-Ethernet-Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
               (unsigned) conn->peerEth[0],
               (unsigned) conn->peerEth[1],
               (unsigned) conn->peerEth[2],
               (unsigned) conn->peerEth[3],
               (unsigned) conn->peerEth[4],
               (unsigned) conn->peerEth[5]);
            printf("--------------------------------------------------\n");
            continue;
        }
        conn->discoveryState = STATE_RECEIVED_PADO;
        syslog(LOG_ERR, "=====>PADO PASSD: packet PADO verify OK!\n");
        break;
        }
    }
    } while (conn->discoveryState != STATE_RECEIVED_PADO);
}

/***********************************************************************
*%FUNCTION: sendPADR
*%ARGUMENTS:
* conn -- PPPoE connection structur
*%RETURNS:
* Nothing
*%DESCRIPTION:
* Sends a PADR packet
***********************************************************************/
void
sendPADR(PPPoEConnection *conn)
{
    PPPoEPacket packet;
    PPPoETag *svc = (PPPoETag *) packet.payload;
    unsigned char *cursor = packet.payload;

    UINT16_t namelen = 0;
    UINT16_t plen;

    if (conn->serviceName) {
    namelen = (UINT16_t) strlen(conn->serviceName);
    }
    plen = TAG_HDR_SIZE + namelen;
    CHECK_ROOM(cursor, packet.payload, plen);

    memcpy(packet.ethHdr.h_dest, conn->peerEth, ETH_ALEN);
    memcpy(packet.ethHdr.h_source, conn->myEth, ETH_ALEN);

    packet.ethHdr.h_proto = htons(Eth_PPPOE_Discovery);
    packet.ver = 1;
    packet.type = 1;
    packet.code = CODE_PADR;
    packet.session = 0;

    svc->type = TAG_SERVICE_NAME;
    svc->length = htons(namelen);
    if (conn->serviceName) {
    memcpy(svc->payload, conn->serviceName, namelen);
    }
    cursor += namelen + TAG_HDR_SIZE;

    /* If we're using Host-Uniq, copy it over */
    if (conn->useHostUniq) {
    PPPoETag hostUniq;
    pid_t pid = getpid();
    hostUniq.type = htons(TAG_HOST_UNIQ);
    hostUniq.length = htons(sizeof(pid));
    memcpy(hostUniq.payload, &pid, sizeof(pid));
    CHECK_ROOM(cursor, packet.payload, sizeof(pid)+TAG_HDR_SIZE);
    memcpy(cursor, &hostUniq, sizeof(pid) + TAG_HDR_SIZE);
    cursor += sizeof(pid) + TAG_HDR_SIZE;
    plen += sizeof(pid) + TAG_HDR_SIZE;
    }

    /* Copy cookie and relay-ID if needed */
    if (conn->cookie.type) {
    CHECK_ROOM(cursor, packet.payload,
           ntohs(conn->cookie.length) + TAG_HDR_SIZE);
    memcpy(cursor, &conn->cookie, ntohs(conn->cookie.length) + TAG_HDR_SIZE);
    cursor += ntohs(conn->cookie.length) + TAG_HDR_SIZE;
    plen += ntohs(conn->cookie.length) + TAG_HDR_SIZE;
    }

    if (conn->relayId.type) {
    CHECK_ROOM(cursor, packet.payload,
           ntohs(conn->relayId.length) + TAG_HDR_SIZE);
    memcpy(cursor, &conn->relayId, ntohs(conn->relayId.length) + TAG_HDR_SIZE);
    cursor += ntohs(conn->relayId.length) + TAG_HDR_SIZE;
    plen += ntohs(conn->relayId.length) + TAG_HDR_SIZE;
    }

    packet.length = htons(plen);
    sendPacket(conn, conn->discoverySocket, &packet, (int) (plen + HDR_SIZE));
    if (conn->debugFile) {
    dumpPacket(conn->debugFile, &packet, "SENT");
    fprintf(conn->debugFile, "\n");
    fflush(conn->debugFile);
    }
}

/**********************************************************************
*%FUNCTION: waitForPADS
*%ARGUMENTS:
* conn -- PPPoE connection info
* timeout -- how long to wait (in seconds)
*%RETURNS:
* Nothing
*%DESCRIPTION:
* Waits for a PADS packet and copies useful information
***********************************************************************/
void
waitForPADS(PPPoEConnection *conn, int timeout)
{
  fd_set readable;
  int r;
  struct timeval tv;
  PPPoEPacket packet;
  int len;
  printf("================>waitForPADS(): begin\n");
  do {
       if (BPF_BUFFER_IS_EMPTY)
         {
           tv.tv_sec = timeout;
           tv.tv_usec = 0;

           FD_ZERO(&readable);
           FD_SET(conn->discoverySocket, &readable);

           printf("================>waitForPADS():BPF_BUFFER_IS_EMPTY \n");
           while(1)
             {
               r = select(conn->discoverySocket+1, &readable, NULL, NULL, &tv);
               if (r >= 0 || errno != EINTR) break;
             }
           if (r < 0) { fatalSys("select (waitForPADS)"); }
           if (r == 0) return;
         }
       syslog(LOG_ERR, "===>PPPoE Discovery: receivED Packet PADS and checkING it ...... \n");
       /* Get the packet */
       receivePacket(conn->discoverySocket, &packet, &len);

       /* Check length */
       if (ntohs(packet.length) + HDR_SIZE > len)
         {
           syslog(LOG_ERR, "=====>PADS FAILED: bogus PPPoE length field (%u) ", (unsigned int) ntohs(packet.length));
           continue;
         }

#ifdef USE_BPF
       /* If it's not a Discovery packet, loop again */
       if (etherType(&packet) != Eth_PPPOE_Discovery)
      {
        syslog(LOG_ERR, "=====>PADS FAILED: it's NOT a Discovery packet!\n");
        continue;
      }
#endif
       if (conn->debugFile)
         {
           dumpPacket(conn->debugFile, &packet, "RCVD");
           fprintf(conn->debugFile, "\n");
           fflush(conn->debugFile);
         }

       /* If it's not from the AC, it's not for me */
       if (memcmp(packet.ethHdr.h_source, conn->peerEth, ETH_ALEN))
         {
           syslog(LOG_ERR, "=====>PADS FAILED: it's NOT from AC and NOT for me!\n");
           continue;
         }
       /* If it's not for us, loop again */
       if (!packetIsForMe(conn, &packet))
         {
           syslog(LOG_ERR, "=====>PADS FAILED: it's  NOT for us!\n");
           continue;
         }

       /* Is it PADS?  */
       if (packet.code == CODE_PADS)
         {
           /* Parse for goodies */
           printf("================>waitForPADS(): CODE FOR PADS\n");

           conn->PADSHadError = 0;
           parsePacket(&packet, parsePADSTags, conn);
           if (!conn->PADSHadError)
             {
               printf("================>waitForPADS(): PADSHadError\n");
               conn->discoveryState = STATE_SESSION;
               break;
             }
         }
       printf("================>waitForPADS(): conn->discoveryState= %d \n",conn->discoveryState );

  } while (conn->discoveryState != STATE_SESSION);

  /* Don't bother with ntohs; we'll just end up converting it back... */
  conn->session = packet.session;
#if (NETGEAR_ROUTER_SPEC)
  {
	char *buf;
	FILE *fp = fopen(LAST_ID_FILE, "w+");
	if (fp)
	{
		/* 
		  * to record session id and peer mac address
		  * format : session id:peer mac address
		  * ex. 10:001122334455
		  */
		if ((buf = malloc(BLOCK_SIZE)))
		{
			memset(buf, 0, BLOCK_SIZE);
			fseek(fp, BLOCK_SIZE, SEEK_SET);
			fread(buf, 1, BLOCK_SIZE, fp);
			sprintf(buf + BUFFER_OFFSET,"%7d:",packet.session);
			memcpy (buf + 8 + BUFFER_OFFSET, conn->peerEth, ETH_ALEN);
			fseek(fp, BLOCK_SIZE, SEEK_SET);
			fwrite (buf, 1, BLOCK_SIZE, fp);
			fflush(fp);
			free(buf);
		}
		fclose(fp);
	}
	clean_session_id = 0;
	padt_count = PADT_MAX_COUNT;
  }
#endif
  syslog(LOG_ERR, "=====>PADS PASSD: packet PADS verify OK!\n");
  syslog(LOG_INFO, "PPP session is %d", (int) ntohs(conn->session));

  /* RFC 2516 says session id MUST NOT be zero or 0xFFFF */
  if (ntohs(conn->session) == 0 || ntohs(conn->session) == 0xFFFF)
    {
      syslog(LOG_ERR, "===>PPP Session: AC used a session value of %x -- the AC is violating RFC 2516", (unsigned int) ntohs(conn->session));
    }
}

/**********************************************************************
*%FUNCTION: discovery
*%ARGUMENTS:
* conn -- PPPoE connection info structure
*%RETURNS:
* Nothing
*%DESCRIPTION:
* Performs the PPPoE discovery phase
***********************************************************************/
void
discovery(PPPoEConnection *conn)
{
    int padiAttempts = 0;
    int padrAttempts = 0;
    int timeout = conn->discoveryTimeout;
    syslog(LOG_ERR, "==>PPPD: performING the PPPoE Discovery Stage......\n");
    /* Skip discovery and don't open discovery socket? */
    if (conn->skipDiscovery && conn->noDiscoverySocket) {
    conn->discoveryState = STATE_SESSION;
    return;
    }

    conn->discoverySocket =
    openInterface(conn->ifName, Eth_PPPOE_Discovery, conn->myEth);

    /* Skip discovery? */
    if (conn->skipDiscovery) {
     syslog(LOG_ERR, "===>PPPD: skipED PPPoE Discovery, and turn into PPPOE Session Stage right now!\n");
    conn->discoveryState = STATE_SESSION;
    if (conn->killSession) {
      syslog(LOG_ERR, "===>PPPD: Send PADT and  Session killed manually!\n");

        sendPADT(conn, "RP-PPPoE: Session killed manually");
        exit(0);
    }
    return;
    }
    do {
    padiAttempts++;
    if (padiAttempts > MAX_PADI_ATTEMPTS) {
        if (persist) {
        padiAttempts = 0;
        timeout = conn->discoveryTimeout;
        printErr("Timeout waiting for PADO packets");
        } else {
        rp_fatal("Timeout waiting for PADO packets");
        }
    }
#if (NETGEAR_ROUTER_SPEC)
	if (padt_count)
	{
    	sendPADT(conn, "initial PPPoE connection");
		padt_count--;
	}
#endif
    sendPADI(conn);
    syslog(LOG_ERR, "===>PPPoE Discovery: sendED Packet PADI  successfully!\n");
    conn->discoveryState = STATE_SENT_PADI;
    syslog(LOG_ERR, "===>PPPoE Discovery: receivING Packet PADO ......\n");
    waitForPADO(conn, timeout);

    /* If we're just probing for access concentrators, don't do
       exponential backoff.  This reduces the time for an unsuccessful
       probe to 15 seconds. */
    if (!conn->printACNames) {
        timeout *= 2;
    }
    if (conn->printACNames && conn->numPADOs) {
        break;
    }
    } while (conn->discoveryState == STATE_SENT_PADI);

    /* If we're only printing access concentrator names, we're done */
    if (conn->printACNames) {
    exit(0);
    }

    timeout = conn->discoveryTimeout;
    do {
    padrAttempts++;
    if (padrAttempts > MAX_PADI_ATTEMPTS) {
        if (persist) {
        padrAttempts = 0;
        timeout = conn->discoveryTimeout;
        printErr("Timeout waiting for PADS packets");
        } else {
        rp_fatal("Timeout waiting for PADS packets");
        }
    }

    sendPADR(conn);
    syslog(LOG_ERR, "===>PPPoE Discovery: sendED Packet PADR  successfully!\n");
    conn->discoveryState = STATE_SENT_PADR;
    syslog(LOG_ERR, "===>PPPoE Discovery: receivING Packet PADS ......\n");
    waitForPADS(conn, timeout);

    //printf("================>discovery():pass waitForPADS; \n");

    timeout *= 2;
    } while (conn->discoveryState == STATE_SENT_PADR);
    syslog(LOG_ERR, "==>PPPD:  PPPoE Discovery Succeeded!\n");
    /* We're done. */
    syslog(LOG_ERR, "==>PPPD:  performING the PPP Session Stage......\n");
    conn->discoveryState = STATE_SESSION;

    return;
}
