# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 4914 2006-10-04 20:03:26Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
PKG_NAME:=library
PKG_RELEASE:=8

#PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

REV:=$(shell LANG=C svn info | awk '/^Revision:/ { print$$2 }' )
ifeq ($(REV),)
  REV:=0
endif

include $(INCLUDE_DIR)/package.mk

ifneq ($(DUMP),1)
  TARGET:=-$(BOARD)-$(KERNEL)
  UCLIBC_VERSION:=${shell cat $(STAGING_DIR)/uclibc_version 2>/dev/null}
  LIBGCC_VERSION:=${shell cat $(STAGING_DIR)/gcc_version 2>/dev/null}
else
  UCLIBC_VERSION:=<UCLIBC_VERSION>
  LIBGCC_VERSION:=<LIBGCC_VERSION>
endif

define Package/libgcc
  SECTION:=libs
  CATEGORY:=Base system
  DEFAULT:=y
  DEPENDS:=@!NATIVE_TOOLCHAIN
  TITLE:=GCC support library
  URL:=http://gcc.gnu.org/
  VERSION:=$(LIBGCC_VERSION)-$(PKG_RELEASE)
endef

define Package/libpthread
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=@!NATIVE_TOOLCHAIN
  TITLE:=POSIX thread library
  URL:=http://uclibc.org/
  VERSION:=$(UCLIBC_VERSION)-$(PKG_RELEASE)
endef

define Package/uclibc
  SECTION:=libs
  CATEGORY:=Base system
  DEFAULT:=y
  DEPENDS:=@!NATIVE_TOOLCHAIN
  TITLE:=C library embedded systems
  URL:=http://uclibc.org/
  UCLIBC_VERSION=0.9.28
  VERSION:=$(UCLIBC_VERSION)-$(PKG_RELEASE)
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
  $(call Build/Compile/$(BOARD))
endef

define Package/libgcc/install
	install -m0755 -d $(1)/lib
	$(CP) $(STAGING_DIR)/lib/libgcc_s.so.1 $(1)/lib/
endef

define Package/libpthread/install
	install -m0755 -d $(1)/lib
	$(CP) $(STAGING_DIR)/lib/libpthread.so.* $(1)/lib/
	$(CP) $(STAGING_DIR)/lib/libpthread-$(UCLIBC_VERSION).so $(1)/lib/
endef

define Package/uclibc/install
	install -m0755 -d $(1)/lib
	for file in ld-uClibc libdl libcrypt libnsl libresolv libm; do \
		$(CP) $(STAGING_DIR)/lib/$$$$file-$(UCLIBC_VERSION).so $(1)/lib/$$$$file.so.0; \
	done
	$(CP) $(STAGING_DIR)/lib/libuClibc-0.9.28.so $(1)/lib/libc.so.0
endef

$(eval $(call BuildPackage,libgcc))
$(eval $(call BuildPackage,libpthread))
$(eval $(call BuildPackage,uclibc))
