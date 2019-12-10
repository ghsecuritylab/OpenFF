# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

LINUX_VERSION:=2.6.15
LINUX_RELEASE:=
LINUX_KERNEL_MD5SUM:=

include $(INCLUDE_DIR)/kernel-build.mk

MODULES_DIR=$(BUILD_DIR)/root/lib/modules/2.6.15

ifeq ($(CODEPUB),1)
install:
	cp $(BUILD_DIR)/linux/net/ipv4/netfilter/ip_nat_sip.ko $(MODULES_DIR)
	cp $(BUILD_DIR)/linux/net/ipv4/netfilter/ip_conntrack_sip.ko $(MODULES_DIR)
	cp $(BUILD_DIR)/linux/lib/crc-ccitt.ko $(MODULES_DIR)
	cp $(BUILD_DIR)/linux/drivers/net/ppp_async.ko $(MODULES_DIR)
	$(STRIP) -g $(MODULES_DIR)/ip_conntrack_sip.ko
	$(STRIP) -g $(MODULES_DIR)/ip_nat_sip.ko
	$(STRIP) -g $(MODULES_DIR)/crc-ccitt.ko
	$(STRIP) -g $(MODULES_DIR)/ppp_async.ko
else
install:
	cp $(BUILD_DIR)/linux/net/ipv4/netfilter/ip_nat_sip.ko $(MODULES_DIR)
	cp $(BUILD_DIR)/linux/net/ipv4/netfilter/ip_conntrack_sip.ko $(MODULES_DIR)
	cp $(BUILD_DIR)/linux/lib/crc-ccitt.ko $(MODULES_DIR) 
	cp $(BUILD_DIR)/linux/drivers/net/ppp_async.ko $(MODULES_DIR)
	$(STRIP) -g $(MODULES_DIR)/ip_conntrack_sip.ko
	$(STRIP) -g $(MODULES_DIR)/ip_nat_sip.ko
	$(STRIP) -g $(MODULES_DIR)/crc-ccitt.ko
	$(STRIP) -g $(MODULES_DIR)/ppp_async.ko
endif

$(eval $(call BuildKernel))

