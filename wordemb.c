#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wordemb.h"

#define Real Float
#define real float

#define torch_(NAME) TH_CONCAT_3(torch_, Real, NAME)
#define torch_Tensor TH_CONCAT_STRING_3(torch., Real, Tensor)

#define MAX_WORD_LEN 2048

typedef char (*DICT)[MAX_WORD_LEN];



static int read_vectors_bin( FILE*fp, DICT words, float * storage, size_t dim, size_t n_word ){

    for( int i = 0; i < n_word; i++ ){
        float *vector = storage + dim * i;

        fscanf(fp, "%s", words[i] );


        fgetc(fp); // delete ' '

        fread( vector, sizeof(float), dim, fp );

        fgetc(fp); // delete '\n'

        //printf("%s\t%f %f %f %f\n", words[i], vector[0], vector[1], vector[dim-2], vector[dim-1] );
    }
    return 0;
}
static int read_vectors_text( FILE*fp, DICT words, float * storage, size_t dim, size_t n_word ){

    for( int i = 0; i < n_word; i++ ){
        float *vector = storage + dim * i;

        fscanf(fp, "%s", words[i] );

        for( int j=0; j<dim; j++){
            fscanf( fp, "%f", & vector[j] );
        }

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

static int load_word_embedding(LUASTATE, FILE*fp, size_t dim, size_t n_word, int is_bin, int to_renorm ){
 
    THTensor *self;
    DICT word;

    allocate_data( &self, &word, dim, n_word);
    if ( is_bin ){
        read_vectors_bin( fp, word, THTensor_(data)(self), dim, n_word );
    }else{
        read_vectors_text( fp, word, THTensor_(data)(self), dim, n_word );
    }

    if( to_renorm ){
        float *storage = THTensor_(data)(self);

        for ( size_t i=0; i < n_word; i++ ){
            float * d = storage + i * dim;
            float sum = 0;
            for( size_t j = 0; j < dim ; j++ ) sum += d[j] * d[j];
            sum = sqrt(sum);
            for( size_t j = 0; j < dim ; j++ ) d[j] /= sum;
        }
    }

    return set_return_vars(L, self, word, dim, n_word);

}


int load_word2vec_bin( LUASTATE ){
    const char * filepath = luaL_checklstring(L, 1, NULL);
    size_t n_word, dim;

    FILE *fp = fopen(filepath, "rb");
    if( fp == NULL ){ return -1; }

    fscanf(fp, "%zu %zu", &n_word, &dim);
    fgetc(fp);

    return load_word_embedding(L, fp, dim, n_word, 1, 1);
}
int load_word2vec_text( LUASTATE ){
    const char * filepath = luaL_checklstring(L, 1, NULL);
    size_t n_word, dim;

    FILE *fp = fopen(filepath, "rb");
    if( fp == NULL ){ return -1; }

    fscanf(fp, "%zu %zu ", &n_word, &dim);
    fgetc(fp);
    //printf("%zu %zu\n", n_word, dim );

    return load_word_embedding(L, fp, dim, n_word, 0, 1);
}

static int get_line_numbers(FILE *fp){
    int cnt = 0;
    int ch = 0;
    do{
        ch = fgetc(fp);
        if( ch == '\n' ) cnt ++;
    }while( ch != EOF );

    rewind(fp);
    return cnt;
}
static int get_tokens_in_first_line(FILE*fp){
    int cnt = 0;
    int ch = 0, last = 0;
    do{
        ch = fgetc(fp);
        if( ch == '\n'){
           if( last != ' ' && last != 0 ){
               cnt++;
           }
        }else{
            if(  last == ' ' ){
                cnt++;
            }
        }
        last = ch;
    }while( ch != '\n' );
    rewind(fp);
    return cnt;
}
int load_glove_text( LUASTATE ){
    const char * filepath = luaL_checklstring(L, 1, NULL);

    FILE *fp = fopen(filepath, "rb");

    if( fp == NULL ){ return -1; }

    size_t dim    = get_tokens_in_first_line(fp) - 1;
    size_t n_word = get_line_numbers(fp);

    //printf("%zu %zu\n", n_word, dim );

    int ret = load_word_embedding(L, fp, dim, n_word, 0, 0);
    fclose(fp);
    return ret;
}


int luaopen_wordemb(LUASTATE){
    lua_newtable(L);

    lua_pushcfunction( L, load_word2vec_bin );
    lua_setfield(L, -2, "load_word2vec_bin");


    lua_pushcfunction(L, load_word2vec_text );
    lua_setfield(L, -2, "load_word2vec_text" );

    lua_pushcfunction(L, load_glove_text );
    lua_setfield(L, -2, "load_glove_text");

    return 1;
}

