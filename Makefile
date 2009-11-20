-include project.config.mk

HEADER_FILES = $(shell find $(PROJECT_INCLUDE_PATH) -name "*.h" -o -name "*.hpp")

HEADER_STEMS = $(patsubst $(PROJECT_INCLUDE_PATH)/%,%,$(HEADER_FILES))

export PROJECT_INCLUDE_PATH
export PROJECT_LIB_PATH

ifeq "$(PROJECT_CONFIGURATION_TYPE)" "release" 
   INSTALL_PREFIX = /usr/local
else
   INSTALL_PREFIX := $(CURDIR)/install
   CONFIG_PREFIX := $(CURDIR)/install
   INSTALLED_LIB_PATH = $(INSTALL_PREFIX)/lib
   INSTALLED_HDR_PATH = $(INSTALL_PREFIX)/include/rng
endif

ifdef INSTALLED_LIB_PATH
   export INSTALLED_LIB_PATH
   RPATH_FLAG = -Wl,-rpath,$(INSTALLED_LIB_PATH)
   export RPATH_FLAG
endif

ifdef INSTALLED_HDR_PATH
   export INSTALLED_HDR_PATH
endif

#PROJECT_DEFINES = CONFIG_PREFIX

export PROJECT_DEFINES
export CONFIG_PREFIX
export PROJECT_CONFIGURATION_TYPE
export INSTALL_PREFIX

all: lib cmd

.PHONY: install
install:
	$(MAKE) --directory=$(PROJECT_LIB_PATH) install
	@for h in $(HEADER_STEMS); \
	   do \
	     install -Dv $(PROJECT_INCLUDE_PATH)/$$h $(INSTALL_PREFIX)/include/$(PROJECT_NAME)/$$h; \
	   done

test: install
	$(MAKE) --directory=$(TEST_DIR)

.PHONY: lib cmd
lib:
	$(MAKE) --directory=$(PROJECT_LIB_PATH)

cmd:
	$(MAKE) --directory=$(PROJECT_CMD_PATH)

clean:
	$(MAKE) --directory=$(PROJECT_LIB_PATH) clean
	$(MAKE) --directory=$(PROJECT_CMD_PATH) clean

vimclean:
	@find . -name "*~" -type f -exec rm -rfv '{}' \;
	@find . -name "*.swp" -type f -exec rm -rfv '{}' \;
