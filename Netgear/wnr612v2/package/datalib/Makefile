#
# Copyright (C) 2007-2008 Delta Networks Inc.
#

include $(TOPDIR)/rules.mk

REGION_TAG=REGION_WW
ifeq ($(FIRMWARE_REGION),GR)
  REGION_TAG=REGION_GR
endif
ifeq ($(FIRMWARE_REGION),IN)
  REGION_TAG=REGION_IN
endif
ifeq ($(FIRMWARE_REGION),KO)
  REGION_TAG=REGION_KO
endif
ifeq ($(FIRMWARE_REGION),NA)
  REGION_TAG=REGION_NA
endif
ifeq ($(FIRMWARE_REGION),PR)
  REGION_TAG=REGION_PR
endif
ifeq ($(FIRMWARE_REGION),PT)
  REGION_TAG=REGION_PT
endif
ifeq ($(FIRMWARE_REGION),RU)
  REGION_TAG=REGION_RU
endif

PKG_NAME:=datalib
PKG_VERSION:=1.0
PKG_RELEASE:=1

#PKG_GIT_TREEISH:=a20723c6f5ac8988421ec8496ebadcc896f27018

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg

define Package/datalib
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=Small application for saving data on embedded system
	DESCRIPTION:=This package contains an utility for configuring embedded system.
endef

#define Build/Prepare
#	test x$(GIT_HOME) != x
#	test -d $(GIT_HOME)/datalib.git
#	$(CP) $(GIT_HOME)/datalib.git/* $(PKG_BUILD_DIR)
#	$(call Build/Patch/Default)
#	$(CP) ./files/defaults.c $(PKG_BUILD_DIR)/
#endef

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

# Modified by WAYNE on 2010/11/08
CFLAGS=$(TARGET_CFLAGS) -D$(REGION_TAG) -I$(STAGING_DIR)/usr/include \
		-I$(STAGING_DIR)/include
ifneq ($(VENDOR_MODEL_NAME),)
CFLAGS += -D$(VENDOR_MODEL_NAME) \
		-DVENDOR_MODEL_NAME=\""$(VENDOR_MODEL_NAME)\""
endif

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
                $(TARGET_CONFIGURE_OPTS) \
                CROSS="$(TARGET_CROSS)" \
		CC="$(TARGET_CROSS)gcc" \
                CFLAGS="$(CFLAGS)" \
                LDFLAGS="-L$(STAGING_DIR)/usr/lib -L$(STAGING_DIR)/lib" \
                all
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Build/InstallDev
	mkdir -p $(STAGING_DIR)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libconfig.so $(STAGING_DIR)/usr/lib/
endef

define Build/UninstallDev
	rm -f $(STAGING_DIR)/usr/lib/libconfig.so
endef

define Package/datalib/install
	install -d -m0755 $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libconfig.so $(1)/usr/lib/
	install -d -m0755 $(1)/bin
	install -m0755 $(PKG_BUILD_DIR)/datalib $(1)/bin/
	install -m0755 $(PKG_BUILD_DIR)/config $(1)/bin/
	install -d -m0755 $(1)/usr/sbin
	ln -sf /bin/config $(1)/usr/sbin/nvram
endef

$(eval $(call BuildPackage,datalib))
