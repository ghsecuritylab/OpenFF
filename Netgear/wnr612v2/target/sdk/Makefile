# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 5081 2006-10-14 14:16:26Z nbd $

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/host.mk

PKG_OS:=$(shell uname -s)
PKG_CPU:=$(shell uname -m)

SDK_NAME:=OpenWrt-SDK-$(BOARD)-$(KERNEL)-for-$(PKG_OS)-$(PKG_CPU)
SDK_BUILD_DIR:=$(BUILD_DIR)/$(SDK_NAME)

all: compile

$(BIN_DIR)/$(SDK_NAME).tar.bz2:
	(cd $(STAGING_DIR); \
		rm -rf info man share stampfiles; \
		cd usr; \
		rm -rf doc info man share; \
	)
	rm -rf $(SDK_BUILD_DIR)
	mkdir -p $(SDK_BUILD_DIR)/dl $(SDK_BUILD_DIR)/package
	$(CP) $(INCLUDE_DIR) $(SCRIPT_DIR) $(TOPDIR)/docs $(SDK_BUILD_DIR)/
	$(TAR) -cf - -C $(TOPDIR) --exclude=ccache/ staging_dir_$(ARCH) | \
		$(TAR) -xf - -C $(SDK_BUILD_DIR)
	$(CP) $(TOPDIR)/rules.mk $(TOPDIR)/.config $(SDK_BUILD_DIR)/
	$(CP) $(TOPDIR)/package/Makefile $(SDK_BUILD_DIR)/package/
	$(CP) ./files/Makefile $(SDK_BUILD_DIR)/
	$(CP) ./files/README.SDK $(SDK_BUILD_DIR)/
	$(CP) ./files/package/rules.mk $(SDK_BUILD_DIR)/package/
	echo OPENWRTVERSION:=$(OPENWRTVERSION) > $(SDK_BUILD_DIR)/.version.mk
	rm -rf $(SDK_BUILD_DIR)/staging_dir*/ccache
	find $(SDK_BUILD_DIR) -name .svn | xargs rm -rf 
	find $(SDK_BUILD_DIR) -name CVS | xargs rm -rf 
	(cd $(BUILD_DIR); \
		tar cfj $@ $(SDK_NAME); \
	)

download:
prepare:
compile: $(BIN_DIR)/$(SDK_NAME).tar.bz2
install:

clean:
	rm -rf $(SDK_BUILD_DIR) $(BIN_DIR)/$(SDK_NAME).tar.bz2
