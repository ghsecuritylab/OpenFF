
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=pppol2tp
PKG_VERSION:=0.17
PKG_RELEASE:=

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/kernel.mk


KERNEL_CROSS:=mips-linux-

define KernelPackage/$(PKG_NAME)
  SUBMENU:=Network Support
  DEFAULT:=y
  TITLE:=pppol2tp kernel module for l2tp
  DESCRIPTION:=\
	Kernel modules for PPPoL2TP (PPP over L2TP) support
  URL:=http://www.openl2tp.org/
  VERSION:=$(LINUX_VERSION)+$(PKG_VERSION)-$(BOARD)-$(PKG_RELEASE)
  FILES:=$(PKG_BUILD_DIR)/pppol2tp.$(LINUX_KMOD_SUFFIX) 
endef

KERNEL_MAKEOPTS=  PATH="$(KERNEL_STAGING_DIR)/bin:$(TARGET_PATH)" \
		ARCH="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		TOOLPREFIX="$(KERNEL_CROSS)" \
		TOOLPATH="$(KERNEL_CROSS)" \
		KERNEL_SRCDIR="$(LINUX_DIR)"

define Build/Prepare
	-rm -rf $(PKG_BUILD_DIR)
	ln -s ${PWD}/pppol2tp/src $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(MAKE) -C $(LINUX_DIR) KERNELPATH="$(LINUX_DIR)" SUBDIRS=$(PKG_BUILD_DIR) MODVERDIR=$(PKG_BUILD_DIR)/modules modules $(KERNEL_MAKEOPTS)
endef

define Build/Clean
	$(MAKE) -C ${PWD}/pppol2tp/src clean
	-rm -f $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Build/UninstallDev
endef

define KernelPackage/$(PKG_NAME)/install
	$(STRIP) -S $(FILES)
	mkdir -p $(1)/lib/modules/$(LINUX_VERSION)
	cp -f $(FILES) $(1)/lib/modules/$(LINUX_VERSION)
endef

$(eval $(call KernelPackage,$(PKG_NAME)))
