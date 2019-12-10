# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 4855 2006-09-24 20:49:31Z nico $

include $(TOPDIR)/rules.mk

PKG_NAME:=ap91-hostapd
PKG_VERSION:=atheros-0.5.9
PKG_RELEASE:=1
PKG_GIT_TREEISH:=hostapd-v7.3.0.387-dni6
PKG_GIT_DIR:=hostap-0.5.x.git

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg
LIBWPA_DIR:=$(PKG_BUILD_DIR)/common
WPAWALK_DIR:=$(PKG_BUILD_DIR)/wpatalk
HOSTAPD_DIR:=$(PKG_BUILD_DIR)/hostapd
WPASUP_DIR:=$(PKG_BUILD_DIR)/wpa_supplicant


#PKG_BUILD_DEPENDS:=ap91-madwifi-11n ap91-libwpa

include $(INCLUDE_DIR)/package.mk

define Package/ap91-hostapd/Default
  SECTION:=net
  CATEGORY:=Network
  
  TITLE:=IEEE 802.1x Authenticator
  URL:=http://hostap.epitest.fi/
endef

define Package/ap91-hostapd
  $(call Package/ap91-hostapd/Default)
  TITLE+= (full)
  DESCRIPTION:=\
	This package contains a full featured IEEE 802.1x/WPA/EAP/RADIUS \\\
	Authenticator.
endef

#define Package/ap91-hostapd/conffiles
#/etc/hostapd.conf
#endef

define Package/ap91-hostapd-mini
  $(call Package/ap91-hostapd/Default)
  TITLE+= (WPA-PSK only)
  
  DESCRIPTION:=\
	This package contains a minimal IEEE 802.1x/WPA/EAP/RADIUS Authenticator \\\
	(WPA-PSK only).
endef

define Package/ap91-hostapd-mini/conffiles
/etc/hostapd.conf
endef

define Package/ap91-hostapd-utils
  $(call Package/ap91-hostapd/Default)
  DEPENDS:=@PACKAGE_ap91-hostapd||PACKAGE_ap91-hostapd-mini
  TITLE+= (utils)
  DESCRIPTION:=\
	This package contains a command line utility to control the \\\
	IEEE 802.1x/WPA/EAP/RADIUS Authenticator.
endef

define Build/PrepareTarget
	#test x$(GIT_HOME) != x
	#test -d $(GIT_HOME)/$(PKG_GIT_DIR)
	#$(CP) $(GIT_HOME)/hostap-0.5.x.git/* $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
	-rm -rf $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/ConfigureTarget
	#if [ -d $(PKG_BUILD_DIR)_$(1) ]; then \
	#	rm -rf $(PKG_BUILD_DIR)_$(1); \
	#fi
	#$(CP) $(PKG_BUILD_DIR) $(PKG_BUILD_DIR)_$(1)
endef

HOSTAPD_CFLAGS="-DNETGEAR_RANDOM_SSID -DNETGEAR_ROUTER_PIN_HACK -DNETGEAR_ANYSSID -DNETGEAR_WPS_AUTO_LOCK_DOWN -DNETGEAR_SINGLE_RADIO_SSID_PREFIX -DNETGEAR_WPS_UPNP_SETTING -DNTG_DUT_NOT_SUPPORT_WEP_OPEN_AUTH -DNTGR_WPS_SET_MIX_MODE_IF_UNCFG -DNO_OPTIONAL_OPTION_IN_WPS_IE"

define Build/CompileTarget
	#build libwpa
	$(MAKE) -C $(PKG_BUILD_DIR)/common \
		$(TARGET_CONFIGURE_OPTS) \
		EXTRA_CFLAGS=$(HOSTAPD_CFLAGS) \
		OPTFLAGS="$(TARGET_CFLAGS)" \
		MADWIFIPATH="$(STAGING_DIR)/usr/include/madwifi" \
		CONFIG_ETHERSTA=y \
		all
		
	#build hostapd
	$(MAKE) -C $(PKG_BUILD_DIR)/hostapd \
		$(TARGET_CONFIGURE_OPTS) \
		EXTRA_CFLAGS=$(HOSTAPD_CFLAGS) \
		OPTFLAGS="$(TARGET_CFLAGS)" \
		CPPFLAGS="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include/madwifi" \
		LDFLAGS="-L$(LIBWPA_DIR)" \
		MADWIFIPATH="$(STAGING_DIR)/usr/include/madwifi" \
		CONFIG_ETHERSTA=y \
		INSTALL_ROOT="$(STAGING_DIR)" \
		LIB_COMMON_DIR="$(LIBWPA_DIR)" \
		hostapd
		
	#build wpatalk
	$(MAKE) -C $(PKG_BUILD_DIR)/wpatalk \
		$(TARGET_CONFIGURE_OPTS) \
		OPTFLAGS="$(TARGET_CFLAGS)" \
		CPPFLAGS="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include/madwifi" \
		LDFLAGS="-L$(LIBWPA_DIR)" \
		MADWIFIPATH="$(STAGING_DIR)/usr/include/madwifi" \
		CONFIG_ETHERSTA=y \
		INSTALL_ROOT="$(STAGING_DIR)" \
		LIB_COMMON_DIR="$(LIBWPA_DIR)" \
		all

	#build wpa_supplicant
	$(MAKE) -C $(PKG_BUILD_DIR)/wpa_supplicant \
		$(TARGET_CONFIGURE_OPTS) \
		OPTFLAGS="$(TARGET_CFLAGS)" \
		CPPFLAGS="-I$(STAGING_DIR)/usr/include -I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include/madwifi" \
		LDFLAGS="-L$(LIBWPA_DIR)" \
		MADWIFIPATH="$(STAGING_DIR)/usr/include/madwifi" \
		CONFIG_ETHERSTA=y \
		INSTALL_ROOT="$(STAGING_DIR)" \
		LIB_COMMON_DIR="$(LIBWPA_DIR)" \
		all		
endef

define Package/InstallTemplate
	#if [ \! -f "$(PKG_BUILD_DIR)_$(2)/hostapd" ]; then \
	#	rm -f $(PKG_BUILD_DIR)/.built; \
	#	$(MAKE) $(PKG_BUILD_DIR)/.built; \
	#fi
	
	#install libwpa
	mkdir -p $$(1)/lib
	$(CP) $(LIBWPA_DIR)/libwpa_common.so* $$(1)/lib/
	$(CP) $(LIBWPA_DIR)/libwpa_ctrl.so* $$(1)/lib/
	
	#install hostapd
	install -m0755 -d $$(1)/etc
	install -m0600 $(HOSTAPD_DIR)/madwifi.conf $$(1)/etc/hostapd.conf
	install -m0755 -d $$(1)/usr/sbin
	install -m0755 $(HOSTAPD_DIR)/hostapd $$(1)/usr/sbin/
	install -m0755 $(WPASUP_DIR)/wpa_supplicant $$(1)/usr/sbin/	
	#install -m0755 -d $$(1)/etc/wpa2
	#install -m0644 $(PKG_BUILD_DIR)_$(2)/../examples/*.conf $$(1)/etc/wpa2
	#install -m0644 $(PKG_BUILD_DIR)_$(2)/../examples/*.eap_user $$(1)/etc/wpa2
	#install -m0644 $(PKG_BUILD_DIR)_$(2)/../examples/*.ap_radio $$(1)/etc/wpa2
	#install -m0644 $(PKG_BUILD_DIR)_$(2)/../examples/*.ap_bss $$(1)/etc/wpa2
	#install -m0644 $(PKG_BUILD_DIR)_$(2)/../examples/*.sta $$(1)/etc/wpa2
	#mv $$(1)/etc/wpa2 $$(1)/etc/wpa2.orig
	#ln -sf /tmp/etc/wpa2 $$(1)/etc/wpa2
	
	#install wpatalk
	install -m0755 $(WPAWALK_DIR)/wpatalk $$(1)/usr/sbin/
endef

define Package/Template
 ifneq ($(CONFIG_PACKAGE_$(1)),)
  define Build/Prepare/$(2)
	$(call Build/PrepareTarget,$(2))
  endef
  define Build/Configure/$(2)
	$(call Build/ConfigureTarget,$(2))
  endef
  define Build/Compile/$(2)
	$(call Build/CompileTarget,$(2))
  endef
  define Package/$(1)/install
	$(call Package/InstallTemplate,$(1),$(2))
  endef
 endif
endef

define Build/Prepare
	$(call Build/Prepare/default)
	$(call Build/Prepare/min)
endef

define Build/Configure
	$(call Build/Configure/default)
	$(call Build/Configure/mini)
endef

define Build/Compile
	$(call Build/Compile/default)
	$(call Build/Compile/mini)
endef

define Build/Clean
	-rm -rf $(PKG_INSTALL_DIR)
	-rm -rf $(PKG_BUILD_DIR)
	#-rm -rf $(PKG_BUILD_DIR)_default
	#-rm -rf $(PKG_BUILD_DIR)_mini
	#-rm -rf $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
endef

define Package/ap91-hostapd-utils/install
	install -m0755 -d $(1)/usr/sbin
endef

$(eval $(call Package/Template,ap91-hostapd,default))
$(eval $(call Package/Template,ap91-hostapd-mini,mini))

$(eval $(call BuildPackage,ap91-hostapd))
$(eval $(call BuildPackage,ap91-hostapd-mini))
$(eval $(call BuildPackage,ap91-hostapd-utils))
