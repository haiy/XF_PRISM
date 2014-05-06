#ifndef MIC_H
#define MIC_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<ctype.h>
#include"para_chk.h"

//#define DEBUG_eqpy
//#define DEBUG_1
#define Debug 7
#define LEVEL_1 3   /* the higest level of debug */
#define LEVEL_2 5    /* only important messages will show */
#define LEVEL_3 4
#define PI 3.1415926
#define LOWEST -FLT_MAX
#define CHECK_DATA 0 
#define  bufsize 100000
#define EXACT(x) ((floor)(x*1000000))/1000000

/* 在大量数据计算的时候会导致错误 ,谨慎使用 */
/*
#define MAX(a,b) (EXACT(a)>EXACT(b))?a:b
#define MIN(a,b) (EXACT(a)>EXACT(b))?b:a
*/
#if (CHECK_DATA==1)
#undef Debug
#define Debug 0
#endif
/* the definition of a point */
typedef struct Point
{
	double val;
	int   pos;
}data_type;

typedef struct Label
{
	/* rn :row number */
	int rn;
	/* cn: clomn number */
	int cn;
}label_type;
/* the definition of the data */
typedef struct DataSet
{
	data_type * x;
	data_type * y;
	label_type * grid;
	int len;
}Points;

/*------------------------------------------------------------*/
/* functions announcement */
void gd ( Points * D);
void eqpY ( Points *D, data_type * Dy,int l,int y );
void q_sort( data_type * sd, int l );
void qs(data_type *sd ,int left , int right );
int p_qs(data_type *sd,int left,int right);
void swap(data_type * sd,int a,int b);
void vd(Points * D,data_type *sdx ,int l);
void vyp ( int y , int * yp );
int rnd( float x );
int eqpX (Points *D,data_type * Dx,data_type *Dy, int l, int x, int y, int f);
int clmX ( data_type * Dx, int l, int x, int y, int f, int *clm );
int smr(Points *D,data_type *sdx,int a,int b);
int smx (data_type *sdx,int a,int b);
int smc(data_type *sdx,int a ,int b,int clmn,int * clm);
void fu(int len ,data_type *SRC_DA,data_type *DES_DB );
void gdrs(Points *D ,int ** rs ); 
void bub_sort(data_type *sd, int l);
void hst(int clmn,int y,int ** rs,float ** Hpq,float **Hst,float **Ai,float *Hq);
void Optx(int x,int y, int clmn, int **rs, float **MI);
void cal_MI(Points *D,data_type * Dx,data_type * Dy,int x ,int y, int f,float **MI);
int count_atr(char * filename);
int count_inst(char * filename);
void rp(FILE * src,int xa, int yb,data_type *x,data_type *y);
int read_pair(char * filename,int xa,int yb,data_type *x,data_type *y);
int chk_sm(Points * D);

int ckpa(int argc,char *argv[],int atrn);
float cal_mic_inter(char *filename,Points *D,int xa,int yb,float alpha,int f);

float opa (int argc,char *argv[] );
float cal_mic(Points *D,float alpha,int f);
void cal_mic_1all(char *filename,int a,float alpha,int f);
/*-------------------------------------------------------------*/

#include"read_content.h"

#endif
