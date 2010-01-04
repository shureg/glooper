LIB_NAME_STEM ?= $(PROJECT_NAME)

LIB_SRC_SUFFIX ?= .cc

LIB_OBJ_SUFFIX ?= .o

LIB_SRC_DEP_SUFFIX ?= .d

LOCAL_LIB_DEPS = rng callback_log xml_serialisation

LIB_EXT_DEPS ?= $(LOCAL_LIB_DEPS) sedna

LIB_EXT_DEP_PATHS ?= $(SEDNA_C_PATH) $(LOCAL_LIB_DIRS)

BOOST_INCLUDE_PATH ?= /usr/local/include/boost-1_39

#LIB_INCLUDE_PATHS += $(BOOST_INCLUDE_PATH) $(SEDNA_C_PATH)

LIB_WARNINGS = all overloaded-virtual

LIB_VERSION_FILE = glooper.version

LIB_VERSION_HEADER_FILE = $(PROJECT_INCLUDE_PATH)/core/version.h
