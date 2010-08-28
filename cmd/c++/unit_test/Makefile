all: table_db

table_db: table_db.o
	g++ -Wl,-rpath=$(PROJECT_LIB_PATH) -o $@ $< -lglooper -llua -ldl -lsedna -L$(PROJECT_LIB_PATH) -L$(SEDNA_C_PATH)

CXXFLAGS += -g -fPIC -I$(PROJECT_INCLUDE_PATH)
