#include"ga.h"
#include"sffs.h"
#include"fcbf.h"
#include"mic.h"


typedef struct FeatureType
{
    int atr;
    float mic;
}featyp,*ftp_ptr;

typedef struct ResultFeatures
{
    int numb;
    float sig;
    featyp *atrs;
}resfts,*rfl_ptr;
void chk_bst(resfts bst);
rfl_ptr gafs(mic_matrix M,int psz,int n,float pc,float pm);
rfl_ptr sffs_rf(mic_matrix M,int n);
rfl_ptr fcfs_rf(mic_matrix M, float sa);
void prt_bst(resfts bst,mic_matrix M) ;
void brk_bst(rfl_ptr *bst);
