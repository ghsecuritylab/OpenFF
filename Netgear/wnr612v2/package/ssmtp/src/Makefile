# Generated automatically from Makefile.in by configure.


srcdir= .
prefix=/usr
exec_prefix=/usr
libexecdir=/usr/lib
bindir=$(prefix)/sbin
mandir=$(prefix)/man/man8

LN_S=ln -s
CC=mips-linux-uclibc-gcc

etcdir=/etc
SSMTPCONFDIR=$(etcdir)/ssmtp
# (End of relocation section)

# Configuration files
CONFIGURATION_FILE=$(SSMTPCONFDIR)/ssmtp.conf
REVALIASES_FILE=$(SSMTPCONFDIR)/revaliases

INSTALLED_CONFIGURATION_FILE=$(CONFIGURATION_FILE)
INSTALLED_REVALIASES_FILE=$(REVALIASES_FILE)

# Programs
GEN_CONFIG=$(srcdir)/generate_config

SRCS=ssmtp.c arpadate.c base64.c xgethostname.c  md5auth/md5c.c md5auth/hmac_md5.c
#SRCS=ssmtp.c arpadate.c xgethostname.c  md5auth/md5c.c md5auth/hmac_md5.c

OBJS=$(SRCS:.c=.o)

INSTALL=/usr/bin/install -c

EXTRADEFS=\
-DSSMTPCONFDIR=\"$(SSMTPCONFDIR)\" \
-DCONFIGURATION_FILE=\"$(CONFIGURATION_FILE)\" \
-DREVALIASES_FILE=\"$(REVALIASES_FILE)\" \


CFLAGS= -DSTDC_HEADERS=1 -DHAVE_LIMITS_H=1 -DHAVE_STRINGS_H=1 -DHAVE_SYSLOG_H=1 -DHAVE_UNISTD_H=1 -DHAVE_LIBNSL=1 -DRETSIGTYPE=void -DHAVE_VPRINTF=1 -DHAVE_GETHOSTNAME=1 -DHAVE_SOCKET=1 -DHAVE_STRDUP=1 -DHAVE_STRSTR=1 -DREWRITE_DOMAIN=1 -DHAVE_SSL=1 -DMD5AUTH=1  $(EXTRADEFS) -Os -pipe -mips32 -mtune=mips32 -funit-at-a-time  

.PHONY: all
all: ssmtp

%.dvi: %.tex
	latex $<

.PHONY: install
install: ssmtp $(GEN_CONFIG)
	$(INSTALL) -d -m 755 $(bindir)
	$(INSTALL) -s -m 755 ssmtp $(bindir)/ssmtp
	$(INSTALL) -d -m 755 $(mandir)
	$(INSTALL) -m 644 $(srcdir)/ssmtp.8 $(mandir)/ssmtp.8
	$(INSTALL) -d -m 755 $(SSMTPCONFDIR)
	$(INSTALL) -m 644 $(srcdir)/revaliases $(INSTALLED_REVALIASES_FILE)
	$(GEN_CONFIG) $(INSTALLED_CONFIGURATION_FILE)


.PHONY: install-sendmail
install-sendmail: install
	$(RM) $(bindir)/sendmail
	$(LN_S) ssmtp $(bindir)/sendmail
	$(INSTALL) -d -m 755 $(libexecdir)
	$(RM) $(libexecdir)/sendmail
	$(LN_S) sendmail /lib/sendmail
	$(RM) $(mandir)/sendmail.8
	$(LN_S) ssmtp.8 $(mandir)/sendmail.8

.PHONY: uninstall
uninstall:
	$(RM) $(bindir)/ssmtp
	$(RM) $(mandir)/ssmtp.8
	$(RM) $(CONFIGURATION_FILE) $(REVALIASES_FILE)
	$(RM) -r $(SSMTPCONFDIR)

.PHONY: uninstall-sendmail
uninstall-sendmail: uninstall
	$(RM)  $(bindir)/sendmail /lib/sendmail
	$(RM)  $(mandir)/sendmail.8

# Binaries:
ssmtp: $(OBJS)
	#$(CC) -o ssmtp $(OBJS) -lnsl  $(CFLAGS) -L/home/timwu/JWNR2000/Jwnr2000-R_0409/package/matrixssl/src -lmatrixssl
	$(CC) -o ssmtp $(OBJS) $(LIBS) $(CFLAGS)

.PHONY: clean
clean:
	$(RM) ssmtp *.o md5auth/*.o core

.PHONY: distclean
distclean: clean docclean
	$(RM) config.* Makefile

.PHONY: docclean
docclean:
	$(RM) *.dvi *.log *.aux
