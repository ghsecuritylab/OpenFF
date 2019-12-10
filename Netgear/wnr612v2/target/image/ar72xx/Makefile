# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/image.mk
include $(INCLUDE_DIR)/kernel-build.mk

define Build/Clean
endef

define Image/Prepare
endef

define Image/BuildKernel
	cp $(LINUX_DIR)/vmlinux $(BIN_DIR)/vmlinux
endef

define Image/Build
	$(call Image/Build/$(1),$(1))
endef


$(eval $(call BuildImage))
