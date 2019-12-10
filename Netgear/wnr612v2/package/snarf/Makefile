include $(TOPDIR)/rules.mk

PKG_NAME:=snarf
PKG_VERSION:=7.0


PKG_CAT:=zcat

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_INSTALL_DIR=$(PKG_BUILD_DIR)/ipkg

include $(INCLUDE_DIR)/package.mk

define Package/snarf
  SECTION:=utils
  CATEGORY:=Utilities
  DEFAULT:=y
  TITLE:=snarf for embedded Linux
  DESCRIPTION:=\
	It is used for get image from Internet for upgrade.
endef
define Build/Prepare
	rmdir $(PKG_BUILD_DIR)
	ln -s ${PWD}/$(PKG_NAME)/src $(PKG_BUILD_DIR)
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS) -I." \
		STAGING_DIR="$(STAGING_DIR)"\
		all
endef

define Build/Clean
	rm -rf $(PKG_INSTALL_DIR)
	rm -rf $(PKG_BUILD_DIR)
endef

define Package/snarf/install
	install -m0755 -d $(1)/bin
	install -m0755 $(PKG_BUILD_DIR)/snarf $(1)/bin/
endef

$(eval $(call BuildPackage,snarf))
