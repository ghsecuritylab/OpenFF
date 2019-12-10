Summary: L2TP kernel module
Name: pppol2tp-kmod
Version: %%VER%%
Release: %%REL%%
License: GPL
Group: System Environment/daemons
URL: ftp://downloads.sourceforge.net/projects/openl2tp/%{name}-%{version}.tar.gz
Source0: %{name}-%{version}.tar.gz
Vendor: Katalix Systems Ltd
Packager: James Chapman <jchapman--at--katalix.com>
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}
Prereq: kernel%%SMP%% == %%KVER%%
ExclusiveOS: Linux

BuildPrereq: kernel%%SMP%%-devel == %%KVER%%

%description
This PPPoL2TP kernel driver implements a PPPoL2TP socket family. It is
used by L2TP daemons such as OpenL2TP and provides low-level L2TP
datapath functions. This driver requires PPPoX support in the kernel.

%prep
%setup

%build
make OPT_CFLAGS="$RPM_OPT_FLAGS" \
	PPPD_VERSION=%%PPPVER%% \
	KERNEL_SRCDIR=/lib/modules/%%KVERSMP%%/build \
	KERNEL_DESTDIR=${RPM_BUILD_ROOT}/lib/modules/%%KVERSMP%%/kernel/drivers/net

%install
make install DESTDIR=$RPM_BUILD_ROOT \
	PPPD_VERSION=%%PPPVER%% DEPMOD=true \
	KERNEL_SRCDIR=/lib/modules/%%KVERSMP%%/build \
	KERNEL_DESTDIR=${RPM_BUILD_ROOT}/lib/modules/%%KVERSMP%%/kernel/drivers/net

%clean
if [ "$RPM_BUILD_ROOT" != `echo $RPM_BUILD_ROOT | sed -e s/pppol2tp-kmod-//` ]; then
	rm -rf $RPM_BUILD_ROOT
fi

%files
%defattr(-,root,root,-)
/lib/modules/%%KVERSMP%%/kernel/drivers/net/pppol2tp.ko
%dir /usr/lib/pppol2tp-kmod
/usr/lib/pppol2tp-kmod/include/linux/if_ppp.h
/usr/lib/pppol2tp-kmod/include/linux/if_pppol2tp.h
/usr/lib/pppol2tp-kmod/include/linux/if_pppox.h
/usr/lib/pppol2tp-kmod/include/linux/socket.h

%changelog
* %%DATE%% %%AUTHOR%% - %%VER%%-%%KVERSMP%%
- Initial build
