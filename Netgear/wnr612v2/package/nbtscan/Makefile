# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.1 2007/03/22 03:13:13 anderson Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=nbtscan
PKG_RELEASE:=4

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/nbtscan
  SECTION:=net
  CATEGORY:=Network
  DEFAULT:=y
  TITLE:=Net scan
  DESCRIPTION:=Net scan
endef


define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)

endef
define Package/nbtscan/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/nbtscan $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,nbtscan))
