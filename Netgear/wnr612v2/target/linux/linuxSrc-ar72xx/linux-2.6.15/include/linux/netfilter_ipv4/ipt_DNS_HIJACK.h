#ifndef _IPT_URL_FILTER_H
#define _IPT_URL_FILTER_H

#define URL_MAX_LEN 256

enum dnshj_state {
	BLANK_STATE,
	NORMAL_STATE
};

struct dnshdr {
    u_int16_t transid;
    u_int16_t flags;
    u_int16_t questions;
    u_int16_t answers;
    u_int16_t authority;
    u_int16_t additional;
};


/* target info */
struct ipt_dnshj_info {
        char url[URL_MAX_LEN];
        u_int8_t state;
};


#endif /* _IPT_URL_FILTER_H */
