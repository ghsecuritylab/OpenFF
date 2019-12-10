# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

KDIR:=$(BUILD_DIR)/linux-$(KERNEL)-$(BOARD)
MODULE_NAME:=$(MODELNAME)
FW_VERSION:=V1.1.2.6
VALID_REGION:=0

ifndef FIRMWARE_REGION
  FW_REGION:=""
  VALID_REGION:=1
else
  ifeq ($(FIRMWARE_REGION),WW)
    FW_REGION:=""
    VALID_REGION:=1
  endif
  ifeq ($(FIRMWARE_REGION),NA)
    FW_REGION:="NA"
    VALID_REGION:=1
  endif
  ifeq ($(FIRMWARE_REGION),GR)
    FW_REGION:="GR"
    VALID_REGION:=1
  endif
  ifeq ($(FIRMWARE_REGION),PR)
    FW_REGION:="PR"
    VALID_REGION:=1
  endif
  ifeq ($(FIRMWARE_REGION),RU)
    FW_REGION:="RU"
    VALID_REGION:=1
  endif
  ifeq ($(FIRMWARE_REGION),PT)
    FW_REGION:="PT"
    VALID_REGION:=1
  endif
  ifeq ($(FIRMWARE_REGION),KO)
    FW_REGION:="KO"
    VALID_REGION:=1
  endif
  ifeq ($(FIRMWARE_REGION),IN)
    FW_REGION:="IN"
    VALID_REGION:=1
  endif
endif

ifeq ($(VALID_REGION),0)
  FW_REGION:=""
  VALID_REGION:=1
endif

KERNEL_IMAGE_NAME=vmlinux.img
KIMAGE_PADDED=vmlinux.burn
RIMAGE_PADDED=root.bin
INFO_HEAD_LENGTH:=128
HEAD_LENGTH:=126
H_VERSION:=WNR2000
FLASH_BLOCK="\x12\x32"
KERNEL_LENGTH=786432
ROOTFS_LENGTH=2359296
#ROOTFS_LENGTH=2097152

#############################################
ifeq ("$(BOARD)", "ar72xx")
	MODULE_NAME=WNR612v2
	FW_VERSION=V1.0.0.3_1.0.2
	FW_FILE_NAME=$(shell echo $(MODULE_NAME) | tr [:upper:] [:lower:])
	H_VERSION=WNR612v2
#  KERNEL_LENGTH=720896
#  ROOTFS_LENGTH=1310720
endif
#############################################

KERNEL_ENTRY=`readelf -a $(KERNEL_BUILD_DIR)/vmlinux.$(BOARD)|grep "Entry"|cut -d":" -f 2`

ifneq ($(CONFIG_BIG_ENDIAN),y)
JFFS2OPTS     :=  --pad --little-endian --squash
SQUASHFS_OPTS :=  -le
else
JFFS2OPTS     :=  --pad --big-endian --squash
SQUASHFS_OPTS :=  -be
endif

define add_jffs2_mark
	echo -ne '\xde\xad\xc0\xde' >> $(1)
endef

define prepare_generic_squashfs
	dd if=$(1) of=$(KDIR)/tmpfile.1 bs=64k conv=sync
	$(call add_jffs2_mark,$(KDIR)/tmpfile.1)
	dd of=$(1) if=$(KDIR)/tmpfile.1 bs=64k conv=sync
	$(call add_jffs2_mark,$(1))
endef
    
define create_devfs_node
		sudo mknod $(BUILD_DIR)/root/dev/console c 5 1
		sudo mknod $(BUILD_DIR)/root/dev/null  c 1 3
		sudo mknod $(BUILD_DIR)/root/dev/zero  c 1 5
		sudo mknod $(BUILD_DIR)/root/dev/urandom  c 1 9
		sudo mknod $(BUILD_DIR)/root/dev/wd  c 10 130
		sudo mknod $(BUILD_DIR)/root/dev/ttyS0 c 4 64
		sudo mknod $(BUILD_DIR)/root/dev/ptyp0 c 2 0
		sudo mknod $(BUILD_DIR)/root/dev/ptyp1 c 2 1
		sudo mknod $(BUILD_DIR)/root/dev/ttyp0 c 3 0
		sudo mknod $(BUILD_DIR)/root/dev/ttyp1 c 3 1
#modified by dvd.chen, for DNI_MTD_PARTITION
		sudo mknod $(BUILD_DIR)/root/dev/mtdblock0 b 31 0
		sudo mknod $(BUILD_DIR)/root/dev/mtdblock1 b 31 1
		sudo mknod $(BUILD_DIR)/root/dev/mtdblock2 b 31 2
		sudo mknod $(BUILD_DIR)/root/dev/mtdblock3 b 31 3
		sudo mknod $(BUILD_DIR)/root/dev/mtdblock4 b 31 4
		sudo mknod $(BUILD_DIR)/root/dev/pot b 31 5
		sudo mknod $(BUILD_DIR)/root/dev/mtdblock6 b 31 6
		sudo mknod $(BUILD_DIR)/root/dev/mtdblock7 b 31 7
		sudo mknod $(BUILD_DIR)/root/dev/caldata b 31 7
		mkdir $(BUILD_DIR)/root/dev/mtd
		sudo mknod $(BUILD_DIR)/root/dev/mtd/0 c 90 0
		sudo mknod $(BUILD_DIR)/root/dev/mtd/0ro c 90 1
		sudo mknod $(BUILD_DIR)/root/dev/mtd/1 c 90 2
		sudo mknod $(BUILD_DIR)/root/dev/mtd/1ro c 90 3
		sudo mknod $(BUILD_DIR)/root/dev/mtd/2 c 90 4
		sudo mknod $(BUILD_DIR)/root/dev/mtd/2ro c 90 5
		sudo mknod $(BUILD_DIR)/root/dev/mtd/3 c 90 6
		sudo mknod $(BUILD_DIR)/root/dev/mtd/3ro c 90 7
		sudo mknod $(BUILD_DIR)/root/dev/mtd/4 c 90 8
		sudo mknod $(BUILD_DIR)/root/dev/mtd/4ro c 90 9
		sudo mknod $(BUILD_DIR)/root/dev/mtd/5 c 90 10
		sudo mknod $(BUILD_DIR)/root/dev/mtd/5ro c 90 11
		sudo mknod $(BUILD_DIR)/root/dev/mtd/6 c 90 12
		sudo mknod $(BUILD_DIR)/root/dev/mtd/6ro c 90 13
		sudo mknod $(BUILD_DIR)/root/dev/mtd/7 c 90 14
		sudo mknod $(BUILD_DIR)/root/dev/mtd/7ro c 90 15
		sudo mknod $(BUILD_DIR)/root/dev/ppp c 108 0
		sudo mknod $(BUILD_DIR)/root/dev/tty c 5 0
		#sudo chmod 666 $(BUILD_DIR)/root/dev/tty
		sudo mknod $(BUILD_DIR)/root/dev/ptmx c 5 2 
		#sudo chmod 666 $(BUILD_DIR)/root/dev/ptmx
		mkdir $(BUILD_DIR)/root/dev/pts
		sudo mknod $(BUILD_DIR)/root/dev/pts/0 c 136 0
		sudo mknod $(BUILD_DIR)/root/dev/pts/1 c 136 1
		sudo mknod $(BUILD_DIR)/root/dev/dk0 c 63 0
		sudo mknod $(BUILD_DIR)/root/dev/dk1 c 63 1
		sudo mknod $(BUILD_DIR)/root/dev/ar7240gpio c 240 0
		sudo mknod $(BUILD_DIR)/root/dev/ar7240gpiointr c 250 0
		ln -sf /var/etc/ppp/chap-secrets $(BUILD_DIR)/root/etc/ppp/chap-secrets
		ln -sf /var/etc/ppp/pap-secrets $(BUILD_DIR)/root/etc/ppp/pap-secrets
		ln -sf /var/etc/xl2tpd/options $(BUILD_DIR)/root/etc/xl2tpd/options
		ln -sf /var/etc/xl2tpd/xl2tpd.conf $(BUILD_DIR)/root/etc/xl2tpd/xl2tpd.conf
		cd $(BUILD_DIR)/root/dev; ln -sf /tmp/log/log log;
endef

ifneq ($(CONFIG_TARGET_ROOTFS_INITRAMFS),y)
    
  ifeq ($(CONFIG_TARGET_ROOTFS_SQUASHFS),y)
    define Image/mkfs/squashfs
		@mkdir -p $(BUILD_DIR)/root/jffs
		rm -fr $(BUILD_DIR)/root/jffs $(BUILD_DIR)/root/rom 
		ln -sf /var/resolv.conf $(BUILD_DIR)/root/etc/resolv.conf
		cd $(BUILD_DIR)/root/etc; rm -fr config hostapd.conf host profile protocols sysctl.conf banner
		rm -fr $(BUILD_DIR)/root/lib/config $(BUILD_DIR)/root/lib/network $(BUILD_DIR)/root/sbin/hotplug $(BUILD_DIR)/root/sbin/ifup $(BUILD_DIR)/root/sbin/ifdown $(BUILD_DIR)/root/usr/lib/common.awk $(BUILD_DIR)/root/ipkg $(BUILD_DIR)/root/usr/lib/ipkg
		echo "$(MODULE_NAME)" > $(BUILD_DIR)/root/module_name
		echo "$(FW_VERSION)" > $(BUILD_DIR)/root/firmware_version
		echo "$(FW_REGION)" > $(BUILD_DIR)/root/firmware_region
		echo "$(H_VERSION)" > $(BUILD_DIR)/root/hardware_version
		$(STAGING_DIR)/bin/mips-linux-uclibc-objcopy -O binary --remove-section=.reginfo --remove-section=.mdebug --remove-section=.comment --remove-section=.note --remove-section=.pdr --remove-section=.options --remove-section=.MIPS.options $(LINUX_DIR)/vmlinux $(BIN_DIR)/vmlinux.bin
		$(STAGING_DIR)/bin/lzma e $(BIN_DIR)/vmlinux.bin $(BIN_DIR)/vmlinux.bin.lzma
		$(STAGING_DIR)/bin/mkimage -A mips -O linux -T kernel -C lzma -n "Linux Kernel Image" -a 0x80002000 -e $(KERNEL_ENTRY) -d $(BIN_DIR)/vmlinux.bin.lzma $(BIN_DIR)/uImage
		-mkdir -p $(BUILD_DIR)/root/image/
		cp $(BIN_DIR)/uImage $(BUILD_DIR)/root/image/uImage

		$(call create_devfs_node)

		$(STAGING_DIR)/bin/mksquashfs-lzma $(BUILD_DIR)/root $(KDIR)/root.squashfs -nopad -noappend -root-owned $(SQUASHFS_OPTS)

		$(STAGING_DIR)/bin/mkimage -A mips -O linux -T filesystem -C none -a 0x9f050000 -e 0x9f050000 -name "$(MODULE_NAME)-$(FW_VERSION)"  -d $(KDIR)/root.squashfs $(BIN_DIR)/$(MODULE_NAME)-image.pad
		dd bs=$(HEAD_LENGTH) if=/dev/zero count=1 of=$(BIN_DIR)/head.pad
		echo "device:$(MODULE_NAME)" > $(BIN_DIR)/head_info.pad
		echo "version:$(FW_VERSION)" >> $(BIN_DIR)/head_info.pad
		echo "region:$(FW_REGION)" >> $(BIN_DIR)/head_info.pad
		cat $(BIN_DIR)/head_info.pad $(BIN_DIR)/head.pad | head -c $(HEAD_LENGTH) > $(BIN_DIR)/info.pad
		echo -n -e $(FLASH_BLOCK) >> $(BIN_DIR)/info.pad
		rm -rf $(BIN_DIR)/head_info.pad $(BIN_DIR)/head.pad
		cat $(BIN_DIR)/info.pad $(BIN_DIR)/$(MODULE_NAME)-image.pad > $(BIN_DIR)/$(MODULE_NAME)-no-crc.img
		$(STAGING_DIR)/../tools/appendsum $(BIN_DIR)/$(MODULE_NAME)-no-crc.img $(BIN_DIR)/root.burn
		#dd if=$(BIN_DIR)/root.burn of=$(BIN_DIR)/$(MODULE_NAME)-$(FW_VERSION)$(FW_REGION).img bs=64k conv=sync
		dd if=$(BIN_DIR)/root.burn of=$(BIN_DIR)/$(FW_FILE_NAME)-$(FW_VERSION)$(FW_REGION).img bs=64k conv=sync
		rm -rf $(BIN_DIR)/info.pad $(BIN_DIR)/pad.img $(BIN_DIR)/pad_t.img $(BIN_DIR)/root.burn $(BIN_DIR)/$(KIMAGE_PADDED) $(BIN_DIR)/$(MODULE_NAME)-no-crc.img $(BIN_DIR)/$(MODULE_NAME)-image.pad
    endef
  endif

  
endif


define Image/mkfs/prepare/default
	find $(BUILD_DIR)/root -type f -not -perm +0100 -not -name 'ssh_host*' | xargs chmod 0644
	find $(BUILD_DIR)/root -type f -perm +0100 | xargs chmod 0755
	find $(BUILD_DIR)/root -type d | xargs chmod 0755
	mkdir -p $(BUILD_DIR)/root/tmp
	chmod 0777 $(BUILD_DIR)/root/tmp
endef

define Image/mkfs/prepare
	$(call Image/mkfs/prepare/default)
endef

define BuildImage
compile:
	$(call Build/Compile)

install:
	$(call Image/Prepare)
	$(call Image/mkfs/prepare)
	$(call Image/BuildKernel)
#	$(call Image/mkfs/jffs2)
	$(call Image/mkfs/squashfs)
	$(call Image/mkfs/cramfs)
	$(call Image/mkfs/tgz)
	$(call Image/mkfs/ext2)
	
clean:
	$(call Build/Clean)
endef

compile-targets:
install-targets:
clean-targets:

download:
prepare:
compile: compile-targets
install: compile install-targets
clean: clean-targets
