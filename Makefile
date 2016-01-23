#TORCH_DIR=$(realpath $(dir $(shell which th))../)

#LUA_INCDIR ?= $(TORCH_DIR)/include
#LUA_LIBDIR ?= $(TORCH_DIR)/lib

#CFLAGS ?=-std=c99 -Wall -pedantic -O2 -I$(LUA_INCDIR)/TH -I$(LUA_INCDIR)

wordemb.so: wordemb.c
	@echo --- build
	@echo CFLAGS $(CFLAGS)
	@echo LIBFLAG $(LIBFLAG)
	@echo LUA_LIBDIR $(LUA_LIBDIR)
	@echo LUA_BINDIR $(LUA_BINDIR)
	@echo LUA_INCDIR $(LUA_INCDIR)
	@echo LUA $(LUA)
	$(CC) $(CFLAGS) -fpic -shared -o $@ $^

install:
	@echo --- install
	@echo INST_PREFIX: $(PREFIX)
	@echo INST_BINDIR: $(BINDIR)
	@echo INST_LIBDIR: $(LIBDIR)
	@echo INST_LUADIR: $(LUADIR)
	@echo INST_CONFDIR: $(CONFDIR)

	cp wordemb.so $(LIBDIR)

clean:
	rm -rf wordemb.so


