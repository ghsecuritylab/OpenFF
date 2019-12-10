# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.3 2007/01/12 02:45:49 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=xl2tpd
PKG_VERSION:=1.2.4
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=$(DNI_DL_WEB) @SF/l2tpd
#PKG_MD5SUM:=ec962285b8044d809c1bff6dde13dc79
#PKG_CAT:=zcat


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg
L2TP_KERNELDIR:=$(BUILD_DIR)/pppol2tp
PKG_PPPD_VERSION:=2.4.3

include $(INCLUDE_DIR)/package.mk

define Package/xl2tpd
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+pppol2tp
  TITLE:=L2TP Client Daemon.
  DESCRIPTION:=\
	This package contains a L2TP (Layer 2 Tunneling Protocol) client.
	URL:=http://www.xelerance.com/software/xl2tpd/
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

#define Build/Configure
#endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

MAKE_FLAGS+= \
	KERNEL_SRCDIR="$(LINUX_DIR)" \
	KERNEL_BLDDIR="$(LINUX_DIR)" \
	PPPD_VERSION="$(PKG_PPPD_VERSION)" \
	PPPD_SRCDIR="$(BUILD_DIR)/ppp-$(PKG_PPPD_VERSION)" \
	PPPD_LIBDIR="$(STAGING_DIR)/usr/lib"

define Build/Compile
        $(MAKE) -C $(PKG_BUILD_DIR) \
                CC="$(TARGET_CC)" \
                CROSS="$(TARGET_CROSS)" \
                $(MAKE_FLAGS) \
		CFLAGS="$(TARGET_CFLAGS) -DTRUST_PPPD_TO_DIE -fno-builtin -Wall -DSANITY -DLINUX -DIP_ALLOCATION -DUSE_KERNEL -I$(L2TP_KERNELDIR)/include -I$(BUILD_DIR)/ppp-$(PKG_PPPD_VERSION)/pppd -I." \
                IPKG_ARCH="$(ARCH)" \
                all
endef

define Package/xl2tpd/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/xl2tpd $(1)/usr/sbin/
	install -d -m0755 $(1)/usr/lib/pppd/$(PKG_PPPD_VERSION)
	install $(PKG_BUILD_DIR)/pppd/pppol2tp.so $(1)/usr/lib/pppd/$(PKG_PPPD_VERSION)/
endef

$(eval $(call BuildPackage,xl2tpd))

