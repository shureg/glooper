LIB_OBJECTS = $(patsubst %$(LIB_SRC_SUFFIX), %$(LIB_OBJ_SUFFIX), $(shell find . -name "*$(LIB_SRC_SUFFIX)" ! -name "*swig_wrap$(LIB_SRC_SUFFIX)" -type f))

LIB_DEP_FILES = $(patsubst %$(LIB_OBJ_SUFFIX), %$(LIB_SRC_DEP_SUFFIX), $(LIB_OBJECTS))

LIB_DEP_FLAGS = $(foreach l,$(LIB_EXT_DEPS),-l$l)

LIB_DEP_PATH_FLAGS = $(foreach L,$(LIB_EXT_DEP_PATHS),-L$L)

ifndef PROJECT_INCLUDE_PATH
   $(error Value for PROJECT_INCLUDE_PATH must be set before libraries can be compiled)
endif

LIB_INCLUDE_PATHS += $(PROJECT_INCLUDE_PATH)

LIB_INCLUDE_FLAGS = $(foreach I,$(LIB_INCLUDE_PATHS),-I$I)

ifeq "$(PROJECT_CONFIGURATION_TYPE)" "debug_strict"
   LIB_WARNINGS += error
endif

LIB_WARNING_FLAGS = $(foreach W,$(LIB_WARNINGS),-W$W)

-include $(LIB_DEP_FILES)

%$(LIB_OBJ_SUFFIX): %$(LIB_SRC_SUFFIX)


%$(LIB_OBJ_SUFFIX): %$(LIB_SRC_SUFFIX)
	@$(if $(LIB_VERSION_FILE),/usr/bin/env python version_increment.py $(LIB_VERSION_FILE) $(LIB_VERSION_HEADER_FILE),)
	$(call make_depend,$<,$@,$*$(LIB_SRC_DEP_SUFFIX),$(LIB_INCLUDE_FLAGS))
	g++ -c -g -O2 -fPIC $(LIB_WARNING_FLAGS) $(LIB_INCLUDE_FLAGS) $(LIB_DEFINE_FLAGS) -o $@ $<

#A script to generate dependency files for c/c++ sources
#usage $(call make_depend,src_file,obj_file,dep_file,flags)
define make_depend
   g++ -MM -MF $3 -MP -MT $2 $4 $1
endef

