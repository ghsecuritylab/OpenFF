/*
 * Copyright (c) 2009, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 */

#ifndef _HW_DEF_H
#define _HW_DEF_H

/*
 * Atheros-specific
 */
typedef enum {
    ANTENNA_CONTROLLABLE,
    ANTENNA_FIXED_A,
    ANTENNA_FIXED_B,
    ANTENNA_DUMMY_MAX
} ANTENNA_CONTROL;

/* 
 * Number of (OEM-defined) functions using GPIO pins currently defined 
 *
 * Function 0: Link/Power LED
 * Function 1: Network/Activity LED
 * Function 2: Connection LED
 */
#ifndef NTGR_SPRT_TXRX_LED
#define NUM_GPIO_FUNCS             3
#else
/* The two extra LEDs is added for TX and RX */
#define NUM_GPIO_FUNCS             5
#endif

/*
** Default cache line size, in bytes.
** Used when PCI device not fully initialized by bootrom/BIOS
*/

#define DEFAULT_CACHELINE	32

#ifdef CONFIG_ARM

/*
** This was borrowed from NETBSD.  Not very atomic
*/

static INLINE int32_t cmpxchg(int32_t *_patomic_arg, int32_t _comparand, int32_t _exchange)
{
    if(*(_patomic_arg) == _comparand)
    {
         *(_patomic_arg) = _exchange;
         return _comparand;
    }
    return (*_patomic_arg);
}


#endif 

#endif
