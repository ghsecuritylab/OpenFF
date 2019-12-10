
DESTDIR=
KERNEL_HEADERS=-I$(LINUX_SRC)

INSTALL=install -s

prefix=/usr
exec_prefix=/usr
bindir=/usr/bin
sbindir=/usr/sbin
mandir=/usr/man

SUBDIRS=libbridge brctl

all:

clean:

install:
	mkdir -p $(DESTDIR)$(mandir)/man8
	install -m 644 brctl.8 $(DESTDIR)$(mandir)/man8
