# Override one or more of the following to build RPM source and binary packages
ARCH=	i686
VER=	0.17
KVER=	2.6.17-1.2174_FC5
PPPVER=	2.4.3
TARGZ=	/tmp/pppol2tp-kmod-$(VER).tar.gz
SPEC=	pppol2tp-kmod.spec
AUTHOR=	$(shell whoami)
DATE=	$(shell date '+%a %b %d %Y')
REDHAT=	/usr/src/redhat
SMP=	

ifneq ($(SMP),)
SMPSTR1=	smp
SMPSTR=		-smp
endif

REL=	$(shell echo $(KVER)$(SMPSTR1) | tr - _)

.PHONY:	spec rpm

all:	spec rpm

spec:
	@if [ ! -e $(TARGZ) ]; then \
		echo "$(TARGZ) not found. Aborting." ;\
		echo "Use make TARGZ=<file> to point to the pppol2tp-kmod release tarball" ;\
	fi
	cp $(SPEC) pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%VER%%/${VER}/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%PPPVER%%/${PPPVER}/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%KVER%%/${KVER}/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%REL%%/$(REL)/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%AUTHOR%%/$(AUTHOR)/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%DATE%%/$(DATE)/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%SMP%%/$(SMPSTR)/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	cat pppol2tp-kmod-1.spec | sed -e 's/%%KVERSMP%%/$(KVER)$(SMPSTR1)/g' > pppol2tp-kmod-2.spec
	mv -f pppol2tp-kmod-2.spec pppol2tp-kmod-1.spec
	mv -f pppol2tp-kmod-1.spec pppol2tp-kmod-$(VER)-$(KVER)$(SMPSTR1).spec

rpm:
	cp pppol2tp-kmod-$(VER)-$(KVER)$(SMPSTR1).spec $(REDHAT)/SPECS
	cp $(TARGZ) $(REDHAT)/SOURCES
	cd $(REDHAT)/SPECS; rpmbuild -ba --target=$(ARCH)-linux pppol2tp-kmod-$(VER)-$(KVER)$(SMPSTR1).spec
	cp -v $(REDHAT)/RPMS/$(ARCH)/pppol2tp-kmod-$(VER)-$(REL).$(ARCH).rpm .
	cp -v $(REDHAT)/SRPMS/pppol2tp-kmod-$(VER)-$(REL).src.rpm .

