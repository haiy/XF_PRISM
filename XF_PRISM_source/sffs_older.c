#include "sffs.h"

/*这个算法的实现主要的数据结构是属性表也就是struct FeatureList
  ，属性表结构中不仅有指向实际存储链表头的指针，还存储有所有
  属性的索引位置，能够方便进行属性的增减。
 */

/*调试记录：
Mon Mar 11 20:02:59 CST 2013
现在调试到83行。
Tue Mar 12 14:27:40 CST 2013
调试至gmsw完成，出错原因在对列表进行addnode时候
没有对membs进行实际的增加删除操作。
Tue Mar 12 20:05:55 CST 2013
调试至del_lsi函数
Thu Mar 14 15:43:13 CST 2013
调试lsi!=last member部分
Wed Mar 20 12:52:17 CST 2013
修改mic_matrix部分，将其改为下三角矩阵。
Fri Mar 22 09:23:54 CST 2013
调试问题是：若调用add_node反向的时候不是期望的结果,返回值的原因
 */
/*
int main()
{
    mic_matrix M;
    char *fnm="test.csv";
    fill_mic_matrix(fnm,&M);
    sffs(M,3);
    return 1;
}
*/

/*
   n denotes the target number of features
*/
fl_tp sffs(mic_matrix M,int n)
{
#define debug_total
    /* Begin to initialize list Xk and Ym */
    /* Begin Xk */
    fl_tp Xk=(fl_tp)malloc(sizeof(feature_list));
    if(Xk==NULL)
    {
        printf("SFFS Xk initial memory error!\n");
        exit(1);
    }
    Xk->k=0;
    Xk->sig=0;
    Xk->membs=NULL;
    Xk->header=Xk->tail=NULL;
    /*Begin Ym */
    fl_tp Ym=(fl_tp)malloc(sizeof(feature_list));
    Ym->k=M.atrn-1;
    Ym->membs=(int *)malloc(sizeof(int)*(Ym->k));
    Ym->header=Ym->tail=NULL;

    int i=0;
    for(i=0;i<Ym->k;i++)
    {
        Ym->membs[i]=i;
        fet_tp cur=(fet_tp)malloc(sizeof(feature_list));
        cur->atr=i;
        cur->pos=i;
        cur->mic=M.mic[M.class_idx][i];
        cur->sig=0.00;
        cur->nn=NULL;
        cur->pn=NULL;
        if(Ym->header==NULL&&Ym->tail==NULL)
        {
            Ym->header=cur;
            Ym->tail=cur;
            cur->pn=cur->nn=NULL;
        }
        else
        {
            Ym->tail->nn=cur;
            cur->pn=Ym->tail;
            Ym->tail=cur;
        }
    }
    /* Begin to add first d features into Xk */
    while(Xk->k < 2)
    {
        int msw=gmsw(Xk,Ym,M);
        add_node(msw,Xk,Ym,M);
#ifdef debug_total
        printf("Now add msw %d to Xk which has %d members.\n",msw,Xk->k);
#endif
    }
#ifdef debug_total
    printf("Xk members :");
    check_feature_list(Xk);
#endif
    /* find the approriate d features */
    int d=n;
    if(d>=M.atrn)
    {
        /*if the target number of feature*/
        d=rnd(M.atrn/3.00);
    }
#ifdef debug_total
    printf("sff d :%d,M.atrn:%d\n",d,M.atrn);
#endif
    while(Xk->k <= d && Ym->k>0)
    {
        int msw=gmsw(Xk,Ym,M);
        fl_tp Xk_plus=plus_msw(Xk,msw);
        int lsi=glsi(Xk_plus,M);
#ifdef debug_total
        printf("*****New loop,Xk has %d members.******\n The lsi is %d while the last is %d.\n",Xk->k,lsi,msw);
#endif
        if(lsi != Xk_plus->membs[Xk_plus->k-1])
        {
            if(Xk_plus->k-1==2)
            {
                Xk=add_node(msw,Xk,Ym,M);
                add_node(lsi,Ym,Xk,M);
#ifdef debug_total
                printf("Xk'->k=2, now Xk:");
                check_feature_list(Xk);
#endif
                free(Xk_plus->membs);
                free(Xk_plus);
            }
            else
            {
                fl_tp Xk_chg=del_lsi(lsi,Xk_plus,M);
#ifdef debug_total
                printf("Begin to condition exclusion,Xk->tail:%d,Xk' without lsi\t",Xk->tail->atr);
                check_feature_list(Xk_chg);
#endif
                while( Xk_chg->k > 2 && Ym->k>0)
                {
                    int lsi_chg=glsi(Xk_chg,M);
                    fl_tp Xk_miu_chg=del_lsi(lsi_chg,Xk_chg,M);//Xk'-1
#ifdef debug_total
                    printf("Xk's lsi :%d,Xk'-1 is ",lsi_chg);
                    check_feature_list(Xk_miu_chg);
                    printf("Sig(Xk'-1) %f and Sig(Xk-1) %d.so..\n",EXACT( Xk_miu_chg->sig), (Xk->tail->pn->atr));
#endif
                    if(EXACT( Xk_miu_chg->sig) > EXACT(Xk->tail->pn->sig) )
                    {
                        fl_tp Xk_chg=lst_rplc(Xk_chg,Xk_miu_chg);
                        add_node(Xk->tail->atr,Ym,Xk,M);
#ifdef debug_total
                        printf("Cause Xk'-1 > Xk-1,so now Xk");
                        check_feature_list(Xk);
#endif
                    }
                    else
                    {
                        add_node(msw,Xk,Ym,M);
                        Xk=del_lsi(lsi,Xk,M);
#ifdef debug_total
                        printf("Cause Xk'-1 < Xk-1,So now Xk");
                        check_feature_list(Xk);
#endif
                        free(Xk_miu_chg->membs);
                        free(Xk_miu_chg);
                        break;
                    }
                }
            }
        }
        else
        {
            Xk=add_node(msw,Xk,Ym,M);
#ifdef debug_total
            printf("Lsi equals the last one,and now Xk ");
            check_feature_list(Xk);
#endif
        }
    }
    return Xk;
}

/* find most significant feature with respect to Xk */
int gmsw(fl_tp Xk,fl_tp Ym,mic_matrix M)
{
    int *lst_Xk=(int*)malloc((Xk->k+1)*sizeof(int));
    int i=0;
    if(Xk->k!=0)
    {
        for(i=0;i<Xk->k;i++)
        {
            lst_Xk[i]=Xk->membs[i];
        }
    }
    int Xk_len=Xk->k+1;
    float max=-FLT_MAX;
    int atr=-1;
    fet_tp cur=Ym->header;
    for(i=0;i<Ym->k;i++)
    {
        lst_Xk[Xk_len-1]=cur->atr;
#ifdef debug_gmsw
        printf("cur->atr:%d\nCheck Xk_:",cur->atr);
        check_arry(lst_Xk,Xk->k+1);
#endif
        float merit=cal_merit(lst_Xk,Xk_len,M);
#ifdef debug_gmsw
        printf("Debugging gmsw and now merit %f with atr %d.\n",merit,cur->atr);
#endif
        if(EXACT(merit)>EXACT(max))
        {
            max=merit;
            atr=cur->atr;
        }
        cur=cur->nn;
    }
    free(lst_Xk);
#ifdef debug_gmsw
    printf("now atr:%d\n",atr);
#endif
    return atr;
}

/* cut the target feature down from Ym and add it to Xk actually*/
fl_tp add_node(int atr,fl_tp Xk,fl_tp Ym,mic_matrix M)
{
#define debug_addnode
    int *Xk_membs=(int *)malloc(sizeof(int)*(Xk->k+1));
    int *Ym_membs=(int *)malloc(sizeof(int)*(Ym->k-1));
    int p=0;
    int i=0;
    for(i=0;i<Xk->k;i++)
    {
        Xk_membs[i]=Xk->membs[i];
    }
#ifdef debug_addnode
    printf("Input atr :%d \n",atr);
#endif
    for(i=0;i<Ym->k;i++)
    {//get Xk_membs and Ym_membs
        if(Ym->membs[i]==atr)
        {
            Xk_membs[Xk->k]=atr;
#ifdef debug_addnode
            printf("gmw allign Xk:%d,Xk->k:%d\n",Xk_membs[Xk->k],Xk->k);
#endif
        }
        else
        {
            Ym_membs[p++]=Ym->membs[i];
#ifdef debug_addnode
            printf("gmw allign Ym:%d\n",Ym_membs[p-1]);
#endif
        }
    }
    int * tp=Xk->membs;
    Xk->membs=Xk_membs;
#ifdef debug_addnode
    for(i=0;i<Xk->k+1;i++)
    {
        printf("Xk members:%d\n",Xk->membs[i]);
    }
#endif
    free(tp);
    tp=NULL;
    tp=Ym->membs;
    Ym->membs=Ym_membs;
    free(tp);
    tp=NULL;

    /* cut down the target node from Ym and
      add it to the Xk list             */
    fet_tp cur=Ym->header;
    fet_tp msw=NULL;
    while(cur)
    {
        if(cur->atr==atr)
        {
            msw=cur;
            break;
        }
        cur=cur->nn;
    }
    if(msw==NULL)
    {
        puts("Not found the msw .This error is in add_node ");
    }
    /*cut down the target node from Ym */
    if(msw==Ym->header&&msw==Ym->tail)
    {
        Ym->header=NULL;
        Ym->tail=NULL;
    }
    else if(msw==Ym->header&&msw!=Ym->tail)
    {
        Ym->header=msw->nn;
        msw->nn->pn=NULL;
    }
    else if(msw==Ym->tail&&msw!=Ym->header)
    {
        Ym->tail=msw->pn;
        msw->pn->nn=NULL;
    }
    else
    {
        msw->pn->nn=msw->nn;
        msw->nn->pn=msw->pn;
    }
    msw->pn=NULL;
    msw->nn=NULL;
    if(Xk->header==NULL)
    {
        Xk->header=msw;
        Xk->tail=msw;
    }
    else
    {
        Xk->tail->nn=msw;
        msw->pn=Xk->tail;
        Xk->tail=msw;
    }
    Xk->k+=1;
    Ym->k-=1;
#ifdef debug_addnode
    check_feature_list(Ym);
    check_feature_list(Xk);
#endif
    if(Ym->header)
    {
        Ym->sig=cal_merit(Ym->membs,Ym->k,M);
        Ym->tail->sig=Ym->sig;
    }
    if(Xk->header)
    {
        Xk->sig=cal_merit(Xk->membs,Xk->k,M);
        Xk->tail->sig=Xk->sig;
    }
#ifdef debug_addnode
    printf("check Xk->sig:%f\tYm->sig:%f\n",Xk->sig,Ym->sig);
#endif
    return Xk;
}

/*Just add the msw to a new list*/
fl_tp plus_msw(fl_tp Xk,int msw)
{
    fl_tp Xk_plus=(fl_tp)malloc(sizeof(feature_list));
    Xk_plus->k=Xk->k+1;
    Xk_plus->membs=(int *)malloc(sizeof(int)*(Xk_plus->k));
    int i=0;
    for(i=0;i<Xk_plus->k-1;i++)
    {
        Xk_plus->membs[i]=Xk->membs[i];
    }
    Xk_plus->membs[i]=msw;
#ifdef debug_plus_msw
    printf("Xk->last:%d\ti:%d\tmsw:%d\n",Xk_plus->membs[Xk_plus->k-1],i,Xk_plus->membs[i]);
#endif
    Xk_plus->header=Xk_plus->tail=NULL;
    return Xk_plus;
}

/*find the least significant feature in Xk*/
int glsi(fl_tp Xk_plus,mic_matrix M)
{
    int l=Xk_plus->k;
    int i=0;

    int *set=(int *)malloc(sizeof(int)*(l-1));
    int atr=-1;
    float max=-1;
    for(i=0;i<l;i++)
    {
        int j=0;
        int k=0;
        for(j=0;j<l;j++)
        {
            if(j!=i)
            {
                set[k++]=Xk_plus->membs[j];
            }
        }
        float merit=cal_merit(set,l-1,M);
#ifdef debug_glsi
        printf("list without:%d\tmerit is %f\n",Xk_plus->membs[i],merit);
#endif
        if(EXACT(merit)>=EXACT(max))
        {
            max=merit;
            atr=Xk_plus->membs[i];
        }
    }
    free(set);
    return atr;
}

/*remove the target node from Xk_plus and return a new list*/
fl_tp del_lsi(int lsi,fl_tp Xk_plus,mic_matrix M)
{
    fl_tp chg=(fl_tp)malloc(sizeof(feature_list));
    chg->membs=(int *)malloc(sizeof(int)*(Xk_plus->k-1));
    chg->k=Xk_plus->k-1;
    int i=0;
    int k=0;
    for(i=0;i<Xk_plus->k;i++)
    {
        if((Xk_plus->membs[i])!=lsi)
        {
            chg->membs[k++]=Xk_plus->membs[i];
        }
    }
#ifdef debug_del_lsi
    puts("Here In the del_lsi.");
    check_feature_list(chg);
#endif
    chg->sig=cal_merit(chg->membs,chg->k,M);
    chg->header=Xk_plus->header;
    chg->tail=Xk_plus->tail;
    return chg;
}

/*break down the target feature list*/
fl_tp lst_rplc(fl_tp original,fl_tp target)
{
    free(target->membs);
    free(target);
    return original;
}


void brk_fltp(fl_tp Xk)
{
    fet_tp cur=Xk->tail;
    while(cur)
    {
        fet_tp tp=cur;
        cur=cur->pn;
        free(tp);
        tp=NULL;
    }
}
void check_feature_list(fl_tp list)
{
    int i=0;
    RED_1;
    //printf("There are %d features !\n",list->k);
    RED_0;
   // puts("Members:");
    for(i=0;i<list->k;i++)
    {
        printf("%d\t",list->membs[i]);
    }
    if(list->header)
    {
        puts("Real One.\nCheck nodes: ");
        fet_tp cur=list->header;
        while(cur)
        {
            printf("atr:%d\tmic:%f\tsig:%f\n",cur->atr,cur->mic,cur->sig);
            cur=cur->nn;
        }

    }
    else
    {
        printf("Virtual one.\n");
    }

//    puts("Feature list check over !");
}
