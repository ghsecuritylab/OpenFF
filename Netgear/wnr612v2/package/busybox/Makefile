# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5015 2006-10-10 15:45:36Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=busybox
PKG_VERSION:=1.0.1
PKG_RELEASE:=1


PKG_CAT:=bzcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Package/busybox
  SECTION:=base
  CATEGORY:=Base system
  DEFAULT:=y
  TITLE:=Core utilities for embedded Linux
  URL:=http://busybox.net/
  MENU:=1
endef

define Package/busybox/description
The Swiss Army Knife of embedded Linux.
	It slices, it dices, it makes Julian Fries.
endef

define Package/busybox/config
	menu "Configuration"
		depends on PACKAGE_busybox
		source "$(SOURCE)/config/Config.in"
	endmenu
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Build/Configure
	$(SCRIPT_DIR)/gen_busybox_config.pl $(TOPDIR)/.config > $(PKG_BUILD_DIR)/.config
	#cp -f ./Config $(PKG_BUILD_DIR)/.config
	cp -f ./default/Config.$(BOARD)-$(KERNEL) $(PKG_BUILD_DIR)/.config;
#	cp -f ./default/Config.$(BOARD) $(PKG_BUILD_DIR)/.config
	yes '' | $(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS="$(TARGET_CROSS)" \
		oldconfig
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS="$(TARGET_CROSS)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		IPKG_ARCH="$(ARCH)" \
		all
endef

define Package/busybox/install
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS="$(TARGET_CROSS)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		IPKG_ARCH="$(ARCH)" \
		PREFIX="$(1)" \
		install
	-rm -rf $(1)/lib64
endef

$(eval $(call BuildPackage,busybox))
