, := ,
space :=
space +=
$(space) :=
$(space) +=


EXEC_SOURCES = $(shell find . -name "*.cxx" -type f)
EXEC_TARGETS = $(patsubst %.cxx,%,$(EXEC_SOURCES))

EXEC_INCLUDE_PATHS = $(PROJECT_INCLUDE_PATH) $(BOOST_INCLUDE_PATH) $(SEDNA_C_PATH)
EXEC_INCLUDE_FLAGS = $(foreach p,$(EXEC_INCLUDE_PATHS),-I$p)
EXEC_LIBS = glooper sedna lua dl
EXEC_LIB_PATHS = $(PROJECT_LIB_PATH) $(SEDNA_C_PATH)
EXEC_LIB_FLAGS = $(foreach l,$(EXEC_LIBS),-l$l)
EXEC_LIB_PATH_FLAGS = $(foreach p,$(EXEC_LIB_PATHS),-L$p)
EXEC_RPATH_FLAGS = $(subst $( ),$(,),$(foreach p,$(PROJECT_LIB_PATH),-rpath=$p))

.PHONY: cmd
cmd: $(EXEC_TARGETS)

clean:
	for f in $(EXEC_TARGETS); \
	   do \
	   ( \
	    rm -rfv $$f; \
	    rm -rfv $$f.o;\
	    ) \
	   done

%.o: %.cxx
	g++ -c -g -O2 -fPIC $(EXEC_INCLUDE_FLAGS) -o $@ $<

%: %.o
	g++ -Wl,$(EXEC_RPATH_FLAGS) $(EXEC_LIB_FLAGS) $(EXEC_LIB_PATH_FLAGS) -o $@ $<
