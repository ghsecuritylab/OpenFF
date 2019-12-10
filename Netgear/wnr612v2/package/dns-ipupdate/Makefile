# 
#  Copyright (C) 2007-2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dns-ipupdate
PKG_RELEASE:=2
PKG_BUILD_DEPENDS:=datalib
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

PKG_GIT_TREEISH:=dns-ipupdate-v1.0.0-dni8

include $(INCLUDE_DIR)/package.mk

define Package/dns-ipupdate
	SECTION:=net
	CATEGORY:=Base system
	TITLE:=Dynamic DNS application.
	DESCRIPTION:=oray ddns.
endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

#define Build/Compile
#	$(MAKE) -C $(PKG_BUILD_DIR) \
#		TOPDIR=$(TOPDIR) \
#		all            
#endef

define Package/dns-ipupdate/install
	install -d -m0755 $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/updatednsip $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,dns-ipupdate))
