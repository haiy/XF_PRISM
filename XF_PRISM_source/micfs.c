#include "mic.h"
#include "micfs.h"

int micfs_main(int argc,char *argv[])
{
    int atrn=count_atr(argv[2]);
    int flag=para_chk(argc,argv,atrn);
    if(flag==0)
    {
        puts("Program will exit now !");
        exit(0);
    }
    char * filename=argv[2];
    Points * D=(Points *)malloc(sizeof(Points));
    D->len=count_inst(filename);
    D->x=(data_type *)malloc(sizeof(data_type)*D->len);
    D->y=(data_type *)malloc(sizeof(data_type)*D->len);
    D->grid=(label_type *)malloc(sizeof(label_type)*D->len);

    if(flag==1)
    {
        puts("MIC only mode !");
        float MIC=cal_mic_inter(filename,D,atoi(argv[3]),atoi(argv[4]),atof(argv[5]),atoi(argv[6]));
        printf("%f\n",MIC);
    }
    else if(flag==2)
    {
        puts("MIC-FS module begin...");
        list * btl=micfs(filename,D,atof(argv[3]),atoi(argv[4]));
        printf("Feature selection part successfully finished ! \n");
        list_node *tp=btl->header;
        /* write selected features' results to file*/
        FILE *fsf=fopen("selected_features.csv","w+");
        char *fbf=(char *)malloc(sizeof(char)*200);
        while(tp)
        {
            sprintf(fbf,"%d,%f\n",tp->idx,tp->mic);
            fputs(fbf,fsf);
            tp=tp->nn;
        }
        fclose(fsf);
        free(fbf);
        brk_l(btl);
    }
    return 1;
}

/* return the best subset list */
list * micfs(char *filename,Points * D,float alpha,int f)
{
    int atrn=count_atr(filename);
    printf("atrn:%d\tlen:%d\n",atrn,D->len);
    data_type *MICS=(data_type *)malloc(sizeof(data_type)*(atrn-1));
    int i=0;

    /* 1st stage of MIC-FCBF */
    FILE * fo=fopen("cmic.csv","w+");
    char * sbf=(char *)malloc(sizeof(char)*(300));


    for(i=0;i+1<atrn;i++)
    {
        /*class label is the first attribute */
        MICS[i].val=cal_mic_inter(filename,D,atrn-1,i,alpha,f);
        sprintf(sbf,"%d,%f\n",i,MICS[i].val);
        if(MICS[i].val)
        {
            fputs(sbf,fo);
        }
        MICS[i].pos=i;
    }
    printf("all attributes's mic to class over !\n");
    fclose(fo);
    free(sbf);

    /* sorted mic array */
    q_sort(MICS,atrn-1);

    /* full fill the list with all mics */
    list *lst=(list * )malloc(sizeof(list));
    lst->header=NULL;
    lst->tail=NULL;
    /* sigema is the threshold value */
    float sigema=0.00;
    for(i=atrn-2;i>=0;i--)
    {
        if(!(EXACT(MICS[i].val)<EXACT(sigema)))
        {
            in_mic(lst,MICS[i]);
            //printf("Now the %d:%f feature will be in list!\n ",i,MICS[i].val);
        }
        else
        {
            break;
        }
    }
    free(MICS);

    /* 2nd stage of MIC-FCBF*/
    puts("Begin selection.Running...");
    list_node *tp=lst->header;
    while(tp)
    {
        list_node *tq=tp->nn;
        while(tq)
        {
            /* the relevance of p and q */
            float rpq=cal_mic_inter(filename,D,tp->idx,tq->idx,alpha,f);
            /* if the relationship between q and class is
               stronger than that of p,q ,then
               remove current q.
             */
            if(!(EXACT(rpq)<EXACT(tq->mic)))
            {
                //printf("Now the %d:%f feature will be out!\n ",tq->idx,tq->mic);
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

    free(D->x);
    free(D->y);
    free(D);

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
}

/* del a certain element in the list */
void del_l_ptr(list *lst,list_node *tp)
{
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
void del_l_idx(list *lst,int idx)
{
    if(lst->header==NULL)
    {
        puts("No list or list empty!");
        return;
    }
    list_node *tp=lst->header;
    while(tp)
    {
        if(tp->idx==idx)
        {
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

        }
        tp=tp->nn;
    }
    printf("Index %d not found !\n",idx);
}

/* break the list */
void brk_l(list *lst)
{
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


