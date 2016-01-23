#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "wordemb.h"

int load_word2vec_bin( LUASTATE ){
    /*
    const char * filepath = luaL_checklstring(L, 1, NULL);
 
    THTensor *self;
    self = THTensor_(newWithSize2d)(8, 5);

    THTensor_(zero)(self);

    THTensor_(set2d)(self, 5, 4, 1.002);

    luaT_pushudata(L, self, torch_Tensor);
    */
    return 1;
}

#define MAX_WORD_LEN 1024


void normalize(float *vec, size_t dim){
    float len = 0;
    int i;
    for( i = 0; i < dim; i++){
        len += vec[i] * vec[i];
    }
    len = sqrt(len);
    for( i = 0; i < dim; i++){
        vec[i] /= len;
    }
}

int load_word2vec_bin_impl( const char *path ){
    FILE *fp = fopen(path, "rb");

    if( fp == NULL ){ return -1; }

    int n_word, dim;
    char word[MAX_WORD_LEN];

    fscanf(fp, "%d %d", &n_word, &dim);
    fgetc(fp);
    printf("%d %d\n", n_word, dim );


    float *vector = (float*) malloc( sizeof(float) * dim );

    for( int i = 0; i < n_word; i++ ){
        fscanf(fp, "%s", word );

        fgetc(fp); // delete ' '

        fread( vector, sizeof(float), dim, fp );

        fgetc(fp); // delete '\n'

        normalize(vector, dim);
        printf("%s\t%lf %f %f %f\n", word, vector[0], vector[1], vector[dim-2], vector[dim-1] );
    }
    return 0;
}

int main(){
    load_word2vec_bin_impl("/nfs/cache/wlzhuang/allenai/ck12.500.skip.bin");
    return 0;
}
