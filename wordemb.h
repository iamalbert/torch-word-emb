#ifndef WORD_EMB_H
#define WORD_EMB_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <luaT.h>
#include <TH.h>

#define LUASTATE lua_State *L

int load_word2vec_bin( LUASTATE );


#endif
