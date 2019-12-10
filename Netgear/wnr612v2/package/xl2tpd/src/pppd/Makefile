# BEGIN CONFIGURABLE SETTINGS

ifdef PPPD_SRCDIR
CFLAGS.pppd=	-I$(PPPD_SRCDIR)/include -I$(PPPD_SRCDIR)/pppd
endif
ifndef PPPD_LIBDIR
PPPD_LIBDIR=	$(LIBDIR)/pppd/$(PPPD_VERSION)
endif
#INCLUDE_DIR?=	/usr/include  #dvd.chen mark off


# END CONFIGURABLE SETTINGS

# __KERNGLUE__ hack needed because asm/param.h includes linux/config.h
# in some distributions
#CFLAGS	+= $(CFLAGS.optimize) -I$(L2TP_KERNELDIR)/include -I$(KERNEL_BLDDIR)/include \
#	-I$(KERNEL_SRCDIR)/include -I.. \
#	-I$(INCLUDE_DIR)/pppd \
#	$(CFLAGS.pppd) -D__KERNGLUE__ -fPIC
CFLAGS	+= $(CFLAGS.optimize) -I$(L2TP_KERNELDIR)/include -I$(KERNEL_BLDDIR)/include \
	-I$(KERNEL_SRCDIR)/include -I.. \
	$(CFLAGS.pppd) -D__KERNGLUE__ -fPIC $(EXTRA_CFLAGS)  #dvd.chen add EXTRA_CFLAGS
LDFLAGS	+= -shared

PLUGINS=	pppol2tp.so
ifeq ($(L2TP_FEATURE_RPC_MANAGEMENT),y)
PLUGINS+=	openl2tp.so
endif

all:	$(PLUGINS)

pppol2tp.so: pppol2tp.c
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) -D_linux_=1 $^

openl2tp.so: openl2tp.c ../l2tp_event_rpc_xdr.o ../l2tp_event_rpc_client.o
	$(CC) -o $@ $(LDFLAGS) $(CFLAGS) -D_linux_=1 $^

install: pppol2tp.so
	$(INSTALL) -d $(DESTDIR)$(PPPD_LIBDIR)
	$(INSTALL) pppol2tp.so $(DESTDIR)$(PPPD_LIBDIR)
ifeq ($(L2TP_FEATURE_RPC_MANAGEMENT),y)
	$(INSTALL) openl2tp.so $(DESTDIR)$(PPPD_LIBDIR)
endif

clean:
	-rm -rf *.o *.so
