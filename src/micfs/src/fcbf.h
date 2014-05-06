#ifndef MICFS_HEADER
#define MICFS_HEADER
#include"mic_matrix.h"
#include "mic.h"

typedef struct ListNode
{
    float mic;
    int idx;
    struct ListNode *nn;
    struct ListNode *pn;
}list_node;

typedef struct List
{
    list_node * header;
    list_node * tail;
    int len;
}list;

void add_l(list * lst);
void del_l_ptr(list *lst,list_node *tp);
void brk_l(list *lst);
void in_mic(list *lst,data_type m); 
void chk_l(list *lst);

list * fcfs(mic_matrix M,float sa);

#endif
