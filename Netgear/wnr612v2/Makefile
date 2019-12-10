# Makefile for OpenWrt
#
# Copyright (C) 2006 OpenWrt.org
# Copyright (C) 2006 by Felix Fietkau <openwrt@nbd.name>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

RELEASE:=Kamikaze
#VERSION:=2.0 # uncomment for final release

#=========================================================================
FIRMWARE_REGION ?= WW
#=========================================================================

# CODE publish
export CODEPUB=$(if $(findstring ipk,$(wildcard $(TOPDIR)/prebuild/ipkg/*.ipk)),1,0)
# Added for ISP 
export FIRMWARE_ISP?=ALL

#--------------------------------------------------------------
# Just run 'make menuconfig', configure stuff, then run 'make'.
# You shouldn't need to mess with anything beyond this point...
#--------------------------------------------------------------

all: world

SHELL:=/usr/bin/env bash
export LC_ALL=C
export LANG=C
export TOPDIR=${shell pwd}
include $(TOPDIR)/include/verbose.mk
ifeq ($(KBUILD_VERBOSE),99)
  MAKE:=3>/dev/null $(MAKE)
endif

OPENWRTVERSION:=$(RELEASE)
ifneq ($(VERSION),)
  OPENWRTVERSION:=$(VERSION) ($(OPENWRTVERSION))
else
  REV:=$(shell LANG=C svn info | awk '/^Revision:/ { print$$2 }' )
  ifneq ($(REV),)
    OPENWRTVERSION:=$(OPENWRTVERSION)/r$(REV)
  endif
endif
export OPENWRTVERSION

ifneq ($(shell ./scripts/timestamp.pl -p .pkginfo package Makefile),.pkginfo)
  .pkginfo .config: FORCE
endif

ifeq ($(FORCE),)
  .config scripts/config/conf scripts/config/mconf: .prereq-build
  world: .prereq-packages
endif

.pkginfo:
	@echo Collecting package info...
	@-for dir in package/*/; do \
		echo Source-Makefile: $${dir}Makefile; \
		$(NO_TRACE_MAKE) --no-print-dir DUMP=1 -C $$dir 3>/dev/null || echo "ERROR: please fix $${dir}Makefile" >&2; \
	done > $@

pkginfo-clean: FORCE
	-rm -f .pkginfo .config.in

.config.in: .pkginfo
	./scripts/gen_menuconfig.pl < $< > $@ || rm -f $@

.config: ./scripts/config/conf .config.in
#	[ -f .config ] || $(NO_TRACE_MAKE) menuconfig
	if [ ! -f .config ] ; then \
		if [ -f $(TOPDIR)/defconfig/defconfig-ar72xx-2.6 ] ; then \
			cp -fp $(TOPDIR)/defconfig/defconfig-ar72xx-2.6 $(TOPDIR)/.config ; \
		else \
			$(NO_TRACE_MAKE) menuconfig ; \
		fi \
	fi
	$< -D .config Config.in &> /dev/null

scripts/config/mconf:
	@$(MAKE) -C scripts/config all

scripts/config/conf:
	@$(MAKE) -C scripts/config conf

config: scripts/config/conf .config.in FORCE
	$< Config.in

config-clean: FORCE
	$(NO_TRACE_MAKE) -C scripts/config clean

defconfig: scripts/config/conf .config.in FORCE
	touch .config
	$< -D .config Config.in

oldconfig: scripts/config/conf .config.in FORCE
	$< -o Config.in

menuconfig: scripts/config/mconf .config.in FORCE
	$< Config.in

package/%: .pkginfo FORCE
	$(MAKE) -C package $(patsubst package/%,%,$@)

target/%: .pkginfo FORCE
	$(MAKE) -C target $(patsubst target/%,%,$@)

tools/%: FORCE
	$(MAKE) -C tools $(patsubst tools/%,%,$@)

toolchain/%: FORCE
	$(MAKE) -C toolchain $(patsubst toolchain/%,%,$@)

.prereq-build: include/prereq-build.mk
	@$(NO_TRACE_MAKE) -s -f $(TOPDIR)/include/prereq-build.mk prereq 2>/dev/null || { \
		echo "Prerequisite check failed. Use FORCE=1 to override."; \
		rm -rf $(TOPDIR)/tmp; \
		false; \
	}
	@rm -rf $(TOPDIR)/tmp
	@touch $@

.prereq-packages: include/prereq.mk .pkginfo .config
	@$(NO_TRACE_MAKE) -s -C package prereq 2>/dev/null || { \
		echo "Prerequisite check failed. Use FORCE=1 to override."; \
		false; \
	}
	@rm -rf "$(TOPDIR)/tmp"
	@touch $@
	
prereq: .prereq-build .prereq-packages FORCE

download: .config FORCE
	$(MAKE) tools/download
	$(MAKE) toolchain/download
	$(MAKE) package/download
	$(MAKE) target/download

PUBLIC_SHARE_STAGING_DIR:= \
	/public_share/wnr612-toolchain/staging_dir_mips
MY_STAGING_DIR:=$(TOPDIR)/staging_dir_mips

ifeq ($(CODEPUB),1)
	PREPKGS:=$(wildcard $(TOPDIR)/prebuild/ipkg/*.ipk)
endif

ifeq ($(CODEPUB),1)
world: .config FORCE
	date +%s >StartTime
	$(MAKE) tools/install
	$(MAKE) toolchain/install
	$(MAKE) uboot
	$(MAKE) target/compile
	$(MAKE) madwifi-install
	$(MAKE) package/compile
	$(MAKE) package/install
	$(MAKE) pre-install
	$(MAKE) target/install
	$(MAKE) package/index
	echo -e "\nTotal build time is about $$(( `date +%s` - `cat StartTime` )) seconds!"
	rm StartTime
else
world: $(MY_STAGING_DIR)

world: .config FORCE
	date +%s >StartTime
	$(MAKE) tools/install
	if [ ! -d $(PUBLIC_SHARE_STAGING_DIR) ] ; then $(MAKE) toolchain/install ; fi
	$(MAKE) target/compile
	$(MAKE) package/compile
	$(MAKE) package/install
	$(MAKE) target/install
	$(MAKE) package/index
	echo -e "\nTotal build time is about $$(( `date +%s` - `cat StartTime` )) seconds!"
	rm StartTime
endif

# Added by wayne to escape toolchain/install
GCC-BIN:= ar gcc ld nm objcopy objdump strip 

$(MY_STAGING_DIR):
#	if [ ! -d $(PUBLIC_SHARE_STAGING_DIR) ] ; then \
#		echo "Please install toolchain first!" ; \
#		exit 1; \
#	fi
	@if [ -d $(PUBLIC_SHARE_STAGING_DIR) -a ! -e "$(MY_STAGING_DIR)/.built" ] ; then \
		mkdir -p $(MY_STAGING_DIR)/bin ; \
		ln -sf $(PUBLIC_SHARE_STAGING_DIR)/bin/mips-linux* $(MY_STAGING_DIR)/bin ; \
		cd $(MY_STAGING_DIR)/bin ; \
		for gcc in $(GCC-BIN) ; do \
			ln -sf mips-linux-$${gcc} mips-linux-uclibc-$${gcc} ; \
		done ; \
		cd .. ; \
		rm -f include ; ln -sf $(PUBLIC_SHARE_STAGING_DIR)/include include ; \
		rm -f lib ; mkdir -p $(MY_STAGING_DIR)/lib ; \
		ln -sf $(PUBLIC_SHARE_STAGING_DIR)/lib/* $(MY_STAGING_DIR)/lib; \
		cp -P --remove-destination $(PUBLIC_SHARE_STAGING_DIR)/lib/*.so* $(MY_STAGING_DIR)/lib; \
		rm -f mips-linux-uclibc ; ln -sf $(PUBLIC_SHARE_STAGING_DIR)/mips-linux-uclibc \
			mips-linux-uclibc ; \
		rm -f mips-linux ; ln -sf mips-linux-uclibc mips-linux ; \
		ln -sf $(PUBLIC_SHARE_STAGING_DIR)/gcc_version gcc_version ; \
		ln -sf $(PUBLIC_SHARE_STAGING_DIR)/uclibc_version uclibc_version ; \
		touch .built; \
	fi

clean: FORCE
	-$(MAKE) target/clean
	-$(MAKE) package/clean
	-rm -rf build_* bin
	-$(MAKE) uboot-clean

dirclean: clean
	-rm -rf staging_dir_* toolchain_build_* tool_build

distclean: dirclean config-clean
	-rm -rf .*config* .pkg* .prereq* .host.mk .kernel.mk
	for i in .built .prepared .configured; do find -name $$i -exec rm -f {} \;; done;
	find -name .version-*_mips -exec rm -f {} \;
	-find package/* -name ipkg -exec rm -rf {} \; 2>/dev/null
	-$(MAKE) uboot-distclean

ifeq ($(CODEPUB),1)
BUILD_MIPS:=$(TOPDIR)/build_mips
ROOTFS:=$(BUILD_MIPS)/root
FIRMWARE_REGION?=WW

madwifi-install: FORCE
	@IPKG_TMP=$(BUILD_MIPS)/tmp \
	IPKG_INSTROOT=$(ROOTFS) \
	IPKG_CONF_DIR=$(TOPDIR)/staging_dir_mips/etc \
	IPKG_OFFLINE_ROOT=$(ROOTFS) \
	$(TOPDIR)/scripts/ipkg -force-defaults -force-depends install \
		$(TOPDIR)/prebuild/ipkg/kmod-ap91-madwifi-11n*.ipk
	@cp -rfp $(TOPDIR)/prebuild/include $(TOPDIR)/staging_dir_mips/usr/


pre-install: FORCE
	@for ipkg in $(PREPKGS); do \
		IPKG_TMP=$(BUILD_MIPS)/tmp \
		IPKG_INSTROOT=$(ROOTFS) \
		IPKG_CONF_DIR=$(TOPDIR)/staging_dir_mips/etc \
		IPKG_OFFLINE_ROOT=$(ROOTFS) \
		$(TOPDIR)/scripts/ipkg -force-defaults -force-depends install $${ipkg} ; \
	done
endif

# uboot 
UBOOTDIR:=$(TOPDIR)/bootloader

uboot:
	echo "Making $@..."
	PATH=$(MY_STAGING_DIR)/bin:$(PATH);$(MAKE) -C $(UBOOTDIR)
	[ -d $(TOPDIR)/bin ] || mkdir $(TOPDIR)/bin
	cp $(UBOOTDIR)/u-boot.bin $(TOPDIR)/bin

uboot-clean:
	echo "Making $@..."
	PATH=$(MY_STAGING_DIR)/bin:$(PATH); $(MAKE) -C $(UBOOTDIR) clean

uboot-distclean:
	echo "Making $@..."
	PATH=$(MY_STAGING_DIR)/bin:$(PATH); $(MAKE) -C $(UBOOTDIR) distclean

.SILENT: clean dirclean distclean config-clean download world
FORCE: ;
.PHONY: FORCE

# Added by wayne to escape toolchain/install
.PHONY: $(MY_STAGING_DIR) uboot uboot-clean uboot-distclean
%: ;
