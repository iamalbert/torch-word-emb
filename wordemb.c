#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wordemb.h"

#define Real Float
#define real float

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_Tensor TH_CONCAT_STRING_3(torch., Real, Tensor)

#define MAX_WORD_LEN 1024

typedef char (*DICT)[MAX_WORD_LEN];



static int read_vectors( FILE*fp, DICT words, float * storage, size_t dim, size_t n_word ){

    for( int i = 0; i < n_word; i++ ){
        float *vector = storage + dim * i;

        fscanf(fp, "%s", words[i] );

        fgetc(fp); // delete ' '

        fread( vector, sizeof(float), dim, fp );

        fgetc(fp); // delete '\n'

        //printf("%s\t%lf %f %f %f\n", word, vector[0], vector[1], vector[dim-2], vector[dim-1] );
    }
    return 0;
}

static void allocate_data( 
    THTensor **tensor, DICT * words, size_t dim, size_t n_word ){
    *words = calloc( MAX_WORD_LEN * sizeof(char), n_word );
    *tensor = THTensor_(newWithSize2d)(n_word, dim);
}


static int set_return_vars( LUASTATE, THTensor *self, DICT words, size_t dim, size_t n_word ){
    THTensor_(renorm)(self, self, 2, 1, 1);

    lua_newtable(L);

    lua_newtable(L);
    for ( int i = 0; i < n_word; i++ ){
        lua_pushinteger(L, i+1);
        lua_setfield(L, -2, words[i] );
    }
    lua_setfield(L, -2, "words");

    luaT_pushudata(L, self, torch_Tensor);
    lua_setfield(L, -2, "vec");

    return 1;
}

static int load_word_embedding(LUASTATE, FILE*fp, size_t dim, size_t n_word ){
 
    THTensor *self;
    DICT word;

    allocate_data( &self, &word, dim, n_word);
    read_vectors( fp, word, THTensor_(data)(self), dim, n_word );
    return set_return_vars(L, self, word, dim, n_word);

}

int load_word2vec_bin( LUASTATE ){
    const char * filepath = luaL_checklstring(L, 1, NULL);
    size_t n_word, dim;

    FILE *fp = fopen(filepath, "rb");
    if( fp == NULL ){ return -1; }

    fscanf(fp, "%zu %zu", &n_word, &dim);
    fgetc(fp);
    //printf("%zu %zu\n", n_word, dim );

    return load_word_embedding(L, fp, dim, n_word);
}

int load_glove_bin(LUASTATE){
    const char * filepath = luaL_checklstring(L, 1, NULL);
    size_t n_word = luaL_checkint(L, 2);
    size_t dim = luaL_checkint(L, 3);

    FILE *fp = fopen(filepath, "rb");

    return load_word_embedding(L, fp, dim, n_word);
}

int luaopen_wordemb(LUASTATE){
    lua_newtable(L);

    lua_pushcfunction( L, load_word2vec_bin );
    lua_setfield(L, -2, "load_word2vec_bin");

    lua_pushcfunction( L, load_glove_bin );
    lua_setfield(L, -2, "load_glove_bin");

    return 1;
}

