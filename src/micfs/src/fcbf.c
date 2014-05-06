#include "fcbf.h"
/* return the best subset list */
list * fcfs(mic_matrix M,float sa)
{
    int atrn=M.atrn;
    data_type *MICS=(data_type *)malloc(sizeof(data_type)*(M.atrn-1));

    int i=0;
    /* 1st stage of MIC-FCBF */
    for(i=0;i<atrn-1;i++)
    {
        /*class label is the first attribute */
        MICS[i].val=M.mic[ M.class_idx ][ i ];
        MICS[i].pos=i;
    }
    /* sorted mic array */
    q_sort(MICS,atrn-1);
    /* full fill the list with all mics 
       sigema is the threshold value */
    list *lst=(list * )malloc(sizeof(list));
    lst->header=NULL;
    lst->tail=NULL;
    lst->len=0;
    float sigema=sa;
    for(i=atrn-2;i>=0;i--)
    {
        if(!(EXACT(MICS[i].val)<EXACT(sigema)))
        {
            in_mic(lst,MICS[i]);
        }
        else
        {
            break;
        }
    }
    free(MICS);
    /* 2nd stage of MIC-FCBF*/
    list_node *tp=lst->header;
    while(tp)
    {
        list_node *tq=tp->nn;
        while(tq)
        {
            /* the relevance of p and q */
            int a=tp->idx;
            int b=tq->idx;
            float rpq=0;
            if(a>b)   rpq=M.mic[a][b];
            if(a<b)   rpq=M.mic[b][a];
            /* if the relationship between q and class is
               stronger than that of p,q ,then
               remove current q.            */
            if(!(EXACT(rpq)<EXACT(tq->mic)))
            {
                del_l_ptr(lst,tq);
                tq=tp->nn;
            }
            else
            {
                tq=tq->nn;
            }
        }
        tp=tp->nn;
    }
    return lst;
}

/* put a node into the mic list */
void in_mic(list *lst,data_type m)
{
    add_l(lst);
    lst->tail->mic=m.val;
    lst->tail->idx=m.pos;
}

/* add an element to the current list */
void add_l(list * lst)
{
    list_node *tp=(list_node *)malloc(sizeof(list_node));
    if(lst->header==NULL)
    {
        tp->pn=NULL;
        tp->nn=NULL;
        lst->header=tp;
        lst->tail=tp;
    }
    else
    {
        tp->pn=lst->tail;
        tp->nn=NULL;
        lst->tail->nn=tp;
        lst->tail=tp;
    }
    lst->len+=1;
}

/* del a certain element in the list */
void del_l_ptr(list *lst,list_node *tp)
{
    lst->len-=1;
    if(tp->pn==NULL)
    {
        if(tp->nn==NULL)
        {
            lst->header=NULL;
            lst->tail=NULL;
            free(tp);
            return ;
        }
        else
        {
            lst->header=tp->nn;
            tp->nn->pn=NULL;
            free(tp);
            return;
        }
    }
    else if(tp->nn==NULL)
    {
        lst->tail=tp->pn;
        lst->tail->nn=NULL;
        free(tp);
        return ;
    }
    else
    {
        tp->pn->nn=tp->nn;
        tp->nn->pn=tp->pn;
        free(tp);
        return ;
    }
    printf("Node remove error !\n");
}

/* break the list */
void brk_l(list *lst)
{
    lst->len=0;
    if(lst==NULL)
    {
        return;
    }
    list_node * cur=lst->header;
    list_node * tp=cur;
    while(cur)
    {
        tp=cur;
        cur=cur->nn;
        free(tp);
    }
    lst->header=NULL;
}

void chk_l(list *lst)
{
    list_node * cur=lst->header;
    int i=0;
    while(cur)
    {
        printf("%d:%f\t",cur->idx,cur->mic);
        i+=1;
        cur=cur->nn;
    }
    puts("");
    printf("Find %d nodes in %d nodes list.\n",i,lst->len);
}
