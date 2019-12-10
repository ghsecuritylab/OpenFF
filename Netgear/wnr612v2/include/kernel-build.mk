# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
KERNEL_BUILD:=1

-include $(TOPDIR)/.config
include $(INCLUDE_DIR)/host.mk

ifneq ($(KERNEL_INC),1)
include $(INCLUDE_DIR)/kernel.mk
endif

LINUX_SOURCE:=linux-$(LINUX_VERSION).tar.bz2
LINUX_SITE:=$(DNI_DL_WEB) 
LINUX_KERNEL_MD5SUM=cdf95e00f5111e31f78e1d97304d9522

LINUX_CONFIG:=./config
LINUX_AUTOCONFIG:=./autoconf.h

KERNEL_CURR_DIR:=$(shell pwd)

ifeq ("$(BOARD)", "ar72xx")
  KERNEL_STAGING_DIR:=$(TOPDIR)/staging_dir_$(ARCH)/bin
endif

KERNEL_CROSS=$(ARCH)-linux-
KERNEL_CC=$(ARCH)-linux-gcc
export PATH:=$(KERNEL_STAGING_DIR):$(PATH);

ifneq (,$(findstring uml,$(BOARD)))
  LINUX_KARCH:=um
else
  LINUX_KARCH:=$(shell echo $(ARCH) | sed -e 's/i[3-9]86/i386/' \
	-e 's/mipsel/mips/' \
	-e 's/mipseb/mips/' \
	-e 's/powerpc/ppc/' \
	-e 's/sh[234]/sh/' \
	-e 's/armeb/arm/' \
  )
endif

KERNELNAME=
ifneq (,$(findstring x86,$(BOARD)))
  KERNELNAME="bzImage"
endif
ifneq (,$(findstring ppc,$(BOARD)))
  KERNELNAME="uImage"
endif


define Kernel/Prepare/Default
	#bzcat $(DL_DIR)/$(LINUX_SOURCE) | tar -C $(KERNEL_BUILD_DIR) $(TAR_OPTIONS)
	#[ -d ../generic-$(KERNEL)/patches ] && $(PATCH) $(LINUX_DIR) ../generic-$(KERNEL)/patches 
	#[ -d ./patches ] && $(PATCH) $(LINUX_DIR) ./patches
endef
define Kernel/Prepare
	#$(call Kernel/Prepare/Default)
	ln -s $(LINUX_SRCDIR)/linux-$(LINUX_VERSION)/ $(LINUX_DIR)
	@if [ ! -h "$(LINUX_SRCDIR)/linux-$(LINUX_VERSION)/Kconfig.DNI" ]; then \
		ln -s $(LINUX_SRCDIR)/../$(BOARD)-$(KERNEL)/Kconfig.DNI $(LINUX_SRCDIR)/linux-$(LINUX_VERSION)/Kconfig.DNI; \
	fi
endef

define Kernel/Configure/2.4
	$(MAKE) -C $(LINUX_DIR) CROSS_COMPILE="$(KERNEL_CROSS)" CC="$(KERNEL_CC)" ARCH=$(LINUX_KARCH) oldconfig
	$(MAKE) -C $(LINUX_DIR) CROSS_COMPILE="$(KERNEL_CROSS)" ARCH=$(LINUX_KARCH) dep
endef

define Kernel/Configure/2.6
	-$(MAKE) -C $(LINUX_DIR) CROSS_COMPILE="$(KERNEL_CROSS)" ARCH="$(LINUX_KARCH)" CONFIG_SHELL="$(BASH)" CC="$(KERNEL_CC)" oldconfig prepare scripts
endef
define Kernel/Configure/Default
	@$(CP) $(LINUX_CONFIG) $(LINUX_DIR)/.config
	$(call Kernel/Configure/$(KERNEL))
	rm -rf $(KERNEL_BUILD_DIR)/modules
	@rm -f $(BUILD_DIR)/linux
	ln -sf $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION) $(BUILD_DIR)/linux
endef
define Kernel/Configure
	$(call Kernel/Configure/Default)
endef


define Kernel/CompileModules/Default
	rm -f $(LINUX_DIR)/vmlinux $(LINUX_DIR)/System.map
	$(MAKE) -j$(CONFIG_JLEVEL) -C $(LINUX_DIR) CROSS_COMPILE="$(KERNEL_CROSS)" ARCH="$(LINUX_KARCH)" CONFIG_SHELL="$(BASH)" CC="$(KERNEL_CC)" modules
endef
define Kernel/CompileModules
	$(call Kernel/CompileModules/Default)
endef


ifeq ($(KERNEL),2.6)
  ifeq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),y)
    define Kernel/SetInitramfs
		mv $(LINUX_DIR)/.config $(LINUX_DIR)/.config.old
		grep -v INITRAMFS $(LINUX_DIR)/.config.old > $(LINUX_DIR)/.config
		echo 'CONFIG_INITRAMFS_SOURCE="../../root"' >> $(LINUX_DIR)/.config
		echo 'CONFIG_INITRAMFS_ROOT_UID=0' >> $(LINUX_DIR)/.config
		echo 'CONFIG_INITRAMFS_ROOT_GID=0' >> $(LINUX_DIR)/.config
		mkdir -p $(BUILD_DIR)/root/etc/init.d
		$(CP) ../generic-2.6/files/init $(BUILD_DIR)/root/
    endef
  else
    define Kernel/SetInitramfs
		mv $(LINUX_DIR)/.config $(LINUX_DIR)/.config.old
		grep -v INITRAMFS $(LINUX_DIR)/.config.old > $(LINUX_DIR)/.config
		rm -f $(BUILD_DIR)/root/init $(BUILD_DIR)/root/etc/init.d/S00initramfs
		echo 'CONFIG_INITRAMFS_SOURCE=""' >> $(LINUX_DIR)/.config
    endef
  endif
endif
define Kernel/CompileImage/Default
	$(call Kernel/SetInitramfs)
	$(MAKE) -j$(CONFIG_JLEVEL) -C $(LINUX_DIR) CROSS_COMPILE="$(KERNEL_CROSS)" CC="$(KERNEL_CC)" ARCH=$(LINUX_KARCH) $(KERNELNAME)
	$(KERNEL_CROSS)objcopy -O binary -R .reginfo -R .note -R .comment -R .mdebug -S $(LINUX_DIR)/vmlinux $(LINUX_KERNEL)
	$(KERNEL_CROSS)objcopy -R .reginfo -R .note -R .comment -R .mdebug -S $(LINUX_DIR)/vmlinux $(KERNEL_BUILD_DIR)/vmlinux.$(BOARD)
endef
define Kernel/CompileImage
	$(call Kernel/CompileImage/Default)
endef

define Kernel/Clean/Default
	rm -f $(LINUX_DIR)/.linux-compile
	rm -f $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION)/.configured
	rm -f $(LINUX_KERNEL)
	-$(MAKE) -C $(LINUX_SRCDIR)/linux-$(LINUX_VERSION) distclean
	rm -f $(LINUX_DIR)/.prepared $(LINUX_DIR)/Kconfig.DNI
endef

define Kernel/Clean
	$(call Kernel/Clean/Default)
endef

define BuildKernel
  ifneq ($(LINUX_SITE),)
    $(DL_DIR)/$(LINUX_SOURCE):
		#-mkdir -p $(DL_DIR)
		#$(SCRIPT_DIR)/download.pl $(DL_DIR) $(LINUX_SOURCE) $(LINUX_KERNEL_MD5SUM) $(LINUX_SITE)
  endif

#  $(LINUX_DIR)/.prepared: $(DL_DIR)/$(LINUX_SOURCE)
  $(LINUX_DIR)/.prepared: $(LINUX_SRCDIR)/linux-$(LINUX_VERSION)/Makefile
	-rm -rf $(KERNEL_BUILD_DIR)
	-mkdir -p $(KERNEL_BUILD_DIR)
	$(call Kernel/Prepare)
	touch $$@

  $(LINUX_DIR)/.configured: $(LINUX_DIR)/.prepared $(LINUX_CONFIG)
	$(call Kernel/Configure)
	touch $$@

#  $(LINUX_DIR)/.modules: $(LINUX_DIR)/.configured
$(LINUX_DIR)/.modules: $(LINUX_DIR)/.compiled
	rm -rf $(KERNEL_BUILD_DIR)/modules
	@rm -f $(BUILD_DIR)/linux
	ln -sf $(KERNEL_BUILD_DIR)/linux-$(LINUX_VERSION) $(BUILD_DIR)/linux
	$(call Kernel/CompileModules)
	touch $$@

  $(LINUX_DIR)/.image: $(LINUX_DIR)/.configured FORCE
	[ -e $(LINUX_DIR)/.modules ] || $(MAKE) compile
	$(call Kernel/CompileImage)
	touch $$@
	
  mostlyclean: FORCE
	$(call Kernel/Clean)

  define BuildKernel
  endef
endef


download: $(DL_DIR)/$(LINUX_SOURCE)
prepare: $(LINUX_DIR)/.configured $(TOPDIR)/.kernel.mk
compile: $(LINUX_DIR)/.modules
install: $(LINUX_DIR)/.image

clean: FORCE
	rm -f $(STAMP_DIR)/.linux-compile
	$(MAKE) mostlyclean
	rm -rf $(KERNEL_BUILD_DIR)

rebuild: FORCE
	@$(MAKE) mostlyclean
	@if [ -f $(LINUX_KERNEL) ]; then \
		$(MAKE) clean; \
	fi
	@$(MAKE) compile

$(TOPDIR)/.kernel.mk: Makefile
	echo "CONFIG_BOARD:=$(BOARD)" > $@
	echo "CONFIG_KERNEL:=$(KERNEL)" >> $@
	echo "CONFIG_LINUX_VERSION:=$(LINUX_VERSION)" >> $@
	echo "CONFIG_LINUX_RELEASE:=$(LINUX_RELEASE)" >> $@
	echo "CONFIG_LINUX_KARCH:=$(LINUX_KARCH)" >> $@


