
#ifndef PARA_H
#define PATA_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>




int para_chk(int argc,char *argv[],int atrn);
int ck_m(char *mode);
int ck_mic(int argc,char *argv[], int atrn);
int ck_micfs(int argc,char *argv[]);
int ck_n(int argc,int der);
int ck_f(char *filename);
int ck_i(int xa,int yb,int atrn);
int ck_e(float e);
void usage();

#endif
