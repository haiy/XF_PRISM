#ifndef READ_ALL_CONTENT
#define READ_ALL_CONTENT

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include"mic.h"
typedef struct FileContent
{
    char *fn;
    int inst;
    int atrn;
    float **ct;
}fct;
float ** read_content(fct *file);
float mic_ct_mem(fct file_content,Points *D,int a ,int b,float alpha,int f);
void cal_mic_ctmem(int argc,char *argv[]);

#endif
