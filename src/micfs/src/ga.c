#include "ga.h"
/*
int main()
{
    mic_matrix M;
    char *fnm="corral.csv";
    fill_mic_matrix(fnm,&M);
    ga(M);
    return 1;
}
*/
/*
psz the population size
n the generations number to run
pc the cross over probability
pm the mutation probability
*/
chro_ptr ga(mic_matrix M,int psz,int n,float pc,float pm)
{
    srand((double)time(NULL));
    population pop;
    pop.n=psz;
    pop.acu=(float*)malloc(sizeof(float)*pop.n);
    if(pop.acu==NULL)
    {
        puts("GA acu memory error!");
        exit(1);
    }
    int i=0;
    for(i=0;i<pop.n;i++)
    {
        pop.acu[i]=0;
    }
    pop.pc=pc;
    pop.pm=pm;
    ini_pop(M,&pop);
    population *S=ini_tmpop(pop);
    int T=pop.n;
    int Gn=0;
    int N=n; 
    do
    {//代数不够或者没达到最优解
//#define check_ga
#ifdef check_ga
        printf("The %d generation's best %f.\n",Gn,pop.m[0].sig);
#endif
        int n=0;
        cal_acu(&pop);
        do
        {//种群的成员数不够
            chro_ptr father=sel_one(pop);
            chro_ptr mother=sel_one(pop);
            cpy_chro(father,&(S->m[n]));
            cpy_chro(mother,&(S->m[n+1]));
            if(CY(pop))
            {
                cross_over(&(S->m[n]),&(S->m[n+1]));
            }
            if(MT(pop))
            {
                mutation(&(S->m[n]));
            }
            if(MT(pop))
            {
                mutation(&(S->m[n+1]));
            }
            n+=2;
        }while(n<T);
        update_pop(S,M);
        elitist_sel(&pop,S);
    }while(++Gn<N);
    chro_ptr bst=(chro_ptr)malloc(sizeof(chrosome));
    bst->l=pop.m[0].l;
    bst->chro=(int *)malloc(sizeof(int)*(bst->l));
    cpy_chro(&(pop.m[0]),bst);
    brk_pop(&pop);
    brk_pop(S);
    return bst;
}

//initialize the population
void ini_pop(mic_matrix M,population *pop)
{
    pop->m=(chro_ptr)malloc(sizeof(chrosome)*(pop->n));
    if(pop->m==NULL)
    {
        printf("Ini_pop memory error!\n");
    }
    int i=0;
    while(i < pop->n)
    {
        /* Here pop->m[i].l denotes the length of the chrosome ,染色体长度不应该包含类标签*/
        pop->m[i].l=M.atrn-1;
        pop->m[i].chro=(int *)malloc(sizeof(int)*(pop->m[i].l));
        if(pop->m[i].chro==NULL)
        {
            printf("GA ini_pop mem error!\n");
            exit(0);
        }
        int j=0;
        for(;j<pop->m[i].l;j++)
        {
            pop->m[i].chro[j]=rand()%2;
        }
        int **gtp=(int **)malloc(sizeof(int *));
        if(gtp==NULL)
        {
            puts("GA gtp mem error!");
            exit(1);
        }
        int num=gentype(pop->m[i].chro,gtp,pop->m[i].l);
        pop->m[i].sig=cal_merit(gtp[0],num,M);
        free(*gtp);
        free(gtp);
        gtp=NULL;
     
        i++;
    };
}

//initialize a temporary population
population *ini_tmpop(population pop)
{
    population *s=(population *)malloc(sizeof(population));
    s->n=pop.n;
    s->acu=NULL;
    s->pc=pop.pc;
    s->pm=pop.pm;
    s->m=(chro_ptr)malloc(sizeof(chrosome)*(pop.n));
    int i=0;
    for(i=0;i<pop.n;i++)
    {
        s->m[i].l=pop.m[i].l;
        s->m[i].sig=0.00;
        s->m[i].chro=(int *)malloc(sizeof(int)*(s->m[i].l));
        int j=0;
        for(j=0;j<s->m[i].l;j++)
        {
            s->m[i].chro[j]=0;
        }
    }
    return s;
}
//change bincode into genetype
int gentype(int *bincode,int ** gtp,int ln)
{
    int i=0;
    int k=0;

    while(i<ln)
    {
        if(bincode[i]==1)
        {
            k++;
        }
        i++;
    }
    gtp[0]=(int *)malloc(sizeof(int)*k);
    if(gtp[0]==NULL)
    {
        puts("gentype gtp mem error!");
        exit(1);
    }

    i=0;
    int j=0;
    while(i<ln)
    {
        if(bincode[i]==1)
        {
            gtp[0][j++]=i;
        }
        i++;
    }
    return k;
}

//calculate the acculumated probability
void cal_acu(population *pop)
{
    int i=0;
    float sum=0.00;
    pop->acu=(float *)malloc(sizeof(float)*pop->n);
    for(i=0;i<pop->n;i++)
    {
        sum+=pop->m[i].sig;
    }
    pop->acu[0]=pop->m[0].sig/sum;
    for(i=1;i<pop->n;i++)
    {
        pop->acu[i]=(float)pop->m[i].sig/sum+pop->acu[i-1];
#ifdef debug_acu
        printf("%f\n",pop->acu[i]);
#endif
    }
}

//selection operator
chro_ptr sel_one(population pop)
{
    float r=rand()%100000/(float)100000;
    if(EXACT(r)<=EXACT(pop.acu[0]))
    {
#ifdef debug_sel_one
        printf("r:%f is between 0 and %f\n",r,pop.acu[0]);
#endif
        return &(pop.m[0]);
    }
    int i=1;
    while(i<pop.n)
    {
        if(r<=pop.acu[i]&&r>=pop.acu[i-1])
        {
#ifdef debug_sel_one
        printf("r:%f is between acu[%d]->%f and acu[%d]->%f\n",r,i-1,pop.acu[i-1],i,pop.acu[i]);
#endif
            return &(pop.m[i]);
        }
        else
        {
            i++;
        }
    }
    return NULL;
}
void cpy_chro(chro_ptr src,chro_ptr dst)
{
    int i=0;
    dst->l=src->l;
    dst->sig=src->sig;
    for(i=0;i<src->l;i++)
    {
        dst->chro[i]=src->chro[i];
    }
}

int CY(population pop)
{
    float p=rand()%100000/(float)100000.00;
    if(p<pop.pc&&p>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int MT(population pop)
{
    float p=rand()%100000/(float)100000.00;
    if(p<pop.pm&&p>0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//cross over the chrosomes
void cross_over(chro_ptr father,chro_ptr mother)
{
    int r=rand()%(father->l);
#ifdef debug_crx
    printf("cross position :%d\n",r);
#endif
    int i=0;
    for(i=r;i<father->l;i++)
    {
        int a=father->chro[i];
        father->chro[i]=mother->chro[i];
        mother->chro[i]=a;
    }
    r=rand()%(father->l);
#ifdef debug_crx
    printf("cross position :%d\n",r);
#endif
    for(i=r;i<father->l;i++)
    {
        int a=father->chro[i];
        father->chro[i]=mother->chro[i];
        mother->chro[i]=a;
    }
}

//mutate the chrosome
void mutation(chro_ptr chros)
{
    int r=rand()%(chros->l);
    int l=rand()%(chros->l);
#ifdef debug_mt
    printf("mutation position:r:%d,l:%d,chol:%d\n",r,l,chros->l);
#endif
    int i=0;
    int stp=0;
    if( (r+l) > chros->l) stp=chros->l;
    else    stp=(r+l);
#ifdef debug_mt
    printf("stp:%d\n",stp);
#endif
    for(i=r;i<stp;i++)
    {
        chros->chro[i]=(chros->chro[i]+1)%2;
    }
}

population *elitist_sel(population *pop,population *S)
{
    qksort(pop);
    qksort(S);
#ifdef debug_elt
    puts("The pop is:\t");
    check_pop(*pop);
    puts("The S is :\t");
    check_pop(*S);
#endif
    int i=0;
    int p=0;
    int q=0;
    for(i=0;i<pop->n;i++)
    {
        if(EXACT(pop->m[p].sig)>EXACT(S->m[q].sig))
        {
            p++;
        }
        else
        {
            chg_chro_PS(pop,i,S,q);
            q++;
        }
    }
#ifdef debug_elt
    puts("After merging...\t");
    check_pop(*pop);
#endif
    return pop;
}

void qksort(population *pop)
{
    qsp(0,pop->n-1,pop);
}
void qsp(int a,int b,population *pop)
{
    if(a<b)
    {
        int mid=partp(a,b,pop);
        qsp(a,mid-1,pop);
        qsp(mid+1,b,pop);
    }
}
int partp(int a,int b,population *pop)
{
    float lb=pop->m[a].sig;
    int k=a+1;
    int i=a+1;
    while(i<=b)
    {
        if(pop->m[i].sig>lb)
        {
            chg_chro(pop,i,k);
            k++;
        }
        i++;
    }
    chg_chro(pop,a,k-1);
    return k-1;
}

//change chrosomes
void chg_chro(population *pop,int a,int b)
{
    int * tp=pop->m[a].chro;
    pop->m[a].chro=pop->m[b].chro;
    pop->m[b].chro=tp;
    float t=pop->m[a].sig;
    pop->m[a].sig=pop->m[b].sig;
    pop->m[b].sig=t;
}
void chg_chro_PS(population *pop,int a,population *S,int b)
{
    int * tp=pop->m[a].chro;
    pop->m[a].chro=S->m[b].chro;
    S->m[b].chro=tp;
    float t=pop->m[a].sig;
    pop->m[a].sig=S->m[b].sig;
    S->m[b].sig=t;
}

//break down the population
void brk_pop(population *pop)
{
    int i=0;
    while(i< pop->n)
    {
        free(pop->m[i].chro);
        pop->m[i].chro=NULL;
        i++;
    }
}

void update_pop(population *S,mic_matrix M)
{
    int i=0;
    for(i=0;i<S->n;i++)
    {
        int **gtp=(int **)malloc(sizeof(int *));
        int num=gentype(S->m[i].chro,gtp,S->m[i].l);
        S->m[i].sig=cal_merit(*gtp,num,M);
        free(*gtp);
        free(gtp);
        gtp=NULL;
    }
}

