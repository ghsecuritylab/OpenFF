# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
define kernel_template
ifeq ($(CONFIG_LINUX_$(3)),y)
KERNEL:=$(1)
BOARD:=$(2)
MODELNAME:=$(3)
else
# for clean to use
KERNEL:=2.6
BOARD:=ar72xx
MODELNAME:=2_6_AR72XX
endif
endef

$(eval $(call kernel_template,2.6,ar72xx,2_6_AR72XX))

export BOARD
export KERNEL
export MODELNAME

