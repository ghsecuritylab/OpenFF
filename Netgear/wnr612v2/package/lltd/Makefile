# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile,v 1.1.2.3 2007/01/12 02:45:49 ronger Exp $

include $(TOPDIR)/rules.mk

PKG_NAME:=lltd
PKG_VERSION:=0.1
PKG_RELEASE:=1


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
#LLTDINC:=-include $(TOPDIR)/PROFILE/$(BOARD)/profile_info.h -include ../impl/$(BOARD)/product.h
LLTDINC:=-include $(TOPDIR)/PROFILE/$(BOARD)/profile_info.h
include $(INCLUDE_DIR)/package.mk

define Package/lltd
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=Link Layer Topology Discovery Daemon
  DESCRIPTION:=\
	Link Layer Topology Discovery Daemon.
  URL:=http://lltd.sourceforge.net/
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src/native-linux $(PKG_BUILD_DIR)
endef

#define Build/Configure
#endef
define Build/Compile
        $(MAKE) -C $(PKG_BUILD_DIR) \
                CC="$(TARGET_CC)" \
                CROSS="$(TARGET_CROSS)" \
		CFLAGS="$(LLTDINC)" \
								LDFLAGS="-L$(STAGING_DIR)/usr/lib -lconfig" \
                EXTRA_CFLAGS="$(TARGET_CFLAGS)" \
                IPKG_ARCH="$(ARCH)" \
                all
endef

define Package/lltd/install
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/lld2d $(1)/usr/sbin/
	install -m0755 -d $(1)/etc
	install -m0755 ./files/lld2d.conf  $(1)/etc/
	install -m0755 ./files/wrt54g.large.ico  $(1)/etc/
	install -m0755 ./files/wrt54g.small.ico $(1)/etc/
endef

$(eval $(call BuildPackage,lltd))
