#include "phglobal.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      /* Needed only for _O_RDWR definition */
#include <stdlib.h>
#include <stdio.h>
#include "log.h"

#ifndef WIN32
#include <termios.h>
#endif

#include <time.h>

/* show status :
 * status0="Dynamic DNS service is not enabled."
 * status1="updated successfully at <time>, <date>."
 * status2="No update action. There is no IP address on the Internet port."
 * status3="Authentication failed. User Name/Password is not correct."
 * status4="Update failed. Host Name is not correct."
 * status5="Update failed at <time>, <date>.The service provider is not reachable."
 */
void show_ddns_status(int flag)
{
	FILE *fp;	
	if(!(fp = fopen(STATUS_FILE, "w")))	
	{		
		LOG(1) ("can not creat /tmp/ez-ipupd.status\n");		
		return;	
	}	
	fprintf(fp,"%d",flag);	
	fclose(fp);		
	
	if (flag == 1){		
		system("date > /tmp/ez-ipupd.time");
		syslog( 6, "[Dynamic DNS] host name %s registeration successful,", hostname);
	}else{ 
		if (flag == 5)
			system("date > /tmp/ez-ipupd.time");
		syslog( 6, "[Dynamic DNS] host name %s registeration failure,", hostname);
	}
}

const char *convert_status_code(int nCode)
{
	static char buf[64] = "";

	switch (nCode)
	{
	case okConnecting:
		strcpy(buf,"okConnecting");
		break;
	case okConnected:
		strcpy(buf,"okConnected");
		break;
	case okAuthpassed:
		strcpy(buf,"okAuthpassed");
		break;
	case okDomainListed:
		strcpy(buf,"okDomainListed");
		break;
	case okDomainsRegistered:
		strcpy(buf,"okDomainsRegistered");
		show_ddns_status(1);
		break;
	case okKeepAliveRecved:
		strcpy(buf,"okKeepAliveRecved");
		break;
	case okRetrievingMisc:
		strcpy(buf,"okRetrievingMisc");
		break;
	case errorConnectFailed:
		strcpy(buf,"errorConnectFailed");
		show_ddns_status(2);
		break;
	case errorSocketInitialFailed:
		strcpy(buf,"errorSocketInitialFailed");
		show_ddns_status(5);
		break;
	case errorAuthFailed:
		strcpy(buf,"errorAuthFailed");
		show_ddns_status(3);
		break;
	case errorDomainListFailed:
		strcpy(buf,"errorDomainListFailed");
		show_ddns_status(4);
		break;
	case errorDomainRegisterFailed:
		strcpy(buf,"errorDomainRegisterFailed");
		show_ddns_status(4);
		break;
	case errorUpdateTimeout:
		strcpy(buf,"errorUpdateTimeout");
		show_ddns_status(5);
		break;
	case errorKeepAliveError:
		strcpy(buf,"errorKeepAliveError");
		break;
	case errorRetrying:
		strcpy(buf,"errorRetrying");
		break;
	case okNormal:
		strcpy(buf,"okNormal");
		break;
	case okNoData:
		strcpy(buf,"okNoData");
		break;
	case okServerER:
		strcpy(buf,"okServerER");
		break;
	case errorOccupyReconnect:
		strcpy(buf,"errorOccupyReconnect");
		break;
	case okRedirecting:
		strcpy(buf,"okRedirecting");
		break;
	case errorAuthBusy:
		strcpy(buf,"errorAuthBusy");
		show_ddns_status(5);
		break;
	case errorStatDetailInfoFailed:
		strcpy(buf,"errorAuthBusy");
		show_ddns_status(5);
		break;
	}

	return buf;
}


const char *my_inet_ntoa(long ip)
{
	struct in_addr addr;
	addr.s_addr = ip;
	return inet_ntoa(addr);
}

static void defOnStatusChanged(int status, long data)
{
	printf("defOnStatusChanged %s", convert_status_code(status));
	if (status == okKeepAliveRecved)
	{
		printf(", IP: %d", data);
	}
	if (status == okDomainsRegistered)
	{
		printf(", UserType: %d", data);
	}
	printf("\n");
}

static void defOnDomainRegistered(char *domain)
{
	printf("defOnDomainRegistered %s\n", domain);
}

static void defOnUserInfo(char *userInfo, int len)
{
	printf("defOnUserInfo %s\n", userInfo);
}

static void defOnAccountDomainInfo(char *domainInfo, int len)
{
	printf("defOnAccountDomainInfo %s\n", domainInfo);
}

void init_global(PHGlobal *global)
{
	strcpy(global->szHost,"phddns60.oray.net");
	strcpy(global->szUserID,"");
	strcpy(global->szUserPWD,"");
	strcpy(global->szBindAddress,"");
	global->nUserType = 0;
	global->nPort = 6060;

	global->bTcpUpdateSuccessed = FALSE;
	strcpy(global->szChallenge,"");
	global->nChallengeLen = 0;
	global->nChatID = global->nStartID = global->nLastResponseID = global->nAddressIndex = 0;
	global->tmLastResponse = -1;
	global->ip = 0;
	strcpy(global->szTcpConnectAddress,"");

	global->cLastResult = -1;

	global->uptime = time(0);
	global->lasttcptime = 0;

	strcpy(global->szActiveDomains[0],".");

	global->bNeed_connect = TRUE;
	global->tmLastSend = 0;

	global->m_tcpsocket = global->m_udpsocket = INVALID_SOCKET;
	
	global->cbOnStatusChanged = NULL;
	global->cbOnDomainRegistered = NULL;
	global->cbOnUserInfo = NULL;
	global->cbOnAccountDomainInfo = NULL;
}

void set_default_callback(PHGlobal *global)
{
	global->cbOnStatusChanged = defOnStatusChanged;
	global->cbOnDomainRegistered = defOnDomainRegistered;
	global->cbOnUserInfo = defOnUserInfo;
	global->cbOnAccountDomainInfo = defOnAccountDomainInfo;
}

