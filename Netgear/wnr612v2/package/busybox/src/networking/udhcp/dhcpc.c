/* dhcpc.c
 *
 * udhcp DHCP client
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sys/time.h>
#include <sys/file.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>

#include "dhcpd.h"
#include "dhcpc.h"
#include "options.h"
#include "clientpacket.h"
#include "clientsocket.h"
#include "script.h"
#include "socket.h"
#include "common.h"
#include "signalpipe.h"

static int state;
static unsigned long requested_ip; /* = 0 */
static unsigned long server_addr;
static unsigned long timeout;
static int packet_num; /* = 0 */
static int fd = -1;
static int decline_packet_num = 0;

#define LISTEN_NONE 0
#define LISTEN_KERNEL 1
#define LISTEN_RAW 2
static int listen_mode;

#ifdef IMPROVE_SEND_DISCOVERY
#define DIS_TIME_OUT 4
#endif

struct client_config_t client_config = {
    /* Default options. */
    abort_if_no_lease: 0,
    foreground: 0,
    quit_after_lease: 0,
    background_if_no_lease: 0,
    interface: "eth0",
    pidfile: "/var/run/udhcpc.pid",
    script: DEFAULT_SCRIPT,
    clientid: NULL,
    vendorid: NULL,
    vendorinfo: NULL,
    hostname: NULL,
    ifindex: 0,
    arp: "\0\0\0\0\0\0",        /* appease gcc-3.0 */
#ifdef SUPPORT_OPTIONS_33_AND_121
    staticRoute:0,
#endif
};

#ifndef IN_BUSYBOX
static void __attribute__ ((noreturn)) show_usage(void)
{
    printf(
"Usage: udhcpc [OPTIONS]\n\n"
"  -c, --clientid=CLIENTID         Set client identifier\n"
"  -C, --clientid-none             Suppress default client identifier\n"
"  -H, --hostname=HOSTNAME         Client hostname\n"
"  -h                              Alias for -H\n"
"  -f, --foreground                Do not fork after getting lease\n"
"  -b, --background                Fork to background if lease cannot be\n"
"                                  immediately negotiated.\n"
"  -i, --interface=INTERFACE       Interface to use (default: eth0)\n"
"  -n, --now                       Exit with failure if lease cannot be\n"
"                                  immediately negotiated.\n"
"  -p, --pidfile=file              Store process ID of daemon in file\n"
"  -q, --quit                      Quit after obtaining lease\n"
"  -r, --request=IP                IP address to request (default: none)\n"
#ifdef SUPPORT_OPTIONS_33_AND_121
"  -R, --sRouter                Support static router or more ... \n"
#endif
"  -s, --script=file               Run file at dhcp events (default:\n"
"                                  " DEFAULT_SCRIPT ")\n"
"  -v, --version                   Display version\n"
    );
    exit(0);
}
#else
#define show_usage bb_show_usage
extern void show_usage(void) __attribute__ ((noreturn));
#endif


/* just a little helper */
static void change_mode(int new_mode)
{
    DEBUG(LOG_INFO, "entering %s listen mode",
        new_mode ? (new_mode == 1 ? "kernel" : "raw") : "none");
    if (fd >= 0) close(fd);
    fd = -1;
    listen_mode = new_mode;
}


/* perform a renew */
static void perform_renew(void)
{
    LOG(LOG_INFO, "Performing a DHCP renew");
    switch (state) {
    case BOUND:
        change_mode(LISTEN_KERNEL);
    case RENEWING:
    case REBINDING:
        state = RENEW_REQUESTED;
        break;
    case RENEW_REQUESTED: /* impatient are we? fine, square 1 */
        run_script(NULL, "deconfig");
    case REQUESTING:
    case RELEASED:
        change_mode(LISTEN_RAW);
        state = INIT_SELECTING;
        break;
    case INIT_SELECTING:
        break;
    }

    /* start things over */
    packet_num = 0;

    /* Kill any timeouts because the user wants this to hurry along */
    timeout = 0;
}


/* perform a release */
static void perform_release(void)
{
    char buffer[16];
    struct in_addr temp_addr;

    /* send release packet */
    if (state == BOUND || state == RENEWING || state == REBINDING) {
        temp_addr.s_addr = server_addr;
        sprintf(buffer, "%s", inet_ntoa(temp_addr));
        temp_addr.s_addr = requested_ip;
        LOG(LOG_INFO, "Unicasting a release of %s to %s",
                inet_ntoa(temp_addr), buffer);
        send_release(server_addr, requested_ip); /* unicast */
        run_script(NULL, "deconfig");
    }
    LOG(LOG_INFO, "Entering released state");

    change_mode(LISTEN_NONE);
    state = RELEASED;
    timeout = 0x7fffffff;
}


static void client_background(void)
{
    background(client_config.pidfile);
    client_config.foreground = 1; /* Do not fork again. */
    client_config.background_if_no_lease = 0;
}

/*get option 60,61,125. Allow blank space in these options.*/
get_options(int argc, char *argv[])
{
    int i, len;
    char tmp[255];
    tmp[0]='\0';

    for(i=1; i<argc; i++)
    {
        if(strcmp(argv[i],"-d")==0)             //vendor class identifier
        {
            strcpy(tmp,argv[++i]);
            if(i != (argc-1))       //if argc[i] not the last argument
                while(argv[i+1][0]!='-'){
                    strcat(tmp," ");
                    strcat(tmp,argv[++i]);
                     if(i == (argc-1))
                         break;
                }
            len = strlen(tmp);
            if (client_config.vendorid) free(client_config.vendorid);
            client_config.vendorid = xmalloc(len + 2);
            client_config.vendorid[OPT_CODE] = DHCP_VENDOR_ID;
            client_config.vendorid[OPT_LEN] = len;
            client_config.vendorid[OPT_DATA] = '\0';
            strncpy(client_config.vendorid + OPT_DATA, tmp, len);
            tmp[0]='\0';
        }
        else if (strcmp(argv[i],"-c") == 0) {           //client identifier
                strcpy(tmp,argv[++i]);
                if(i != (argc-1))
                     while(argv[i+1][0]!='-'){
                         strcat(tmp," ");
                         strcat(tmp,argv[++i]);
                         if(i == (argc-1))
                            break;
                    }
                len = strlen(tmp);
                if (client_config.clientid) free(client_config.clientid);
                client_config.clientid = xmalloc(len + 3);
                client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
                client_config.clientid[OPT_LEN] = len+1;
                client_config.clientid[OPT_DATA] = '0';
                strncpy(client_config.clientid + 3, tmp, len);
                tmp[0]='\0';
        }
        else if (strcmp(argv[i],"-e") ==0){             //vendor specific information
                 strcpy(tmp,argv[++i]);
                if(i != (argc-1))
                    while(argv[i+1][0]!='-'){
                        strcat(tmp," ");
                        strcat(tmp,argv[++i]);
                        if(i == (argc-1))
                            break;
                    }
                len = strlen(tmp);
                if (client_config.vendorinfo) free(client_config.vendorinfo);
                client_config.vendorinfo = xmalloc(len + 2);
                client_config.vendorinfo[OPT_CODE] = DHCP_VENDOR_INFO;
                client_config.vendorinfo[OPT_LEN] = len;
                client_config.vendorinfo[OPT_DATA] = '\0';
                strncpy(client_config.vendorinfo + OPT_DATA, tmp, len);
                tmp[0]='\0';
        }
    }
}
/*return:   1  addr free
 *      0  addr used
 *      -1 error
*/
static int client_ip_check(uint32_t addr)
{
    uint32_t myip = 0;
    uint8_t *mac = client_config.arp;
    //char *interface = "eth2";
    char *interface, buf[10], *ptr;

    strcpy(buf, client_config.interface);
    ptr = strchr(buf, ':');
    if (ptr) {
        *ptr = 0;
    }
    interface = buf;
    return arpping(addr, myip, mac, interface);
}

#ifdef COMBINED_BINARY
int udhcpc_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    uint8_t *temp, *message;
    unsigned long t1 = 0, t2 = 0, xid = 0;
    unsigned long start = 0, lease;
    fd_set rfds;
    int retval;
    struct timeval tv;
    int c, len;
    struct dhcpMessage packet;
    struct in_addr temp_addr;
    long now;
    int max_fd;
    int sig;
    int no_clientid = 0;

    //time_t timer;
    get_options(argc, argv); /* get option 60,61 and 125*/
    static const struct option arg_options[] = {
        {"clientid",    required_argument,  0, 'c'},
        {"clientid-none", no_argument,      0, 'C'},
        {"foreground",  no_argument,        0, 'f'},
        {"background",  no_argument,        0, 'b'},
        {"hostname",    required_argument,  0, 'H'},
        {"hostname",    required_argument,      0, 'h'},
        {"interface",   required_argument,  0, 'i'},
        {"now",     no_argument,        0, 'n'},
        {"pidfile", required_argument,  0, 'p'},
        {"quit",    no_argument,        0, 'q'},
        {"request", required_argument,  0, 'r'},
#ifdef SUPPORT_OPTIONS_33_AND_121
        {"sRouter", required_argument,  0, 'R'},
#endif
        {"script",  required_argument,  0, 's'},
        {"version", no_argument,        0, 'v'},
        {"vendorid",    required_argument,  0, 'd'},
        {"vendorinfo",  required_argument,  0, 'e'},
        {0, 0, 0, 0}
    };

    /* get options */
    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, "c:CfbH:h:i:np:qRr:s:vd:e:", arg_options, &option_index);
        if (c == -1) break;

        switch (c) {
        case 'c'://parsing in get_options()
            break;
        case 'd'://parsing in get_options()
            break;
        case 'e'://parsing in get_options()
            break;
        case 'C':
            if (client_config.clientid) show_usage();
            no_clientid = 1;
            break;
        case 'f':
            client_config.foreground = 1;
            break;
        case 'b':
            client_config.background_if_no_lease = 1;
            break;
        case 'h':
        case 'H':
            len = strlen(optarg) > 255 ? 255 : strlen(optarg);
            if (client_config.hostname) free(client_config.hostname);
            client_config.hostname = xmalloc(len + 2);
            client_config.hostname[OPT_CODE] = DHCP_HOST_NAME;
            client_config.hostname[OPT_LEN] = len;
            strncpy(client_config.hostname + 2, optarg, len);
            break;
        case 'i':
            client_config.interface =  optarg;
            break;
        case 'n':
            client_config.abort_if_no_lease = 1;
            break;
        case 'p':
            client_config.pidfile = optarg;
            break;
        case 'q':
            client_config.quit_after_lease = 1;
            break;
        case 'r':
            requested_ip = inet_addr(optarg);
            break;
#ifdef SUPPORT_OPTIONS_33_AND_121
        case 'R':
            client_config.staticRoute = 1;
            break;
#endif
        case 's':
            client_config.script = optarg;
            break;
        case 'v':
            printf("udhcpcd, version %s\n\n", VERSION);
            return 0;
            break;
        default:
            show_usage();
        }
    }

    /* Start the log, sanitize fd's, and write a pid file */
    start_log_and_pid("udhcpc", client_config.pidfile);

    if (read_interface(client_config.interface, &client_config.ifindex,
               NULL, client_config.arp) < 0)
        return 1;

    /* if not set, and not suppressed, setup the default client ID */
    if (!client_config.clientid && !no_clientid) {
        client_config.clientid = xmalloc(6 + 3);
        client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
        client_config.clientid[OPT_LEN] = 7;
        client_config.clientid[OPT_DATA] = 1;
        memcpy(client_config.clientid + 3, client_config.arp, 6);
    }

    /* setup the signal pipe */
    udhcp_sp_setup();

    state = INIT_SELECTING;
    run_script(NULL, "deconfig");
    change_mode(LISTEN_RAW);

    for (;;) {

        tv.tv_sec = timeout - uptime();
        tv.tv_usec = 0;

        if (listen_mode != LISTEN_NONE && fd < 0) {
            if (listen_mode == LISTEN_KERNEL)
                fd = listen_socket(INADDR_ANY, CLIENT_PORT, client_config.interface);
            else
                fd = raw_socket(client_config.ifindex);
            if (fd < 0) {
                LOG(LOG_ERR, "FATAL: couldn't listen on socket, %m");
                return 0;
            }
        }
        max_fd = udhcp_sp_fd_set(&rfds, fd);

        if (tv.tv_sec > 0) {
#define SELECT_TIMEOUT_LIMIT 3600
            if (tv.tv_sec <= SELECT_TIMEOUT_LIMIT) {
                DEBUG(LOG_INFO, "[dhcp client]Waiting %ld secs on select...", tv.tv_sec);
                retval = select(max_fd + 1, &rfds, NULL, NULL, &tv);
                DEBUG(LOG_INFO, "[dhcp client]Leaving select...");
            } else {
                unsigned int round_counter, delay_per_round;
                struct timeval mytv;
                unsigned int delay_round = 1+ (tv.tv_sec / SELECT_TIMEOUT_LIMIT);
#undef SELECT_TIMEOUT_LIMIT

                delay_per_round = tv.tv_sec / delay_round;
                DEBUG(LOG_INFO, "[dhcp client]Waiting %ld secs on select with %u round(s) of %u secs...", tv.tv_sec, delay_round, delay_per_round);
                for (round_counter=0; round_counter<delay_round; round_counter++) {
                    mytv.tv_sec = delay_per_round;
                    mytv.tv_usec = 0;
                    DEBUG(LOG_INFO, "[dhcp client]Waiting %ld secs on select...", mytv.tv_sec);
                    retval = select(max_fd + 1, &rfds, NULL, NULL, &mytv);
                    if (retval != 0) { // Is it possible?
                        break; // break this for loop
                    }
                    DEBUG(LOG_INFO, "[dhcp client]Leaving select with retval %d, %u round(s) to go", retval, (delay_round-round_counter-1));
                }
            }
        } else {
            DEBUG(LOG_INFO, "No waiting due to negative tv ", tv.tv_sec);
            retval = 0; /* If we already timed out, fall through */
        }
        now = uptime();
        if (retval == 0) {
            /* timeout dropped to zero */
            switch (state) {
            case INIT_SELECTING:
#ifdef IMPROVE_SEND_DISCOVERY
                if (packet_num < 30) {
#else
								if (packet_num < 5) {
#endif
                    if (packet_num == 0)
                        xid = random_xid();

                    /* send discover packet */
#ifdef IMPROVE_SEND_DISCOVERY
                    if(packet_num ==0)
                    	printf("Start sending discover ...\n");
#endif
                    send_discover(xid, requested_ip); /* broadcast */

                    timeout = now + DIS_TIME_OUT;
                    packet_num++;
                } else {
                    run_script(NULL, "leasefail");
                    if (client_config.background_if_no_lease) {
                        LOG(LOG_INFO, "No lease, forking to background.");
                        client_background();
                    } else if (client_config.abort_if_no_lease) {
                        LOG(LOG_INFO, "No lease, failing.");
                        return 1;
                    }
                    /* wait to try again */
                    packet_num = 0;
                    /*timeout = now + 300;*/
                    //timeout = now + 60;
                    timeout = now + 300;//by Aaron 10/30/2008, for nategear spaces.
                }
                break;
            case DECLINE:
    //          send_decline(xid, requested_ip);
    //          sleep(10);
#ifdef IMPROVE_SEND_DISCOVERY
                if (decline_packet_num < 30) {
#else
								if (decline_packet_num < 5) {
#endif
                    send_decline(xid, requested_ip);
                    sleep(10);//By Aaron 10/30/2008 , for netgetar specs.
                    requested_ip = 0;
                    decline_packet_num++;
                } else {
                    run_script(NULL, "leasefail");
                    if (client_config.background_if_no_lease) {
                        LOG(LOG_INFO, "No lease, forking to background.");
                        client_background();
                    } else if (client_config.abort_if_no_lease) {
                        LOG(LOG_INFO, "No lease, failing.");
                        return 1;
                    }
                    /* wait to try again */
                    decline_packet_num = 0;
                    /*timeout = now + 300;*/
                    //timeout = now + 60;
                    timeout = now + 300;//by Aaron 10/30/2008, for nategear spaces.
                }

                state = INIT_SELECTING;
                break;
            case RENEW_REQUESTED:
            case REQUESTING:
                if (packet_num < 3) {
                    /* send request packet */
                    if (state == RENEW_REQUESTED)
                        send_renew(xid, server_addr, requested_ip); /* unicast */
                    else send_selecting(xid, server_addr, requested_ip); /* broadcast */

                    timeout = now + ((packet_num == 2) ? 10 : 2);
                    packet_num++;
                } else {
                    /* timed out, go back to init state */
                    if (state == RENEW_REQUESTED) run_script(NULL, "deconfig");
                    state = INIT_SELECTING;
                    timeout = now;
                    packet_num = 0;
                    change_mode(LISTEN_RAW);
                }
                break;
            case BOUND:
                /* Lease is starting to run out, time to enter renewing state */
                state = RENEWING;
                change_mode(LISTEN_KERNEL);
                DEBUG(LOG_INFO, "[dhcp client]Entering renew state");
                /* fall right through */
            case RENEWING:
                /* Either set a new T1, or enter REBINDING state */
                if ((t2 - t1) <= (lease / 14400 + 1)) {
                    /* timed out, enter rebinding state */
                    state = REBINDING;
                    timeout = now + (t2 - t1);
                    DEBUG(LOG_INFO, "Entering rebinding state");
                } else {
                    /* send a request packet */
                    send_renew(xid, server_addr, requested_ip); /* unicast */

                    t1 = (t2 - t1) / 2 + t1;
                    timeout = t1 + start;
                }
                break;
            case REBINDING:
                /* Either set a new T2, or enter INIT state */
                if ((lease - t2) <= (lease / 14400 + 1)) {
                    /* timed out, enter init state */
                    state = INIT_SELECTING;
                    LOG(LOG_INFO, "Lease lost, entering init state");
                    run_script(NULL, "deconfig");
                    timeout = now;
                    packet_num = 0;
                    change_mode(LISTEN_RAW);
                } else {
                    /* send a request packet */
                    send_renew(xid, 0, requested_ip); /* broadcast */

                    t2 = (lease - t2) / 2 + t2;
                    timeout = t2 + start;
                }
                break;
            case RELEASED:
                /* yah, I know, *you* say it would never happen */
                timeout = 0x7fffffff;
                break;
            }
        } else if (retval > 0 && listen_mode != LISTEN_NONE && FD_ISSET(fd, &rfds)) {
            /* a packet is ready, read it */

            if (listen_mode == LISTEN_KERNEL)
                len = get_packet(&packet, fd);
            else len = get_raw_packet(&packet, fd);

            if (len == -1 && errno != EINTR) {
                DEBUG(LOG_INFO, "error on read, %m, reopening socket");
                change_mode(listen_mode); /* just close and reopen */
            }
            if (len < 0) continue;

            if (packet.xid != xid) {
                DEBUG(LOG_INFO, "Ignoring XID %lx (our xid is %lx)",
                    (unsigned long) packet.xid, xid);
                continue;
            }
            /* Ignore packets that aren't for us */
            if (memcmp(packet.chaddr, client_config.arp, 6)) {
                DEBUG(LOG_INFO, "packet does not have our chaddr -- ignoring");
                continue;
            }

            if ((message = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
                DEBUG(LOG_ERR, "couldnt get option from packet -- ignoring");
                continue;
            }

            switch (state) {
            case INIT_SELECTING:
                /* Must be a DHCPOFFER to one of our xid's */
                if (*message == DHCPOFFER) {
                    if ((temp = get_option(&packet, DHCP_SERVER_ID))) {
                        memcpy(&server_addr, temp, 4);
                        xid = packet.xid;
                        requested_ip = packet.yiaddr;

                        /* enter requesting state */
                        state = REQUESTING;
                        timeout = now;
                        packet_num = 0;
                    } else {
                        DEBUG(LOG_ERR, "No server ID in message");
                    }

                    if (!client_ip_check(requested_ip)) {
                        state = DECLINE;
                        LOG(LOG_ERR, "ip %s conflict !!",
                            inet_ntoa(*((struct in_addr *)&requested_ip)));
                    }
                }
                break;
            case RENEW_REQUESTED:
            case REQUESTING:
            case RENEWING:
            case REBINDING:
                if (*message == DHCPACK) {
                    if (!(temp = get_option(&packet, DHCP_LEASE_TIME))) {
                        LOG(LOG_ERR, "No lease time with ACK, using 1 hour lease");
                        lease = 60 * 60;
                    } else {
                        memcpy(&lease, temp, 4);
                        lease = ntohl(lease);
                    }

                    /* enter bound state */
                    t1 = lease / 2;

                    /* little fixed point for n * .875 */
                    t2 = (lease * 0x7) >> 3;
                    temp_addr.s_addr = packet.yiaddr;
                    LOG(LOG_INFO, "[dhcp client]Lease of %s obtained, lease time %ld",
                        inet_ntoa(temp_addr), lease);
                    //timer=time(0);
                    //syslog(6, "[Internet connected] IP address: %s,", inet_ntoa(temp_addr) );
                    start = now;
                    timeout = t1 + start;
                    requested_ip = packet.yiaddr;

                    #ifdef CONFLICT
                    if(check_ip_conflict(&packet) == 1){
						sleep(10);
                        run_script(NULL, "deconfig");
                        state = INIT_SELECTING;
                        timeout = now;
                        requested_ip = 0;
                        packet_num = 0;
                        change_mode(LISTEN_RAW);
                        break;
                    }
                    #endif

                    run_script(&packet,
                           ((state == RENEWING || state == REBINDING) ? "renew" : "bound"));

                    state = BOUND;
                    change_mode(LISTEN_NONE);
                    if (client_config.quit_after_lease)
                        return 0;
                    if (!client_config.foreground)
                        client_background();

                } else if (*message == DHCPNAK) {
                    /* return to init state */
                    LOG(LOG_INFO, "Received DHCP NAK");
                    run_script(&packet, "nak");
                    if (state != REQUESTING)
                        run_script(NULL, "deconfig");
                    state = INIT_SELECTING;
                    timeout = now;
                    requested_ip = 0;
                    packet_num = 0;
                    change_mode(LISTEN_RAW);
                    sleep(3); /* avoid excessive network traffic */
                }
                break;
            /* case BOUND, RELEASED: - ignore all packets */
            }
        } else if (retval > 0 && (sig = udhcp_sp_read(&rfds))) {
            switch (sig) {
            case SIGUSR1:
                perform_renew();
                break;
            case SIGUSR2:
                perform_release();
                break;
            case SIGTERM:
                LOG(LOG_INFO, "Received SIGTERM");
                //timer = time(0);
                syslog(6, "[Internet disconnected]" );
                return 0;
            }
        } else if (retval == -1 && errno == EINTR) {
            /* a signal was caught */
        } else {
            /* An error occured */
            DEBUG(LOG_ERR, "Error on select");
        }

    }
    return 0;
}
