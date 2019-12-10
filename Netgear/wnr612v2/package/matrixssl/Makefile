# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=matrixssl
PKG_VERSION:=3.2.1
PKG_RELEASE:=1

#PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=http://nbd.vd-s.ath.cx/openwrt
#PKG_MD5SUM:=771c1489488e62668d673478311d63ca

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Package/libmatrixssl
  SECTION:=libs
  SUBMENU:=SSL
  CATEGORY:=Libraries
  DEPENDS:=+libpthread
  TITLE:=Embedded SSL implementation
  URL:=http://www.matrixssl.org/
endef

define Build/Configure
endef

TARGET_CFLAGS += $(FPIC)

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		TARGET_CFLAGS="$(TARGET_CFLAGS) -Wall"
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Build/InstallDev
	#install -m0755 -d $(STAGING_DIR)/usr/include/matrixSsl
	#$(CP) $(PKG_BUILD_DIR)/matrixSsl.h $(STAGING_DIR)/usr/include/matrixSsl/
	#ln -sf matrixSsl/matrixSsl.h $(STAGING_DIR)/usr/include/matrixSsl.h
	#install -m0755 -d $(STAGING_DIR)/usr/lib
	#$(CP) $(PKG_BUILD_DIR)/libmatrixssl.so* $(STAGING_DIR)/usr/lib/
endef

define Package/libmatrixssl/install
	install -m0755 -d $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libmatrixssl.so $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libmatrixssl))
