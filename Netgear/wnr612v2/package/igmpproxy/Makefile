# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 4841 2006-09-23 19:28:18Z nico $

include $(TOPDIR)/rules.mk
include $(TOPDIR)/include/target.mk

PKG_NAME:=igmpproxy

PKG_VERSION:=0.1b2
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg
HWTBL=$(shell grep "CONFIG_ATHEROS_HEADER_EN=y" $(BUILD_DIR)/linux/.config)
ifeq ("$(HWTBL)", "CONFIG_ATHEROS_HEADER_EN=y")
EXTRA_FLAG = -DUSE_ATH_HEADER
endif

define Package/igmpproxy
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=DNI config utility
  DEPENDS:=
  DESCRIPTION:=\
	multicast proxy .
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CROSS="$(TARGET_CROSS)" \
		EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
		IPKG_ARCH="$(ARCH)" \
		EXTRA_FLAGS="$(EXTRA_FLAG)" \
		all
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/igmpproxy/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/igmpproxy $(1)/usr/sbin/
	#install -d -m0755 $(1)/etc/
	#install -m0666 files/$(BOARD)/igmpproxy.conf $(1)/etc/
endef

$(eval $(call BuildPackage,igmpproxy))

