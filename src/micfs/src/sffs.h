#ifndef SFFS_H
#define SFFS_H

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "mic.h"
#include"mic_matrix.h"

//#define debug_total
#define RED_1 printf("\033[31m")
#define RED_0 printf("\033[0m")

/*这个算法的实现主要的数据结构是属性表也就是struct FeatureList
  ，属性表结构中不仅有指向实际存储链表头的指针，还存储有所有
  属性的索引位置，能够方便进行属性的增减。
 */


/* struct of the feature node */
typedef struct FeatureNode
{
    int atr;
    float sig;
    struct FeatureNode *pn;
    struct FeatureNode *nn;
}feature_node,*fet_tp;

typedef struct FeatureList
{
    int * membs;
    int k;
    float sig;      //the significane of current feature list 
    fet_tp header;
    fet_tp tail;
}feature_list,*fl_tp;


fl_tp ini_Xk();
fl_tp ini_Ym(mic_matrix M);
void update_Xk_merit(fl_tp Xk,mic_matrix M);
int gmsw(fl_tp Xk,fl_tp Ym,mic_matrix M);
fl_tp add_node(int atr,fl_tp Xk,mic_matrix M);
fl_tp del_node(int atr,fl_tp Ym,mic_matrix M);
fl_tp plus_msw(fl_tp Xk,int msw);
int glsi(fl_tp Xk_plus,mic_matrix M);
fl_tp del_lsi(int lsi,fl_tp Xk_plus,mic_matrix M);
fl_tp lst_rplc(fl_tp *original,fl_tp *target);
fl_tp sffs(mic_matrix M,int n);
void check_feature_list(fl_tp list);
void brk_fltp(fl_tp Xk);

#endif
