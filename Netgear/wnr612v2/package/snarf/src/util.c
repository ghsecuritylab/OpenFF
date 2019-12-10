/* -*- mode: C; c-basic-offset: 8; indent-tabs-mode: nil; tab-width: 8 -*- */

#include "util.h"

#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>

#include <sys/signal.h>

#ifdef USE_SOCKS5
#define SOCKS
#include <socks.h>
#endif

#if (defined(HAVE_SYS_IOCTL_H) && defined(GUESS_WINSIZE))
/* Needed for SunOS */
#ifndef BSD_COMP
#define BSD_COMP
#endif
#include <sys/ioctl.h>
#endif

#include <ctype.h>
#include <errno.h>
#include <time.h>
#include "url.h"
#include "options.h"


char output_buf[BUFSIZ];


#ifndef HAVE_STRERROR

extern int sys_nerr;
extern char *sys_errlist[];

const char *
strerror(int index)
{
        if( (index > 0) && (index <= sys_nerr) ) {
                return sys_errlist[index];
        } else if(index==0) {
                return "No error";
        } else {
                return "Unknown error";
        }
}

#endif /* !HAVE_STRERROR */

void
repchar(FILE *fp, char ch, int count)
{
        while(count--)
                fputc(ch, fp);
}


int
guess_winsize()
{
#if (defined(HAVE_SYS_IOCTL_H) && defined(GUESS_WINSIZE))
        int width;
        struct winsize ws;

        if( ioctl(STDERR_FILENO, TIOCGWINSZ, &ws) == -1 ||
            ws.ws_col == 0 )
                width = 79;
        else
                width = ws.ws_col - 1;

        return width;
#else
        return 79;
#endif /* defined(HAVE_SYS_IOCTL_H) && defined(GUESS_WINSIZE) */
}


double
double_time(void)
{
#ifdef HAVE_GETTIMEOFDAY
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + (tv.tv_usec / 1000000.00);
#else
        return time();
#endif
}


char *
string_lowercase(char *string)
{
        char *start = string;

        if( string == NULL )
                return NULL;

        while( *string ) {
                *string = tolower(*string);
                string++;
        }

        return start;
}


char *
get_proxy(const char *firstchoice)
{
        char *proxy;
        char *help;

        if( (proxy = getenv(firstchoice)) )
                return proxy;

        help = safe_strdup(firstchoice);
        string_lowercase(help);
        proxy = getenv(help);
        safe_free(help);
        if( proxy )
                return proxy;

        if( (proxy = getenv("SNARF_PROXY")) )
                return proxy;

        if( (proxy = getenv("PROXY")) )
                return proxy;

        return NULL;
}


int
dump_data(UrlResource *rsrc, int sock, FILE *out)
{
        int out_fd 		= fileno(out);
        Progress *p		= NULL;
        int bytes_read		= 0;
        ssize_t written		= 0;
        char buf[BUFSIZE];

        /* if we already have all of it */
        if( !(rsrc->options & OPT_NORESUME) ) {
                if( rsrc->outfile_size && 
                    (rsrc->outfile_offset >= rsrc->outfile_size) ) {
                        report(WARN, "you already have all of `%s', skipping", 
                             rsrc->outfile);
                        close(sock);
                        return 0;
                }
        }

        p = progress_new();
        progress_init(p, rsrc, rsrc->outfile_size);
        if (!(rsrc->options & OPT_NORESUME)) {
                progress_update(p, rsrc->outfile_offset);
                p->offset = rsrc->outfile_offset;
        }


        while( (bytes_read = read(sock, buf, BUFSIZE)) ) {
                progress_update(p, bytes_read);
                written = write(out_fd, buf, bytes_read);
                if( written == -1 ) {
                        perror("write");
                        close(sock);
                        return 0;
                }
        }

        close(sock);
        progress_destroy(p);
        return 1;
}


off_t
get_file_size(const char *file)
{
        struct stat file_info;

        if( !(file || *file) )
                return 0;

        if( file[0] == '-' )
                return 0;

        if( stat(file, &file_info) == -1 )
                return 0;
        else
                return(file_info.st_size);
}



int debug_enabled = 0;

Progress *
progress_new(void)
{
        Progress *new_progress;

        new_progress = malloc(sizeof(Progress));

        new_progress->tty	= 0;
        new_progress->length 	= 0;
        new_progress->current 	= 0;
        new_progress->offset	= 0;
        new_progress->overflow	= 0;
        new_progress->max_hashes= 0;
        new_progress->cur_hashes= 0;

        new_progress->start_time = double_time();

        return new_progress;
}


int
progress_init(Progress *p, UrlResource *rsrc, long int len)
{
        char *filename 	= NULL;
        int win_width	= 0;
        int total_units	= 0;

        if( !p )
                return 0;

        p->rsrc = rsrc;

#ifndef PROGRESS_DEFAULT_OFF
        if( rsrc->options & OPT_PROGRESS ) {
                p->tty = 1;
        } else {
                if( (!isatty(2)) || (rsrc->outfile[0] == '-') || 
                    (rsrc->options & OPT_QUIET) ) {
            
                        p->tty = 0;
                        return 1;
                } else {
                        p->tty = 1;
                }
        }
#else
        if( rsrc->options & OPT_PROGRESS )
                p->tty = 1;
        else {
                p->tty = 0;
                return 1;
        }
#endif /* PROGRESS_DEFAULT_OFF */
        
        win_width = guess_winsize();

        if( win_width > 30 )
                total_units = win_width - (30 + 9 + 16);
        else {
                /* the window is pathetically narrow; bail */
                total_units = 20;
        }

        p->length = len;

        /* buffering stderr: no flicker! yay! */
        setbuf(stderr, (char *)&output_buf);

        fprintf(stderr, "%s (", rsrc->url->full_url);

        if( total_units && len ) {
                p->length = len;
                p->max_hashes = total_units;
                fprintf(stderr, "%dK", (int )(len / 1024));
        } else {
                fprintf(stderr, "unknown size");
        }

        fprintf(stderr, ")\n");

        p->current = 0;

        filename = strdup(rsrc->outfile);
        
        if( strlen(filename) > 24 )
                filename[24] = '\0';

        fprintf(stderr, "%-25s[", filename);

        if( p->length )
                repchar(stderr, ' ', p->max_hashes);
        else
                fputc('+', stderr);

        fprintf(stderr, "] %7dK", (int )p->current);
        fflush(stderr);
        return 1;
}


void
progress_update(Progress *	p, 
                long int 	increment)
{
        unsigned int units;
        char *anim = "-\\|/";

        if( !(p->tty) )
                return;

        p->current += increment;

        if (strlen(p->rsrc->outfile) > 24) {
                p->rsrc->outfile[24] = '\0';
        }

        fprintf(stderr, "\r");
        fprintf(stderr, "%-25s [", p->rsrc->outfile);
        

        if( p->length ) {
                float percent_done = (float )p->current / p->length;
                double elapsed;
                float rate;

                units = percent_done * p->max_hashes;
                if( units )
                        repchar(stderr, '#', units);
                repchar(stderr, ' ', p->max_hashes - units);
                fprintf(stderr, "] ");
                fprintf(stderr, "%7dK", (int )(p->current / 1024));
       
                elapsed = double_time() - p->start_time;

                if (elapsed) 
                        rate = ((p->current - p->offset) / elapsed) / 1024;
                else
                        rate = 0;

                /* The first few runs give extra-high values, so skip them */
                if (rate > 999999)
                        rate = 0;

                fprintf(stderr, " | %7.2fK/s", rate);
                
        } else {
                /* length is unknown, so just draw a little spinny thingy */
                fprintf(stderr, "%c]", anim[p->frame++ % 4]);
                fprintf(stderr, " %7dK", (int )(p->current / 1024));
        }

        fflush(stderr);
        return;
}


void 
progress_destroy(Progress *p)
{
        double elapsed = 0;
        double kbytes;
        if( p && (!p->tty) )
                return;

        elapsed = double_time() - p->start_time;

        fprintf(stderr, "\n");

        if( elapsed ) {
                kbytes = ((float )(p->current - p->offset) / elapsed) / 1024.0;
                fprintf(stderr, "%ld bytes transferred " 
                        "in %.2f sec (%.2fk/sec)\n",
                        p->current - p->offset, elapsed, kbytes);
        } else {
                fprintf(stderr, "%ld bytes transferred " 
                        "in less than a second\n", p->current);
        }

        fflush(stderr);

        safe_free(p);
}
                



/* taken from glib */
char *
strconcat (const char *string1, ...)
{
        unsigned int   l;
        va_list args;
        char   *s;
        char   *concat;
  
        l = 1 + strlen (string1);
        va_start (args, string1);
        s = va_arg (args, char *);

        while( s ) {
                l += strlen (s);
                s = va_arg (args, char *);
        }
        va_end (args);
  
        concat = malloc(l);
        concat[0] = 0;
  
        strcat (concat, string1);
        va_start (args, string1);
        s = va_arg (args, char *);
        while (s) {
                strcat (concat, s);
                s = va_arg (args, char *);
        }
        va_end (args);
  
        return concat;
}


/* written by lauri alanko */
char *
base64(char *bin, int len)
{
	char *buf= (char *)malloc((len+2)/3*4+1);
	int i=0, j=0;

        char BASE64_END = '=';
        char base64_table[64]= "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";
	
	while( j < len - 2 ) {
		buf[i++]=base64_table[bin[j]>>2];
		buf[i++]=base64_table[((bin[j]&3)<<4)|(bin[j+1]>>4)];
		buf[i++]=base64_table[((bin[j+1]&15)<<2)|(bin[j+2]>>6)];
		buf[i++]=base64_table[bin[j+2]&63];
		j+=3;
	}

	switch ( len - j ) {
        case 1:
		buf[i++] = base64_table[bin[j]>>2];
		buf[i++] = base64_table[(bin[j]&3)<<4];
		buf[i++] = BASE64_END;
		buf[i++] = BASE64_END;
		break;
        case 2:
		buf[i++] = base64_table[bin[j] >> 2];
		buf[i++] = base64_table[((bin[j] & 3) << 4) 
                                       | (bin[j + 1] >> 4)];
		buf[i++] = base64_table[(bin[j + 1] & 15) << 2];
		buf[i++] = BASE64_END;
		break;
        case 0:
	      break;
	}
	buf[i]='\0';
	return buf;
}



void
report(enum report_levels lev, char *format, ...)
{
        switch( lev ) {
        case DEBUG:
                fprintf(stderr, "debug: ");
                break;
        case WARN:
                fprintf(stderr, "warning: ");
                break;
        case ERR:
                fprintf(stderr, "error: ");
                break;
        default:
                break;
        }

        if( format ) {
                va_list args;
                va_start(args, format);
                vfprintf(stderr, format, args);
                va_end(args);
                fprintf(stderr, "\n");
        }
}

/* #define FAST_DNS 1 */

#if FAST_DNS
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern unsigned int resolve_dns(char *name);
#endif

int
tcp_connect(char *remote_host, int port) 
{
#if FAST_DNS
	struct in_addr ipaddr;
#else
        struct hostent *host;
#endif
        struct sockaddr_in sa;
        int sock_fd;

#if FAST_DNS
	ipaddr.s_addr = resolve_dns(remote_host);
	printf("\nhost: %s, inet: %s\n", remote_host, inet_ntoa(ipaddr));
#else
        if((host = (struct hostent *)gethostbyname(remote_host)) == NULL) {
                herror(remote_host);
                return 0;
        }
#endif
        /* get the socket */
        if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                perror("socket");
                return 0;
        }

        /* connect the socket, filling in the important stuff */
        sa.sin_family = AF_INET;
        sa.sin_port = htons(port);
#if FAST_DNS
	sa.sin_addr = ipaddr;
#else
        memcpy(&sa.sin_addr, host->h_addr,host->h_length);
#endif  
        if(connect(sock_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0){
                perror(remote_host);
                return 0;
        }

        return sock_fd;
}


#ifndef HAVE_STRDUP
char *
strdup(const char *s)
{
        char *new_string = NULL;

        if (s == NULL)
                return NULL;

        new_string = malloc(strlen(s) + 1);

        strcpy(new_string, s);

        return new_string;
}
#endif /* HAVE_STRDUP */

static void alarm_handler(int i)
{
	printf("\nIt is so long time!\n");
	exit(1);
}

/* Reboot the system after 'sec' s .... */
static void sys_exit(int sec)
{
	struct itimerval tv;
	
	tv.it_value.tv_sec = sec;
	tv.it_value.tv_usec = 0;
	tv.it_interval.tv_sec = 0;
	tv.it_interval.tv_usec = 0;
	
	setitimer(ITIMER_REAL, &tv, 0);

	signal(SIGALRM, alarm_handler);
}

int
transfer(UrlResource *rsrc)
{
        int i;

	if (rsrc->url->full_url && strstr(rsrc->url->full_url, "fileinfo")) {
		printf("\nDUT is snarfing 'fileinfo.txt'!\n");
		sys_exit(30);
	}

        switch (rsrc->url->service_type) {
        case SERVICE_HTTP:
                i = http_transfer(rsrc);
                break;
        case SERVICE_FTP:
                i = ftp_transfer(rsrc);
                break;
        case SERVICE_GOPHER:
                i = gopher_transfer(rsrc);
                break;
        default:
                report(ERR, "bad url: %s", rsrc->url->full_url);
        }

        return i;
}

#if FAST_DNS
/* resolv.c: DNS Resolver
 *
 * Copyright (C) 1998  Kenneth Albanowski <kjahds@kjahds.com>,
 *                     The Silver Hammer Group, Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
*/

/* 
  * Simplied by haiyue @ Delta Networks Inc., and used with dnsmasq 
  * for embedded system.  --- 2008-03-20
  */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PKTSIZE		512
#define MAXRETRIES 	3
#define DNS_HFIXEDSZ		12	/* bytes of fixed data in header */
#define DNS_RRFIXEDSZ	10	/* bytes of fixed data in r record */
#define DNS_PORT	53
#define DNS_IPADDR	0x7F000001	/* local nameserver: 127.0.0.1 */
#define DNS_TIMEOUT	3

#define NS_T_A	1	/* Host address. */
#define NS_C_IN	1	/* Internet. */

/* Structs */
struct resolv_header 
{
	int id;
	int qr,opcode,aa,tc,rd,ra,rcode;
	int qdcount;
	int ancount;
	int nscount;
	int arcount;
};

struct resolv_question 
{
	char *dotted;
	int qtype;
	int qclass;
};

struct resolv_answer 
{
	int atype;
	int aclass;
	int rdlength;
	unsigned char *rdata;
};

/* 
  * Encode a dotted string into nameserver transport-level encoding.
  * and the dest is BIG enough. 
  * g e m i n i . t u c . n o a o . e d u
  * [6] g e m i n i [3] t u c [4] n o a o [3] e d u[0]
  */
static int __encode_dotted(const char *dotted, unsigned char *dest)
{
	char c;
	int used, len;
	unsigned char *plen;

	len = 0, used = 1;
	plen = &dest[0];
	
	while ((c = *dotted++) != '\0') {
		if (c == '.') {
			*plen = (unsigned char)len;
			len = 0, plen = &dest[used++];
			continue;
		}

		dest[used++] = (unsigned char)c;
		len++;
	}

	*plen = (unsigned char)len;
	dest[used++] = 0;

	return used;
}

static int __length_dotted(unsigned char *data, int offset)
{
	int len, orig = offset;
	
	while ((len = data[offset++])) {
		if ((len & 0xC0) == 0xC0) { /* compress */
			offset++;
			break;
		}

		offset += len;
	}

	return offset - orig;
}

/* The dest is BIG enough */
int __encode_question(struct resolv_question *q, unsigned char *dest)
{
	int i;

	i = __encode_dotted(q->dotted, dest);
	
	dest += i;

	dest[0] = (q->qtype & 0xFF00) >> 8;
	dest[1] = (q->qtype & 0x00FF) >> 0;
	dest[2] = (q->qclass & 0xFF00) >> 8;
	dest[3] = (q->qclass & 0x00FF) >> 0;

	return i + 4;
}

static int __decode_answer(unsigned char *message, int offset,
			struct resolv_answer *a)
{
	int i;

	i = __length_dotted(message, offset);
	
	message += offset + i;

	a->atype = (message[0] << 8) |message[1]; 
	message += 2;
	a->aclass = (message[0] << 8) |message[1]; 
	message += 6; /* skip ttl */
	a->rdlength = (message[0] << 8) |message[1];
	message += 2;
	a->rdata = message;

	return i + DNS_RRFIXEDSZ + a->rdlength;
}

static int __length_question(unsigned char *message, int offset)
{
	int i;

	i = __length_dotted(message, offset);

	return i + 4;
}

static int __encode_header(struct resolv_header *h, unsigned char *dest)
{
	dest[0] = (h->id & 0xFF00) >> 8;
	dest[1] = (h->id & 0x00FF) >> 0;
	dest[2] = (h->qr ? 0x80 : 0) |
			((h->opcode & 0x0F) << 3) |
			(h->aa ? 0x04 : 0) |
			(h->tc ? 0x02 : 0) |
			(h->rd ? 0x01 : 0);
	dest[3] = (h->ra ? 0x80 : 0) | (h->rcode & 0x0F);
	dest[4] = (h->qdcount & 0xFF00) >> 8;
	dest[5] = (h->qdcount & 0x00FF) >> 0;
	dest[6] = (h->ancount & 0xFF00) >> 8;
	dest[7] = (h->ancount & 0x00FF) >> 0;
	dest[8] = (h->nscount & 0xFF00) >> 8;
	dest[9] = (h->nscount & 0x00FF) >> 0;
	dest[10] = (h->arcount & 0xFF00) >> 8;
	dest[11] = (h->arcount & 0x00FF) >> 0;

	return DNS_HFIXEDSZ;
}

static void __decode_header(struct resolv_header *h, unsigned char *data)
{
	h->id = (data[0] << 8) | data[1];
	h->qr = (data[2] & 0x80) ? 1 : 0;
	h->opcode = (data[2] >> 3) & 0x0F;
	h->aa = (data[2] & 0x04) ? 1 : 0;
	h->tc = (data[2] & 0x02) ? 1 : 0;
	h->rd = (data[2] & 0x01) ? 1 : 0;
	h->ra = (data[3] & 0x80) ? 1 : 0;
	h->rcode = data[3] & 0x0F;
	h->qdcount = (data[4] << 8) | data[5];
	h->ancount = (data[6] << 8) | data[7];
	h->nscount = (data[8] << 8) | data[9];
	h->arcount = (data[10] << 8) | data[11];
}

#define OPEN_UDP(fd) (fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))
#define SOCKTYPE(dest) ((struct sockaddr const *)(dest))

/* modified from uclib function '__dns_lookup', just inet type */
unsigned int dns_lookup(char *name)
{
	int retries = -1;
	int local_id = 1313;
	unsigned int ipaddr = 0;
	int i, j, len, fd, pos;
	fd_set readable;
	struct timeval timeo;
	struct sockaddr_in dest;
	struct resolv_header h;
	struct resolv_question q;
	struct resolv_answer ma;
	unsigned char packet[PKTSIZE];
	
	fd = -1;
	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(DNS_PORT);
	dest.sin_addr.s_addr = htonl(DNS_IPADDR); /* use '127.0.0.1' as nameserver, it's dnsmasq */
		
	while (++retries < MAXRETRIES) {
		if (fd < 0 && OPEN_UDP(fd) < 0)
			continue;

		memset(packet, 0, PKTSIZE);
		
		memset(&h, 0, sizeof(h));
		h.id = ++local_id;
		h.qdcount = 1;
		h.rd = 1;
		i = __encode_header(&h, packet);

		q.dotted = name;
		q.qtype = NS_T_A;
		q.qclass = NS_C_IN;
		j = __encode_question(&q, packet + i);
	
		len = i + j;
		if (sendto(fd, packet, len, 0, SOCKTYPE(&dest), sizeof(dest)) < 1)
			continue;

		FD_ZERO(&readable);
		FD_SET(fd, &readable);
		timeo.tv_sec = DNS_TIMEOUT;
		timeo.tv_usec = 0;
		if (select(fd + 1, &readable, NULL, NULL, &timeo) < 1)
			continue;			

		len = recvfrom(fd, packet, sizeof(packet), 0, NULL, 0);
		if (len < DNS_HFIXEDSZ)
			continue;

		__decode_header(&h, packet);
		if (h.id != local_id ||!h.qr ||h.rcode ||h.ancount < 1)
			continue;
		
		pos = DNS_HFIXEDSZ;
		for (j = 0; j < h.qdcount; j++)
			pos += __length_question(packet, pos);
		
		for (j = 0; j < h.ancount; j++, pos += i) {
			i = __decode_answer(packet, pos, &ma);
			if (ma.atype != NS_T_A ||ma.aclass != NS_C_IN)
				continue;

			unsigned char *p = ma.rdata;
			ipaddr = (p[0] << 24) |(p[1] << 16) |(p[2] << 8) |(p[3]);
			goto ret;
		}

	}

ret:
	if (fd >= 0)
		close(fd);

	return ipaddr;
}

unsigned int resolve_dns(char *name)
{
	unsigned int ipaddr;
	
	ipaddr = inet_addr(name);
	if (ipaddr == INADDR_NONE)
		ipaddr = dns_lookup(name);

	return ipaddr;
}
#endif
