#ifndef WORD_EMB_H
#define WORD_EMB_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <luaT.h>
#include <TH.h>

#define LUASTATE lua_State *L

#define Real Float
#define real float

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_Tensor TH_CONCAT_STRING_3(torch., Real, Tensor)

int load_word2vec_bin( LUASTATE );


#endif
