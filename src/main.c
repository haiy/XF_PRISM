/*
Author:haiyfu
Date:Tue Apr 2 09:57:34 CST 2013
log:change the M.class_idx
Date:Apr 3-Apr 8
log:
1.修改了均分x轴时计算clump的返回值加1所导致的内存错误
2.修改mic_matrix计算函数，修改为先将所有内容读入内存存储，
然后计算的模式，而之前的每次计算只读取特定列的不再用。
3.修改参数输入部分不在由程序内定而是输入
Thu Apr 11 18:52:05 CST 2013
修改mic大于1的值为0
4.增加了对mic的参数支持 Tue Apr 16 21:15:08 CST 2013
5.修改了GA函数，增加关键函数update_pop

Tue Apr 23 14:58:10 CST 2013
6.修改mic值输入部分，这时候实现将所有的输入文件放入一个mic
文档，只需计算一次mic值，就能实现多次筛选
Wed Apr 24 14:15:50 CST 2013
7.错误为由mic.h中MAX和MIN宏定义引起。同时改正了
cal_merit函数的循环判断条件。
*/
#include"main.h"
/*  参数1为输入文件名字
    参数2为exp
    参数3为f
    导入含有mic值的以及参数的文件
*/
int main(int argc,char *argv[])
{
    mic_matrix M;
    char *fnm=argv[1];
    if(argc<3)
    {
        printf("Parameters Error!");
        exit(1);
    }
    fill_mic_matrix(fnm,&M,atof(argv[2]),atof(argv[3]));

    printf("GA find ");
    int pop_size;
    int loop_time;
    float pc;
    float pm;
    scanf("%d%d%f%f",&pop_size,&loop_time,&pc,&pm);
    rfl_ptr gabst=gafs(M,pop_size,loop_time,pc,pm);
    chk_bst(*gabst);

    free(gabst->atrs);
    free(gabst);

    printf("SFFS find ");
    int d;
    /* d is the size of target feature set */
    scanf("%d",&d);

    rfl_ptr sfbst=sffs_rf(M,d);
    chk_bst(*sfbst);
    free(sfbst->atrs);
    free(sfbst);

    printf("FCFS find ");
    float threshold;
    scanf("%f",&threshold);

    rfl_ptr fcbst=fcfs_rf(M,threshold);
    chk_bst(*fcbst);
    free(fcbst->atrs);
    free(fcbst);
//    puts("");
//    check_mic_matrix(M);

    prt_bst(*gabst,M);
    prt_bst(*sfbst,M);
    prt_bst(*fcbst,M);

    brk_bst(&gabst);
    brk_bst(&sfbst);
    brk_bst(&fcbst);
    return 1;
}

void chk_bst(resfts bst)
{
    printf("%d features with sig %f .\n",bst.numb,bst.sig);
    int i=0;
    do
    {
        printf("%d:%f,",bst.atrs[i].atr,bst.atrs[i].mic);
    }while(++i<bst.numb);
    puts("");
}
void prt_bst(resfts bst,mic_matrix M) 
{
    int i=0;
    puts("");
    for(i=0;i<bst.numb*(10+4+1);i++)
    {
        printf("%s","-");
    }
    puts("");
    for(i=0;i<bst.numb;i++)
    {
        printf("%10d,\t",bst.atrs[i].atr);
    }
    puts("");
    for(i=0;i<bst.numb*(10+4+1);i++)
    {
        printf("%s","-");
    }
    puts("");
    for(i=0;i<bst.numb;i++)
    {
        int j=0;
        for(j=0;j<=i;j++)
        {
            if(i==j)
            {
                printf("%10d,\t",1);
            }
            else
            {
                int a=bst.atrs[i].atr;
                int b=bst.atrs[j].atr;
                if(a>b)
                printf("%10.5f,\t",M.mic[ a ][ b ]);
                else
                printf("%10.5f,\t",M.mic[ b ][ a ]);

            }
        }
        puts("");
    }
    for(i=0;i<bst.numb*(10+4+1);i++)
    {
        printf("%s","-");
    }
    puts("");
}
void brk_bst(rfl_ptr * bst)
{
    free((*bst)->atrs);
    free(*bst);
}

//MIC-GA 
rfl_ptr gafs(mic_matrix M,int psz,int n,float pc,float pm)
{
    chro_ptr bst=ga(M,psz,n,pc,pm);
    int **gtp=(int **)malloc(sizeof(int *));
    if(gtp==NULL)
    {
        puts("GA gtp memory malloc error!");
        exit(1);
    }
    int num=gentype(bst->chro,gtp,bst->l);

    rfl_ptr gabst=(rfl_ptr)malloc(sizeof(resfts));
    gabst->numb=num;
    gabst->sig=bst->sig;
    gabst->atrs=(ftp_ptr)malloc(sizeof(featyp)*(gabst->numb));
    if(gabst->atrs==NULL)
    {
        puts("GA gabst memory malloc error!");
        exit(1);
    }
    int i=0;
    for(i=0;i< gabst->numb;i++)
    {
        int f=(*gtp)[i];
        gabst->atrs[i].atr=f;
        if(f>M.class_idx)
        gabst->atrs[i].mic= M.mic[f][ M.class_idx ];
        else
        gabst->atrs[i].mic= M.mic[ M.class_idx ][f];
    }
    free(bst->chro);
    free(bst);
    free(*gtp);
    free(gtp);
    return gabst;
}
//MIC-SFFS 
rfl_ptr sffs_rf(mic_matrix M,int n)
{
    fl_tp Xk=sffs(M,n);
    rfl_ptr sfbst=(rfl_ptr)malloc(sizeof(resfts));
    sfbst->numb=Xk->k;
    sfbst->sig=Xk->sig;
    sfbst->atrs=(ftp_ptr)malloc(sizeof(featyp)*sfbst->numb);
    fet_tp cur=Xk->header;
    int i=0;
    while(cur)
    {
        int f=cur->atr;
        sfbst->atrs[i].atr=f;
        sfbst->atrs[i].mic=M.mic[ M.class_idx ][ f ];
        cur=cur->nn;
        i++;
    }
    brk_fltp(Xk);
    free(Xk->membs);
    free(Xk);
    return sfbst;
}
//MIC-FCBF
rfl_ptr fcfs_rf(mic_matrix M, float sa)
{
    list *result=fcfs(M,sa);
    rfl_ptr bst=(rfl_ptr)malloc(sizeof(resfts));
    bst->numb=result->len;
    bst->atrs=(ftp_ptr)malloc(sizeof(featyp)*bst->numb);
    list_node * cur=result->header;
    int *fs=(int *)malloc(sizeof(int)*(result->len));
    int i=0;
    while(cur)
    {
        int f=cur->idx;
        fs[i]=cur->idx;
        bst->atrs[i].atr=f;
        if (f > M.class_idx)
        bst->atrs[i].mic=M.mic[f][ M.class_idx ];
        else
        bst->atrs[i].mic=M.mic[ M.class_idx ][ f ];
        cur=cur->nn;
        i++;
    }
    bst->sig=cal_merit(fs,bst->numb,M);
    brk_l(result);
    free(result);
    return bst;
}
