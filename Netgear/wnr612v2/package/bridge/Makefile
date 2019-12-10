# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 4855 2006-09-24 20:49:31Z nico $

include $(TOPDIR)/rules.mk

PKG_NAME:=bridge
PKG_VERSION:=1.0.6
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/bridge-utils-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR);
	ln -s $(PWD)/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Package/bridge
  SECTION:=net
  CATEGORY:=Base system
  DEFAULT:=y
  TITLE:=Ethernet bridging configuration utility
  DESCRIPTION:=\
	Manage ethernet bridging: a way to connect networks together to \\\
	form a larger network.
  URL:=http://bridge.sourceforge.net/
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		STAGING_DIR="$(STAGING_DIR)"\
		LINUX_SRC="$(BUILD_DIR)/linux"
		
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/bridge/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/brctl/brctl $(1)/usr/sbin/
	install -m0755 -d $(1)/usr/lib
	#install -m0644 $(PKG_BUILD_DIR)/libbridge/libbridge.a $(1)/usr/lib/
endef

$(eval $(call BuildPackage,bridge))
