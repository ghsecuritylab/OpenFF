#include "phupdate.h"
#include "log.h"
#include <signal.h>     /* for singal handle */
#ifndef WIN32
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <netdb.h>
#include <unistd.h>     /* for close() */

static void create_pidfile()
{
    FILE *pidfile;
    char pidfilename[128];
    sprintf(pidfilename, "%s", "/var/run/phddns.pid");
	
    if ((pidfile = fopen(pidfilename, "w")) != NULL) {
		fprintf(pidfile, "%d\n", getpid());
		(void) fclose(pidfile);
    } else {
		printf("Failed to create pid file %s: %m", pidfilename);
		pidfilename[0] = 0;
    }
}
#endif


PHGlobal global;
static void my_handleSIG (int sig)
{
	if (sig == SIGINT)
	{
#ifndef WIN32
		remove("/var/run/phddns.pid");
#endif
		printf ("signal = SIGINT\n");
		phddns_stop(&global);
		exit(0);
	}
	if (sig == SIGTERM)
	{
#ifndef WIN32
		remove("/var/run/phddns.pid");
#endif
		printf ("signal = SIGTERM\n");
		phddns_stop(&global);
	}
	signal (sig, my_handleSIG);
}

//狀態更新回調
static void myOnStatusChanged(int status, long data)
{
	printf("myOnStatusChanged %s", convert_status_code(status));
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

//域名註冊回調
static void myOnDomainRegistered(char *domain)
{
	printf("myOnDomainRegistered %s\n", domain);
}

//用戶信息XML數據回調
static void myOnUserInfo(char *userInfo, int len)
{
	printf("myOnUserInfo %s\n", userInfo);
}

//域名信息XML數據回調
static void myOnAccountDomainInfo(char *domainInfo, int len)
{
	printf("myOnAccountDomainInfo %s\n", domainInfo);
}

int main(int argc, char *argv[])
{
	void (*ohandler) (int);

#ifdef WIN32
	WORD VersionRequested;		// passed to WSAStartup
	WSADATA  WsaData;			// receives data from WSAStartup
	int error;
	
	VersionRequested = MAKEWORD(2, 0);
	
	//start Winsock 2
	error = WSAStartup(VersionRequested, &WsaData); 
	log_open("c:\\phclientlog.log", 1);	//empty file will cause we printf to stdout
#else
	
	
	log_open("/var/log/phddns.log", 1);	//empty file will cause we printf to stdout
	create_pidfile();
#endif


	ohandler = signal (SIGINT, my_handleSIG);
	if (ohandler != SIG_DFL) {
		printf ("previous signal handler for SIGINT is not a default handler\n");
		signal (SIGINT, ohandler);
	}
	signal (SIGTERM, my_handleSIG);

	init_global(&global);

	global.cbOnStatusChanged = myOnStatusChanged;
	global.cbOnDomainRegistered = myOnDomainRegistered;
	global.cbOnUserInfo = myOnUserInfo;
	global.cbOnAccountDomainInfo = myOnAccountDomainInfo;

	set_default_callback(&global);
	//注意！！！！！！！！！！！！！！！！！！！！！！！！
	//以下兩個值用戶您臨時測試（Oray可能隨時刪除或修改），正式發佈前請填寫您的實際分配值
	global.clientinfo = 0x26869735; 		//這裡填寫剛才第二步算出的值
	global.challengekey = 0x1E08AA4D;	//這裡填寫嵌入式認證碼
	//注意！！！！！！！！！！！！！！！！！！！！！！！！

	strcpy(global.szHost, 
		argv[1]);			//你所拿到的服務器地址
	strcpy(global.szUserID, 
		argv[2]);							//Oray賬號
	strcpy(global.szUserPWD, 
		argv[3]);							//對應的密碼
	
	for (;;)
	{
		int next = phddns_step(&global);
		sleep(next);
	}
	phddns_stop(&global);
	return 0;
}

