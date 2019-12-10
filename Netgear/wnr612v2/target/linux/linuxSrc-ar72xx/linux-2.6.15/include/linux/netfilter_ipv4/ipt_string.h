#ifndef _IPT_STRING_H
#define _IPT_STRING_H

#define IPT_STRING_HAYSTACK_THRESH	100
#define IPT_STRING_NEEDLE_THRESH	20

#define BM_MAX_NLEN 256
#define BM_MAX_HLEN 1024

#define BLOCK_SITE 1

typedef char *(*proc_ipt_search) (char *, char *, int, int);

struct ipt_string_info {
    char string[BM_MAX_NLEN];
    u_int16_t invert;
    u_int16_t allow_flag;
    u_int16_t len;
    u_int16_t is_get;
    u_int32_t TrustedIPaddr;
};

#endif /*_IPT_STRING_H*/
