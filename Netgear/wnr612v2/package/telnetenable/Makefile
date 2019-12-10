# 
# Copyright (C) 2008 OpenWrt.org
#

include $(TOPDIR)/rules.mk

PKG_NAME:=telnetenable
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Package/telnetenable
	SECTION:=utils
	CATEGORY:=Network
	TITLE:=unlocking the utelnetd.
	DESCRIPTION:=It is used for NTGR's Router Debug mode.
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/telnetenable/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/telnetenable $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,telnetenable))
