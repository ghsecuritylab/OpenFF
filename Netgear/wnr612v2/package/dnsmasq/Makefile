# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5128 2006-10-15 21:03:30Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=dnsmasq
PKG_VERSION:=1.18.orig
PKG_RELEASE:=1


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg
DATALIB_DIR:=$(BUILD_DIR)/datalib

include $(INCLUDE_DIR)/package.mk

ifeq ($(CONFIG_PACKAGE_nvram-brcm),y)
  DEP_TARGET=nvram-brcm
else
  DEP_TARGET=datalib
endif


define Build/Prepare
	rm -rf $(PKG_BUILD_DIR);
	ln -s $(PWD)/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Package/dnsmasq
  SECTION:=net
  CATEGORY:=Base system
  DEFAULT:=y
ifneq ($(CODEPUB),1)
  DEPENDS:=$(DEP_TARGET)
endif
  TITLE:=A lightweight DNS and DHCP server
  DESCRIPTION:=\
	It is intended to provide coupled DNS and DHCP service to a LAN.
  URL:=http://www.thekelleys.org.uk/dnsmasq/
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -DHAVE_ISC_READER=1 -I$(DATALIB_DIR)" \
		LIBS="-L$(DATALIB_DIR) -lconfig" \
		BINDIR="/usr/sbin" MANDIR="/usr/man" \
		all
endef

define Package/dnsmasq/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/dnsmasq $(1)/usr/sbin/
	#install -m0755 -d $(1)/etc
	#install -m0644 ./files/dnsmasq.conf $(1)/etc/dnsmasq.conf
	#install -m0755 -d $(1)/etc/init.d
	#install -m0755 ./files/dnsmasq.init $(1)/etc/init.d/dnsmasq
endef

$(eval $(call BuildPackage,dnsmasq))
