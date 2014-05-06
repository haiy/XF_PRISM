#ifndef GA_HEADER
#define GA_HEADER

#include<time.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include"mic_matrix.h"

//#define debug_ini_pop
//#define debug_acu
//#define debug_sel_one
//#define debug_crx
//#define debug_mt
//#define chg_debug
//#define debug_elt
typedef struct Chrosome
{
    int l;
    float sig;
    int *chro;
}chrosome,*chro_ptr;

typedef struct Population
{
    int n;
    float *acu;
    float pc;
    float pm;
    chro_ptr m;
}population;

chro_ptr ga(mic_matrix M,int psz,int n,float pc,float pm);
void ini_pop(mic_matrix M,population *pop);
void check_pop(population pop);
int gentype(int *bincode,int **gtp,int l);
void cal_acu(population *pop);
chro_ptr sel_one(population pop);
population *ini_tmpop(population pop);
void cross_over(chro_ptr father,chro_ptr mother);
void cpy_chro(chro_ptr src,chro_ptr dst);
int CY(population pop);
int MT(population pop);
void mutation(chro_ptr chros);
void chg_chro(population *pop,int a,int b);
void chg_chro_PS(population *pop,int a,population *S,int b);
int partp(int a,int b,population *pop);
void qsp(int a,int b,population *pop);
void qksort(population *pop);
population *elitist_sel(population *pop,population *S);
void brk_pop(population *pop);
void update_pop(population *S,mic_matrix M);
#endif
