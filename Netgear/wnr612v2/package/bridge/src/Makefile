
DESTDIR=
KERNEL_HEADERS=-I$(LINUX_SRC)

INSTALL=/usr/bin/install -c

prefix=/usr
exec_prefix=/usr
bindir=/usr/bin
sbindir=/usr/sbin
mandir=/usr/man
distdir = $(PACKAGE)-$(VERSION)

SUBDIRS=libbridge brctl doc

all:
	for x in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$x; done

clean:
	for x in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$x clean ; done

distclean:	clean
	rm -f config.log
	rm -f config.cache
	rm -f config.status

install:
	for x in $(SUBDIRS); do $(MAKE) $(MFLAGS) -C $$x install; done

