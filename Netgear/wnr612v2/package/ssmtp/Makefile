#
# Copyright (C) 2007-2010 OpenWrt.org 
#
# This is free software, licensed under the GNU General Public License v2. 
# See /LICENSE for more information. 
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ssmtp
PKG_VERSION:=2.64
PKG_RELEASE:=2

#PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).orig.tar.bz2
#PKG_SOURCE_URL:=http://ftp.debian.org/debian/pool/main/s/ssmtp
#PKG_MD5SUM:=65b4e0df4934a6cd08c506cabcbe584f

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg
MATRIXSSL_DIR:=$(BUILD_DIR)/matrixssl
MATRIXSSL_APP_DIR=$(BUILD_DIR)/matrixssl/apps
MATRIXSSL_CORE_DIR=$(BUILD_DIR)/matrixssl/core
MATRIXSSL_CERT_DIR=$(BUILD_DIR)/matrixssl/sampleCerts
MATRIXSSL_INCLUDE_DIR=$(BUILD_DIR)/matrixssl/matrixssl



include $(INCLUDE_DIR)/package.mk 

TARGET_CFLAGS += $(TARGET_CPPFLAGS) 



define Package/$(PKG_NAME)
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=A minimal and secure mail sender
	DEPENDS:=+matrixssl
	URL:=http://packages.debian.org/ssmtp
endef

define Package/ssmtp/description
	A secure, effective and simple way of getting mail off a system to your
	mail hub. It contains no suid-binaries or other dangerous things - no
	mail spool to poke around in, and no daemons running in the background.
	mail is simply forwarded to the configured mailhost. Extremely easy
	configuration.
endef

#define Package/ssmtp/conffiles
#	/etc/ssmtp/ssmtp.conf
#	/etc/ssmtp/revaliases
#endef

CONFIGURE_VARS += \
        LIBS="-L$(MATRIXSSL_DIR) -lmatrixssl"
        
#CONFIGURE_VARS += \
#        LIBS="-Wl,-L$(OPENSSL_DIR),--start-group -lcrypto -lssl --end-group"

CONFIGURE_ARGS += \
        --enable-ssl
CONFIGURE_ARGS += \
        --enable-md5auth

define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		CPPFLAGS="-I. -I$(MATRIXSSL_INCLUDE_DIR)" \
		./configure $(CONFIGURE_ARGS) \
			--target=mips-linux \
			--host=mips-linux \
			--build=i386-redhat-linux \
			--program-prefix="" \
			--program-suffix="" \
			--prefix=/usr \
			--exec-prefix=/usr \
			--bindir=/usr/bin \
			--sbindir=/usr/sbin \
			--libexecdir=/usr/lib \
			--sysconfdir=/etc \
			--datadir=/usr/share \
			--localstatedir=/var \
			--mandir=/usr/man \
			--infodir=/usr/info \
			--disable-nls \
	)
endef

define Build/Compile
#	rm -rf $(PKG_INSTALL_DIR)
#	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CPPFLAGS="$(TARGET_CFLAGS) -I. -I$(MATRIXSSL_INCLUDE_DIR) -I$(MATRIXSSL_DIR) -I$(MATRIXSSL_APP_DIR) -I$(MATRIXSSL_CORE_DIR) -I$(MATRIXSSL_CERT_DIR) -L$(MATRIXSSL_DIR) -lmatrixssl" \
		$(CONFIGURE_VARS) \
		all
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/$(PKG_NAME)/install
	#install -m0755 -d $(1)/etc/ssmtp
	#install -m0755 $(PKG_BUILD_DIR)/ssmtp.conf $(1)/etc/ssmtp/
	#install -m0755 $(PKG_BUILD_DIR)/revaliases $(1)/etc/ssmtp/
	install -m0755 -d $(1)/usr/sbin
	install -m0755 $(PKG_BUILD_DIR)/ssmtp $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,$(PKG_NAME)))

