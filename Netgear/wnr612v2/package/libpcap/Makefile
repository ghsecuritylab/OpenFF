# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5232 2006-10-19 15:25:58Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=libpcap
PKG_VERSION:=0.9.4
PKG_RELEASE:=1


PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/libpcap
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Low-level packet capture library
  DESCRIPTION:=\
	This package contains a system-independent library for user-level \\\
	network packet capture.
  URL:=http://www.tcpdump.org/
endef


define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CCOPT="$(TARGET_CFLAGS) -I$(BUILD_DIR)/linux/include" \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/include
	$(CP)	$(PKG_INSTALL_DIR)/usr/include/pcap{,-bpf,-namedb}.h \
		$(STAGING_DIR)/usr/include/
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP)	$(PKG_INSTALL_DIR)/usr/lib/libpcap.{a,so*} \
		$(STAGING_DIR)/usr/lib/
endef

define Build/UninstallDev
	rm -rf	$(STAGING_DIR)/usr/include/pcap{,-bpf,-named}.h \
		$(STAGING_DIR)/usr/lib/libpcap.{a,so*}
endef

define Package/libpcap/install
	install -m0755 -d $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libpcap.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libpcap))
