PROJECT_NAME = glooper
export PROJECT_NAME

PROJECT_CONFIGURATION_TYPE = debug

PROJECT_INCLUDE_PATH := $(CURDIR)/include/c++

PROJECT_LIB_PATH := $(CURDIR)/lib/c++

PROJECT_CMD_PATH := $(CURDIR)/cmd/c++

BOOST_INCLUDE_PATH = /usr/local/include/boost-1_39

SEDNA_C_PATH = /simulation_data/sedna/driver/c

PROJECT_WRAP_PATH = $(CURDIR)/swig

export BOOST_INCLUDE_PATH

export SEDNA_C_PATH
