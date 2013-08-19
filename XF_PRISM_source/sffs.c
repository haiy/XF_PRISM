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
  Sun Apr 28 21:14:41 CST 2013
  1.原始算法流程中的第三步的错误
  表现出来是内存错误，修改了链表的整体结构，添加merit更新函数
  双向链表的删除与添加问题。
  Mon Apr 29 10:32:04 CST 2013
  1.重写了第三步，解决算法的逻辑性错误。十分关键的！！！！

 */
/*
   int main()
   {
   mic_matrix M;
   char *fnm="test.csv";
   fill_mic_matrix(fnm,&M);
   sffs(M);
   return 1;
   }
 */
fl_tp ini_Xk()
{
    fl_tp Xk=(fl_tp)malloc(sizeof(feature_list));
    Xk->k=0;
    Xk->sig=0;
    Xk->membs=(int *)malloc(sizeof(int)*Xk->k);
    Xk->header=Xk->tail=NULL;
    return Xk;
}
fl_tp ini_Ym(mic_matrix M)
{
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
        cur->sig=0.00;
        cur->nn=NULL;
        cur->pn=NULL;
        if(Ym->header==NULL)
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
    return Ym;
}
fl_tp sffs(mic_matrix M,int n)
{
    /* Begin to initialize list Xk and Ym */

    fl_tp Xk=ini_Xk();
    fl_tp Ym=ini_Ym(M);
    /* Begin to add first d features into Xk */
    while(Xk->k < 2)
    {
        int msw=gmsw(Xk,Ym,M);
        add_node(msw,Xk,M);
        del_node(msw,Ym,M);
        //#define debug_total
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
    while(Xk->k < d && Ym->k>0)
    {
        int msw=gmsw(Xk,Ym,M);
        fl_tp Xk_plus=plus_msw(Xk,msw);
        int lsi=glsi(Xk_plus,M);
#ifdef debug_total
        printf("*****New loop,Xk has %d members d :%d,msw:%d.******\n Xk_plus lsi is %d while the Xk->last is %d.\n",Xk->k,d,msw,lsi,Xk->tail->atr);
#endif
        if(lsi != Xk_plus->membs[Xk_plus->k-1])
        {
            if(Xk_plus->k-1==2)
            {
                add_node(msw,Xk,M);
                del_node(msw,Ym,M);
                add_node(lsi,Ym,M);
                del_node(lsi,Xk,M);
                update_Xk_merit(Xk,M);
                free(Xk_plus->membs);
                free(Xk_plus);
#ifdef debug_total
                printf("Xk'->k=2, now Xk:");
                check_feature_list(Xk);
#endif
            }
            else
            {
                fl_tp Xk_chg=del_lsi(lsi,Xk_plus,M);
#ifdef debug_total
                printf("Begin to condition exclusion,Xk' without lsi\t");
                check_feature_list(Xk_chg);
#endif
                int lsi_chg=glsi(Xk_chg,M);

                fl_tp Xk_miu_chg=del_lsi(lsi_chg,Xk_chg,M);//Xk'-1
#ifdef debug_total
                printf("Xk's lsi :%d,Xk'-1 is ",lsi_chg);
                check_feature_list(Xk_miu_chg);
                printf("Sig(Xk'-1) %f and Sig(Xk-1) %f.so..\n",EXACT( Xk_miu_chg->sig), EXACT(Xk->tail->pn->sig));
#endif
                if(EXACT( Xk_miu_chg->sig) > EXACT(Xk->tail->pn->sig) )
                {
                    while(EXACT(Xk_miu_chg->sig) > EXACT(Xk->tail->pn->sig) && Xk_miu_chg->k >2 )
                    {
                        lst_rplc(&Xk_chg,&Xk_miu_chg);
                        free(Xk_miu_chg->membs);
                        free(Xk_miu_chg);
                        del_node(Xk->tail->atr,Xk,M);
                        add_node(Xk->tail->atr,Ym,M);
                        int lsi_chg=glsi(Xk_chg,M);
                        fl_tp Xk_miu_chg=del_lsi(lsi_chg,Xk_chg,M);//Xk'-1
#ifdef debug_total
                        printf("Cause Xk'-1 > Xk-1,so now Xk");
                        check_feature_list(Xk);
#endif
                    }
                }
                else
                {
#ifdef debug_total
                    printf("Cause Xk'-1 < Xk-1,So now Xk");
                    check_feature_list(Xk);
#endif
                    free(Xk_miu_chg->membs);
                    free(Xk_miu_chg);
                }
                brk_fltp(Xk);
                free(Xk->membs);
                brk_fltp(Ym);
                free(Ym->membs);
                Xk=ini_Xk();
                Ym=ini_Ym(M);
                int i=0;
                for(i=0;i<Xk_chg->k;i++)
                {
                    add_node(Xk_chg->membs[i],Xk,M);
                    del_node(Xk_chg->membs[i],Ym,M);
                }
                free(Xk_chg->membs);
                free(Xk_chg);


            }
        }
        else
        {
            add_node(msw,Xk,M);
            del_node(msw,Ym,M);
#ifdef debug_total
            printf("Lsi equals the last one,and now Xk ");
            check_feature_list(Xk);
#endif
        }
        free(Xk_plus->membs);
        free(Xk_plus);

    }
    //    exit(1);
    return Xk;
}

int gmsw(fl_tp Xk,fl_tp Ym,mic_matrix M)
{
    int *lst_Xk=(int*)malloc((Xk->k+1)*sizeof(int));
    int i=0;
    if(Xk->k==0)
    {
        Xk->membs[0]=0;
    }
    for(i=0;i<Xk->k;i++)
    {

        lst_Xk[i]=Xk->membs[i];
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
fl_tp add_node(int atr,fl_tp Xk,mic_matrix M)
{
    int *Xk_membs=(int *)malloc(sizeof(int)*(Xk->k+1));
    int i=0;
    for(i=0;i<Xk->k;i++)
    {
        Xk_membs[i]=Xk->membs[i];
    }
    Xk_membs[Xk->k]=atr;
    int * tp=Xk->membs;
    Xk->membs=Xk_membs;
    free(tp);
    tp=NULL;
    Xk->k+=1;
    fet_tp msw=(fet_tp)malloc(sizeof(feature_node));
    msw->pn=NULL;
    msw->nn=NULL;
    msw->atr=atr;
    msw->sig=cal_merit(Xk->membs,Xk->k,M);
    Xk->sig=msw->sig;
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

#ifdef debug_addnode
    check_feature_list(Xk);
#endif
    return Xk;
}
fl_tp del_node(int atr,fl_tp Ym,mic_matrix M)
{
    int *Ym_membs=(int *)malloc(sizeof(int)*(Ym->k-1));
    int p=0;
    fet_tp cur=Ym->header;
    fet_tp msw=NULL;
    while(cur)
    {
        if(cur->atr==atr)
        {
            msw=cur;
        }
        else
        {
            Ym_membs[p++]=cur->atr;
        }
        cur=cur->nn;
    }
    int *tpm=Ym->membs;
    Ym->membs=Ym_membs;
    free(tpm);
    tpm=NULL;
    Ym->k-=1;
    Ym->sig=cal_merit(Ym->membs,Ym->k,M);
    if(msw==NULL)
    {
        puts("Not found the msw .This error is in add_node ");
    }
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
    free(msw);
    msw=NULL;
    return Ym;
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
fl_tp lst_rplc(fl_tp *original,fl_tp *target)
{
    fl_tp tpn=NULL;
    //    printf("beofre o:%p,t:%p\n",original,target);
    tpn=*target;
    *target=*original;
    *original=tpn;
    //    printf("after o:%p,t:%p\n",original,target);
    return *original;
}

void brk_fltp(fl_tp Xk)
{
    fet_tp cur=Xk->header;
    fet_tp tp;
    while(cur)
    {
        tp=cur;
        cur=cur->nn;
        free(tp);
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
            printf("atr:%d\tsig:%f\n",cur->atr,cur->sig);
            cur=cur->nn;
        }

    }
    else
    {
        printf("Virtual one.\n");
    }

    //    puts("Feature list check over !");
}
void update_Xk_merit(fl_tp Xk,mic_matrix M)
{
    int i=0;
    fet_tp cur=Xk->header;
    for(i=0;i<Xk->k;i++)
    {
        cur->sig=cal_merit(Xk->membs,i,M);
        cur=cur->nn;
    }
}

