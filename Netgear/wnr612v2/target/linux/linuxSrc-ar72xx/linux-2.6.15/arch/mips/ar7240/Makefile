#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#
# Copyright © 2003 Atheros Communications, Inc.,  All Rights Reserved.
#

# Makefile for Atheros ar531x boards
#
# Note! Dependencies are done automagically by 'make dep', which also
# removes any old dependencies. DON'T put your own dependencies here
# unless it's something special (ie not a .c file).
#

#.S.s:
#	$(CPP) $(CFLAGS) $< -o $*.s
#.S.o:
#	$(CC) $(CFLAGS) -D__ASSEMBLY__ -c $< -o $*.o

obj-y	+=	prom.o	\
			ar7240_gpio.o	\
			ar9285_gpio.o	\
			intr.o	\
			irq.o	\
			setup.o	\
			ar7240.o \
			platform.o

ifdef CONFIG_AR9100
obj-$(CONFIG_AR9100)	+= wdt.o
else
obj-y	+= watchdog.o
endif

ifdef CONFIG_CUS100
CFLAGS_ar7240_gpio.o += -DCONFIG_CUS100=1
CFLAGS_ar9285_gpio.o += -DCONFIG_CUS100=1
endif

ifndef WPS_LED_GPIO_PIN
WPS_LED_GPIO_PIN=5
endif

CFLAGS_ar7240_gpio.o += -DWPS_LED_GPIO=$(WPS_LED_GPIO_PIN)
CFLAGS_ar9285_gpio.o += -DWPS_LED_GPIO=$(WPS_LED_GPIO_PIN)

#EXTRA_CFLAGS				+=	-fno-schedule-insns -fno-schedule-insns2
