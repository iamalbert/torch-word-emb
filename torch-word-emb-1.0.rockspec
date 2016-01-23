package = "torch-word-emb"
version = "1.0"
source = {
   url = "https://github.com/iamalbert/torch-word-emb" -- We don't have one yet
}
description = {
   summary = "load word2vec or glove word vectors to torch.Tensor",
   detailed = [[
	load word2vec or glove word vectors to torch.Tensor
   ]],
   homepage = "https://github.com/iamalbert/torch-word-emb", -- We don't have one yet
   license = "MIT" -- or whatever you like
}
dependencies = {
   "torch >= 7.0"
}
build = {
	type = "make",
	build_variables  = {
		 CFLAGS="-std=c99 -Wall -pedantic -O2 -I$(LUA_INCDIR)/TH -I$(LUA_INCDIR)",
         LIBFLAG="$(LIBFLAG)",
         LUA_LIBDIR="$(LUA_LIBDIR)",
         LUA_BINDIR="$(LUA_BINDIR)",
         LUA_INCDIR="$(LUA_INCDIR)",
         LUA="$(LUA)",
	},
    install_variables = {
         PREFIX="$(PREFIX)",
         BINDIR="$(BINDIR)",
         LIBDIR="$(LIBDIR)",
         LUADIR="$(LUADIR)",
         CONFDIR="$(CONFDIR)",
    },
}
