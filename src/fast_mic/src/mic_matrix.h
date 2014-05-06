#ifndef MIC_MATRIX
#define MIC_MATRIX
#include"mic.h"
//#define debug_fill_mic
typedef struct MicMatrix
{
    int class_idx;
    int atrn;
    float **mic;
}mic_matrix;
float cal_merit(int *lst,int l,mic_matrix M);
void fill_mic_matrix(char *fnm,mic_matrix *M,float exp,float f);
float ** cal_mic_matrix(char *fnm,float alpha,float f);
void check_mic_matrix(mic_matrix M);
void check_arry(int *l,int a);
float **cal_mic_section(char * fnm,int start_col,int end_col,float alpha,float f);
#endif
