# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 4855 2006-09-24 20:49:31Z nico $

include $(TOPDIR)/rules.mk

PKG_NAME:=ebtables
PKG_VERSION:=2.0.8-2
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-v$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=$(DNI_DL_WEB) \
		@SF/ebtables
#PKG_MD5SUM:=216e5d20fbd0e56dbe7e56b0d07b1909
3PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/ebtables-v$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk


define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
	cp -f $(PWD)/$(PKG_NAME)/src/extensions/mk/Makefile-$(BOARD) $(PWD)/$(PKG_NAME)/src/extensions/Makefile
endef

define Package/ebtables
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Ethernet bridge firewall administration utility
  URL:=http://ebtables.sourceforge.net/
endef


define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -Dtarget_$(BOARD)=1 -Iinclude"
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/ebtables/install
	install -d -m0755 $(1)/etc
	install -m0644 $(PKG_BUILD_DIR)/ethertypes $(1)/etc/
	install -d -m0755 $(1)/usr/lib/ebtables
	install -m0755 $(PKG_BUILD_DIR)/lib*.so $(1)/usr/lib/
	install -m0755 $(PKG_BUILD_DIR)/extensions/*.so $(1)/usr/lib/
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/ebtables $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/ebtables-save $(1)/usr/sbin/
	install -m0755 $(PKG_BUILD_DIR)/ebtables-restore $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,ebtables))
