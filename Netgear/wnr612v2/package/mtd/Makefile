# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 4823 2006-09-23 13:45:49Z nico $

include $(TOPDIR)/rules.mk

PKG_NAME:=mtd
PKG_RELEASE:=4

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Package/mtd
  SECTION:=utils
  CATEGORY:=Base system
  DEFAULT:=y
  TITLE:=Update utility for trx firmware images
  DESCRIPTION:=\
	This package contains an utility useful to upgrade from other firmware or \\\
	older OpenWrt releases.
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)

endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
                $(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)"
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/mtd/install
	install -d -m0755 $(1)/sbin
	install -m0755 $(PKG_BUILD_DIR)/mtd $(1)/sbin/
	install -m0755 $(PKG_BUILD_DIR)/webupload $(1)/sbin/
	install -m0755 $(PKG_BUILD_DIR)/imgextract $(1)/sbin/
endef

$(eval $(call BuildPackage,mtd))
