# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.2 2006/12/06 00:48:31 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=utelnetd
PKG_VERSION:=0.1.9
PKG_RELEASE:=1


PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Package/utelnetd
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=utelnetd for CLI
  DESCRIPTION:=\
	Command Line Interface.
  URL:=http://utelnetd.sourceforge.net/
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/utelnetd/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/utelnetd $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,utelnetd))
