# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 4841 2006-09-23 19:28:18Z nico $

include $(TOPDIR)/rules.mk

PKG_NAME:=smtpclient
PKG_RELEASE:=1
PKG_VERSION:=0
PKG_RELEASE:=0

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

define Package/$(PKG_NAME)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=DNI config utility
  DESCRIPTION:=\
	This package contains an smtp client to control DNI.
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -I." \
		all

endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

#define Build/InstallDev
#	mkdir -p $(STAGING_DIR)/usr/lib
#endef

#define Build/UninstallDev
#	rm -f $(STAGING_DIR)/usr/lib/libdniutil.so
#endef

define Package/$(PKG_NAME)/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/smtpclient $(1)/usr/sbin/

endef

$(eval $(call BuildPackage,$(PKG_NAME)))

