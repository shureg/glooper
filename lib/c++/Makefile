#defining 'escaped' space and comma, so that $( ) evaluates to a space and $(,) evaluates to a comma
, := ,
space :=
space +=
$(space) :=
$(space) +=

-include lib.config.mk

DY_LIB_NAME = lib$(LIB_NAME_STEM).so
ST_LIB_NAME = lib$(LIB_NAME_STEM).a

LIB_DEFINES += $(PROJECT_DEFINES)

LIB_VERSION = $(if $(LIB_VERSION_FILE),`cat $(LIB_VERSION_FILE)`,)
#LIB_VERSION_NUMBERS = $(subst .,$( ),$(LIB_VERSION))

#DY_LIB_SO_SUFFIX = $(if $(LIB_VERSION),.$(word 1,$(LIB_VERSION_NUMBERS)),)
#DY_LIB_FULL_SUFFIX = $(if $(LIB_VERSION),.$(LIB_VERSION),)
#DY_LIB_SO_NAME = lib$(LIB_NAME_STEM).so$(DY_LIB_SO_SUFFIX)
#DY_LIB_FULL_NAME = $(DY_LIB_NAME)$(DY_LIB_FULL_SUFFIX)

LIB_DEFINE_FLAGS = $(foreach D,$(LIB_DEFINES),-D$D=$($D))

ifeq "$(MAKECMDGOALS)" "install"
   ifndef INSTALL_PREFIX
      $(error INSTALL_PREFIX value must be set for installation to proceed)
   endif
endif

.PHONY: lib wrap

lib: $(DY_LIB_NAME) $(ST_LIB_NAME)

wrap:
	$(MAKE) --directory=$(PROJECT_WRAP_PATH) $@

include lib.compile.mk

$(DY_LIB_NAME): $(LIB_OBJECTS)
	g++ -shared -pg -Wl,-soname=$(DY_LIB_NAME) -o $(DY_LIB_NAME) $(LIB_DEP_FLAGS) $(LIB_DEP_PATH_FLAGS) $^


$(ST_LIB_NAME): $(LIB_OBJECTS)
	ar -rv $@ $^

clean:
	@rm -rfv $(DY_LIB_NAME)
	@rm -rfv $(ST_LIB_NAME)
	@find -name "*$(LIB_OBJ_SUFFIX)" -type f -exec rm -rfv '{}' \;
	$(MAKE) --directory=$(PROJECT_WRAP_PATH) clean

.PHONY: install
install: lib
	install -Dv $(DY_LIB_NAME) $(INSTALL_PREFIX)/lib/$(DY_LIB_NAME)
	install -Dv $(ST_LIB_NAME) $(INSTALL_PREFIX)/lib/$(ST_LIB_NAME)
	$(MAKE) --directory=$(PROJECT_WRAP_PATH) install
