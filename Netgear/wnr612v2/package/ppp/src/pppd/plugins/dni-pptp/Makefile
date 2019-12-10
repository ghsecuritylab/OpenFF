#***********************************************************************
# Makefile for Linux PPTP plugin.
#
# Copyright (C) 2007 Delta Network Inc.
#
#***********************************************************************

DESTDIR = /usr
BINDIR = $(DESTDIR)/sbin
LIBDIR = $(DESTDIR)/lib/pppd/$(PPPDVERSION)

PPPDVERSION = $(shell awk -F '"' '/VERSION/ { print $$2; }' ../../patchlevel.h)

INSTALL	= install

# Version is set ONLY IN THE MAKEFILE!  Don't delete this!
VERSION=3.3

COPTS=-O2 -g
CFLAGS=$(COPTS) -I../../../include/linux
all: dni-pptp.so

install: all
	$(INSTALL) -d -m 755 $(LIBDIR)
	$(INSTALL) -c -m 4550 dni-pptp.so $(LIBDIR)

clean:
	rm -f *.o

