# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5128 2006-10-15 21:03:30Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=iptables
PKG_VERSION:=1.3.5
PKG_RELEASE:=1


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR);
	ln -s $(PWD)/$(PKG_NAME)/src $(PKG_BUILD_DIR)
	cp -f $(PWD)/$(PKG_NAME)/src/extensions/mk/Makefile-$(BOARD) $(PWD)/$(PKG_NAME)/src/extensions/Makefile
endef

ifeq ($(DUMP),)
  -include $(LINUX_DIR)/.config
  include $(INCLUDE_DIR)/netfilter.mk
endif

define Package/iptables/Default
  SECTION:=net
  CATEGORY:=Base system
  URL:=http://netfilter.org/
endef

define Package/iptables
  $(call Package/iptables/Default)
  DEFAULT:=y
  TITLE:=IPv4 firewall administration tool
  MENU:=1
endef

define Build/Configure
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Build/Compile
	chmod a+x $(PKG_BUILD_DIR)/extensions/.*-test*
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		COPT_FLAGS="$(TARGET_CFLAGS)" \
		KERNEL_DIR="$(LINUX_DIR)" PREFIX=/usr \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install install-devel
endef

define Build/InstallDev
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		COPT_FLAGS="$(TARGET_CFLAGS)" \
		KERNEL_DIR="$(LINUX_DIR)" PREFIX=/usr \
		DESTDIR="$(STAGING_DIR)" \
		install install-devel
endef

define Build/UninstallDev
	rm -rf $(STAGING_DIR)/usr/include/libipq.h
	rm -rf $(STAGING_DIR)/usr/lib/libipq.a
endef

define Package/iptables/install
	#install -d -m0755 $(1)/etc/config
	#install -m0644 ./files/firewall.config $(1)/etc/config/firewall
	install -d -m0755 $(1)/etc/init.d
	install -m0755 ./files/firewall.init $(1)/etc/init.d/firewall
	#install -m0755 ./files/firewall.user $(1)/etc/
	#install -d -m0755 $(1)/usr/lib
	#install -m0644 ./files/firewall.awk $(1)/usr/lib
	install -d -m0755 $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/iptables $(1)/usr/sbin/
	install -d -m0755 $(1)/usr/lib/iptables
	#$(CP) $(PKG_INSTALL_DIR)/usr/lib/iptables/*.so $(1)/usr/lib/iptables/
	#(cd $(PKG_INSTALL_DIR)/usr/lib/iptables ; \
	#	$(CP) $(patsubst %,lib%.so,$(IPT_BUILTIN:xt_%=ipt_%)) $(1)/usr/lib/iptables/ \
	#)
endef

$(eval $(call BuildPackage,iptables))
