# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/target.mk

all: install

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

TARGETS-y := linux utils
TARGETS-$(CONFIG_SDK) += sdk

# Modified by wayne
# linux-compile: utils-install
# linux-install: $(BIN_DIR)
linux-compile: utils-compile
linux-install: $(BIN_DIR) utils-install
# End of modification

image_install: linux-install

download: $(patsubst %,%-download,$(TARGETS-y))
prepare: linux-prepare
compile: linux-compile image_compile
install: image_clean $(patsubst %,%-install,$(TARGETS-y)) image_install
clean: $(patsubst %,%-clean,$(TARGETS-y)) image_clean

image_clean: FORCE
	$(MAKE) -C image/$(BOARD) clean
	rm -f $(BIN_DIR)/openwrt-*
	
image_compile: FORCE
	$(MAKE) -C image/$(BOARD) compile

image_install: image_compile
	$(MAKE) -C image/$(BOARD) install
image_rootfs: 
	$(MAKE) -C image/$(BOARD) install


%-clean: FORCE
	$(MAKE) -C $(patsubst %-clean,%,$@) clean
%-download: FORCE
	$(MAKE) -C $(patsubst %-download,%,$@) download
%-prepare: FORCE
	$(MAKE) -C $(patsubst %-prepare,%,$@) prepare
%-compile: %-prepare
	$(MAKE) -C $(patsubst %-compile,%,$@) compile
# Modified by wayne, too many compilations if install depends on compile 
#%-install: %-compile
%-install: 
# End of modification
	$(MAKE) -C $(patsubst %-install,%,$@) install

