# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.2 2006/12/06 00:48:30 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=ez-ipupdate
PKG_VERSION:=3.0.10
PKG_RELEASE:=1


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR);
	ln -s $(PWD)/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/ez-ipupdate
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Dynamic DNS application
  DESCRIPTION:=\
	Command Line Interface.
  URL:=http://zebra.sourceforge.net/
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		STAGING_DIR="$(STAGING_DIR)"
endef

define Package/ez-ipupdate/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/ez-ipupdate $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,ez-ipupdate))
