# Generated automatically from Makefile.in by configure.
#***********************************************************************
#
# Makefile
#
# Makefile for Roaring Penguin's Linux PPPoE plugin.
#
# Copyright (C) 2001 Roaring Penguin Software Inc.
#
# This program may be distributed according to the terms of the GNU
# General Public License, version 2 or (at your option) any later version.
#
# $Id: Makefile,v 1.2 2005/12/02 09:11:18 tony Exp $
#***********************************************************************
#DESTDIR = $(TARGET_DIR)
#BINDIR = $(DESTDIR)/usr/local/sbin
#LIBDIR = $(DESTDIR)/usr/local/lib/pppd/$(PPPDVERSION)
#LIBDIR1 = $(DESTDIR)/usr/lib/pppd/$(PPPDVERSION)
BINDIR = $(DESTDIR)/sbin
LIBDIR = $(DESTDIR)/lib/pppd/$(PPPDVERSION)
PPPDVERSION = $(shell awk -F '"' '/VERSION/ { print $$2; }' ../../patchlevel.h)

INSTALL	= install

# Version is set ONLY IN THE MAKEFILE!  Don't delete this!
VERSION=3.7

#COPTS=-O2 -g -mips1 -msoft-float  -fPIC -I../../../../../include -nostdinc -I../../../../../lib/uClibc/include -I../../../../../lib/libcrypt -I../../../../../lib/zlib -I../../../../.. -idirafter /opt/toolchain_mips_nofpu/usr/bin/../lib/gcc-lib/mips-linux-uclibc/3.3.3//include -I../../../../../linux-2.4.x/include 
CFLAGS+= -I../../../include/linux
all: rp-pppoe.so 

pppoe-discovery: libplugin.a pppoe-discovery.o
	$(CC) -o pppoe-discovery pppoe-discovery.o libplugin.a

pppoe-discovery.o: pppoe-discovery.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe-discovery.o pppoe-discovery.c

rp-pppoe.so: libplugin.a plugin.o
	$(CC) -o rp-pppoe.so -shared plugin.o libplugin.a
	#$(LD) $(LDFLAGS) -o rp-pppoe.so -shared plugin.o libplugin.a


pppoe: pppoe.o if.o debug.o common.o ppp.o discovery.o
	$(CC) -o pppoe pppoe.o if.o debug.o common.o ppp.o discovery.o

install: all
	echo "aaron rp-pppoe 3.7 install "
	$(INSTALL) -d -m 755 $(LIBDIR)
	$(INSTALL) -c -m 4550 rp-pppoe.so $(LIBDIR)
clean:
	rm -f *.o *.so *.a pppoe

plugin.o: plugin.c
	$(CC) '-DRP_VERSION="$(VERSION)"' $(CFLAGS) -I../../..  -c -o plugin.o -fPIC plugin.c

libplugin.a: discovery.o if.o common.o debug.o
	$(AR) -rc $@ $^

discovery.o: discovery.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o discovery.o -fPIC discovery.c

pppoe.o: pppoe.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o pppoe.o -fPIC pppoe.c

if.o: if.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o if.o -fPIC if.c

debug.o: debug.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o debug.o -fPIC debug.c

common.o: common.c
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o common.o -fPIC common.c

ppp.o: ppp.c pppoe.h
	$(CC) $(CFLAGS) '-DVERSION="$(VERSION)"' -c -o ppp.o -fPIC ppp.c


