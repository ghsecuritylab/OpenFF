# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME=kernel-headers
PKG_VERSION=$(LINUX_VERSION)

PKG_SOURCE=$(LINUX_SOURCE)
PKG_SOURCE_URL=$(LINUX_SITE)

PKG_BUILD_DIR:=$(TOOLCHAIN_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

LINUX_DIR:=$(PKG_BUILD_DIR)
KERNEL_BUILD_DIR:=$(TOOLCHAIN_BUILD_DIR)
override QUILT:=

include $(INCLUDE_DIR)/host-build.mk

ifdef Build/Prepare/$(ARCH)
define Build/Prepare
	$(call Build/Prepare/$(ARCH))
endef
else
define Build/Prepare
	cd $(TOOLCHAIN_BUILD_DIR); tar -zxf $(TOPDIR)/dl/kernel-headers-2.6.15.tar.gz;
	$(SED) 's/@expr length/@-expr length/' $(PKG_BUILD_DIR)/Makefile
	ln -sf $(PKG_BUILD_DIR) $(LINUX_HEADERS_DIR)
endef
endif

define Build/Configure/powerpc
	$(CP) $(PKG_BUILD_DIR)/include/asm-ppc/* $(PKG_BUILD_DIR)/include/asm-powerpc/
	rm -rf $(PKG_BUILD_DIR)/include/asm-ppc
	mv $(PKG_BUILD_DIR)/include/asm-powerpc $(PKG_BUILD_DIR)/include/asm-ppc
endef

ifneq (,$(findstring uml,$(BOARD)))
  LINUX_KARCH:=$(ARCH)
endif

KMAKE := $(MAKE) -C $(PKG_BUILD_DIR) \
	ARCH=$(LINUX_KARCH) \
	CONFIG_SHELL=$(BASH)

define Build/Configure
	yes '' | $(KMAKE) oldconfig
	$(KMAKE) include/linux/version.h include/asm
	$(call Build/Configure/$(ARCH))
endef

define Build/Compile
endef

define Build/Install
endef

$(eval $(call HostBuild))
