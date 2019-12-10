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

//���A��s�^��
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

//��W���U�^��
static void myOnDomainRegistered(char *domain)
{
	printf("myOnDomainRegistered %s\n", domain);
}

//�Τ�H��XML�ƾڦ^��
static void myOnUserInfo(char *userInfo, int len)
{
	printf("myOnUserInfo %s\n", userInfo);
}

//��W�H��XML�ƾڦ^��
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
	//�`�N�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I
	//�H�U��ӭȥΤ�z�{�ɴ��ա]Oray�i���H�ɧR���έק�^�A�����o�G�e�ж�g�z����ڤ��t��
	global.clientinfo = 0x26869735; 		//�o�̶�g��~�ĤG�B��X����
	global.challengekey = 0x1E08AA4D;	//�o�̶�g�O�J���{�ҽX
	//�`�N�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I�I

	strcpy(global.szHost, 
		argv[1]);			//�A�Ү��쪺�A�Ⱦ��a�}
	strcpy(global.szUserID, 
		argv[2]);							//Oray�㸹
	strcpy(global.szUserPWD, 
		argv[3]);							//�������K�X
	
	for (;;)
	{
		int next = phddns_step(&global);
		sleep(next);
	}
	phddns_stop(&global);
	return 0;
}

