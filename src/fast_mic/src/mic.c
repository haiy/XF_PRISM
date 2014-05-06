#include "mic.h"

/*Example usage*/
/*
int main(int argc, char *argv[])
{
//    float mic=opa(argc,argv);
//    printf("Final mic:%f\n",mic);
    cal_mic_1all(argv[1],atoi(argv[2]),atof(argv[3]),atoi(argv[4]));

}
*/
/* interface 1 of mic */
float opa (int argc,char *argv[] )
{       
    Points * D=(Points *)malloc(sizeof(Points));
    int atrn=count_atr(argv[1]);
    printf("attribures:%d\n",atrn);
    /*
       if(!ckpa(argc,argv,atrn))
       {
       puts("parameter error!");
       usage();
       exit(1);
       }
     */
    D->len=count_inst(argv[1]);
    printf("%d instances\n",D->len);
    D->x=(data_type *)malloc(sizeof(data_type)*D->len);
    D->y=(data_type *)malloc(sizeof(data_type)*D->len);
    D->grid=(label_type *)malloc(sizeof(label_type)*D->len);
    read_pair(argv[1],atoi(argv[2]),atoi(argv[3]),D->x,D->y);
    float alpha=atof(argv[4]);
    int f=atoi(argv[5]);
    float result=cal_mic(D,alpha,f);
    free(D->x);
    D->x=NULL;
    free(D->y);
    D->y=NULL;
    free(D->grid);
    D->grid=NULL;
    free(D);
    D=NULL;
    return result;
}

/* inteface 2 of mic */
float cal_mic_inter(char *filename,Points *D,int xa,int yb,float alpha,int f)
{
    read_pair(filename,xa,yb,D->x,D->y);
    if( !chk_sm(D))
    {
        return cal_mic(D,alpha,f);
    }
    else
    {
        return 0;
    }
}
/* inteface 3 of mic */
void cal_mic_1all(char *filename,int a,float alpha,int f)
{
    Points * D=(Points *)malloc(sizeof(Points));
    int atrn=count_atr(filename);
    printf("attribures:%d\n",atrn);
    D->len=count_inst(filename);
    printf("%d instances\n",D->len);
    D->x=(data_type *)malloc(sizeof(data_type)*D->len);
    D->y=(data_type *)malloc(sizeof(data_type)*D->len);
    D->grid=(label_type *)malloc(sizeof(label_type)*D->len);
    float result=0.00;
    int i=0;
    printf("alpha:%f\tf:%d\n",alpha,f);
    for(i=0;i<atrn;i++)
    {
        read_pair(filename,i,a,D->x,D->y);
        result=cal_mic(D,alpha,f); 
        if(EXACT(result)>EXACT(1))
        {
            //printf("Mic larger than 1.Error occured !\n");
            //exit(1);
        }
        printf("%d,%d,%f\n",i,a,result);
    }
    free(D->x);
    free(D->y);
    free(D);
}

/* check if all the values are the same one */
int chk_sm(Points * D)
{
    int i=0;
    while(D->x[i].val==D->x[i+1].val)
    {
        i++;
        if(i > (D->len-1))
        {
            return 1;
        }
    }

    i=0;
    while(D->y[i].val==D->y[i+1].val)
    {
        i++;
        if(i > (D->len-1))
        {
            return 1;
        }
    }

    return 0;
}

/* the kernel part to calculate mic */
float cal_mic(Points *D,float alpha,int f)
{
    data_type *Dx=(data_type *)malloc(sizeof(data_type)*D->len);
    data_type *Dy=(data_type *)malloc(sizeof(data_type)*D->len);

    fu(D->len,D->x,Dx);
    fu(D->len,D->y,Dy);

    q_sort(Dx,D->len);
    q_sort(Dy,D->len);
#ifdef check_data 
    vd ( D,Dy,D->len );
#endif

    int B=pow(D->len,alpha);
    int max_B=((float)B/2);
    int y=0;
    int x=0;
    float **I=(float **)malloc(sizeof(float *)*(max_B+1));
    int i=0;
    for(i=0;i<=max_B;i++)
    {
        I[i]=(float *)malloc(sizeof(float)*(max_B+1));
    }

    float **MI_xy=(float **)malloc(sizeof(float *)*(max_B+1));
    float **MI_yx=(float **)malloc(sizeof(float *)*(max_B+1));
    for(i=0;i<=max_B;i++)
    {
        MI_xy[i]=(float *)malloc(sizeof(float)*(max_B+1));
        MI_yx[i]=(float *)malloc(sizeof(float)*(max_B+1));
    }
#ifdef DEBUG_1
        printf("B:%d\tB/2:%d\tf:%d\talpha:%f\n",B,rnd((float)B/2),f,alpha);
#endif
    for(y=2;y<=(B/2);y++)
    {
        /* MI will store the mutual information on a fixed y
           and the clumps on x axis is begin from 2 to x */

        x=(B/y);
#ifdef DEBUG_1
        printf("%d\t%d\n",x,y);
#endif
        cal_MI(D,Dx,Dy,x,y,f,MI_xy);
        cal_MI(D,Dy,Dx,x,y,f,MI_yx);
    }

    float **MIC=(float **)malloc(sizeof(float * )*(max_B+1));
    for(i=0;i<=max_B;i++)
    {
        MIC[i]=(float *)malloc(sizeof(float)*(max_B+1));
    }
    float maxMIC=LOWEST;
    for(y=2;y<=max_B;y++)
    {
        for(x=2;x<=max_B;x++)
        {
            if(x*y<=B)
            {
                if(EXACT(MI_xy[x][y])>MI_yx[y][x])  I[x][y]=MI_xy[x][y];
                else I[x][y]=MI_yx[y][x];
                int minxy;
                if (x>y) minxy=y;
                else minxy=x;
                MIC[x][y]=(float)I[x][y]/log(minxy);
                if(EXACT(MIC[x][y])>EXACT(maxMIC))
                {
                    maxMIC=MIC[x][y];
                }
#ifdef DEBUG_1
                printf("%10f",MIC[x][y]);
#endif
            }
        }
#ifdef DEBUG_1
        printf("\n");
#endif
    }
    for(i=0;i<=max_B;i++)
    {
        free(I[i]);
        I[i]=NULL;
        free(MI_yx[i]);
        MI_yx[i]=NULL;
        free(MI_xy[i]);
        MI_xy[i]=NULL;
        free(MIC[i]);
        MIC[i]=NULL;
    }
    free(I);
    I=NULL;
    free(MI_xy);
    MI_xy=NULL;
    free(MI_yx);
    MI_yx=NULL;
    free(MIC);
    MIC=NULL;
    free(Dx);
    Dx=NULL;
    free(Dy);
    Dy=NULL;

    return maxMIC;
}

void cal_MI(Points *D,data_type * Dx,data_type * Dy,int x ,int y, int f,float **MI)
{
    /* equipartition y axis into y parts */ 
    eqpY (D, Dy,D->len,y);

    /*get almost y superclumps on x axis */
    /* super clump number */
    int scln=0;
    scln=eqpX (D,Dx,Dy,D->len,x,y,f);

    /*fill the cell of current grid resolution*/
    int **rs=(int **)malloc(sizeof(int *)*y);
    if(rs==NULL)
    {
        puts(" rs Memory malloc Error!Exit now.");
        exit(0);
    }
    
    int i=0;
    int j=0;
    for(i=0;i<y;i++)
    {
        rs[i]=(int *)malloc(sizeof(int)*(scln+1));
        for(j=0;j<scln+1;j++)
        {
            rs[i][j]=0;
        }
    }
    gdrs(D,rs);

    /* calculate the f values without HQ */
    Optx(x,y,scln,rs,MI);
    /* free memory for cell record */
    for(i=0;i<y;i++)
    {
#ifdef DEBUG_1
            printf("rs[%d][%d]:%d\t",i,j,rs[i][j]);
#endif
        free(rs[i]);
        rs[i]=NULL;
    }
    free(rs);
    rs=NULL;
}

/* find the very x partition can lead to maximum 
   mutual information */
void Optx(int x,int y, int clmn, int **rs, float **MI)
{
    float **Hpq=(float **)malloc((clmn+1)*sizeof(float *));
    float **Hst=(float **)malloc((clmn+1)*sizeof(float *));
    float **Ai=(float **)malloc((clmn+1)*sizeof(float *));
    float *Hq=(float *)malloc(sizeof(float));

    int i=0;
    for(i=0;i<=clmn;i++)
    {
        Hpq[i]=(float *)malloc((clmn+1)*sizeof(float));
        Hst[i]=(float *)malloc((clmn+1)*sizeof(float));
        Ai[i]=(float *)malloc((clmn+1)*sizeof(float));
        int j=0;
        for(j=0;j<=clmn;j++)
        {
            Hpq[i][j]=0;
            Hst[i][j]=0;
            Ai[i][j]=0;
        }
    }
#ifdef DEBUG_1
        printf("%d\t%d\t%d\n",x,y,clmn);
#endif

    hst(clmn,y,rs,Hpq,Hst,Ai,Hq);

    float *F=(float *)malloc(sizeof(float)*(clmn+1));
    for(i=0;i<=clmn;i++)
    {
        F[i]=0;
    }
    int t=0;
    int s=0;
    int k=clmn;
    float f=0.00;
    /* calculate all the f values of different l */
    float *tp=(float *)malloc(sizeof(float)*(k+1));
    int l=0;
#ifdef DEBUG_1
        printf("Normalized : [ ");
#endif
    for(l=2;l<=x;l++)
    {
        for(t=l;t<=k;t++)
        {
            float fmax=LOWEST;
            for(s=l-1;s<t;s++)
            {
                if(l>2)
                {
                    f=Ai[s][t]*F[s]+Hst[s][t];
                }
                else
                {
                    f=Hpq[s][t];
                }

                if(EXACT(f)>EXACT(fmax))
                {
                    fmax=f;
                }
            }
            tp[t]=fmax;

        }
        for(i=0;i<=k;i++)
        {
            F[i]=tp[i];
        }
        MI[l][y]=F[k]+(*Hq);
#ifdef DEBUG_1
            printf("%10f\t",MI[l][y]/log(MIN(l,y)));
#endif
    }

#ifdef DEBUG_1
        printf(" ]\n \n");
#endif
    /* memory free */

    for(i=0;i<=clmn;i++)
    {
        free(Hpq[i]);
        Hpq[i]=NULL;
        free(Hst[i]);
        Hst[i]=NULL;
        free(Ai[i]);
        Ai[i]=NULL;
    }
    free(Hpq);
    Hpq=NULL;
    free(Hst);
    Hst=NULL;
    free(Ai);
    Ai=NULL;
    free(Hq);
    Hq=NULL;
    free(F);
    F=NULL;
    free(tp);
    tp=NULL;
}

/* pre-compute all kinds of probabilities */
void hst(int clmn,int y,int **rs,float **Hpq,float **Hst,float **Ai,float *Hq)
{
    int i=0;
    int j=0;

    int *xq=(int *)malloc(sizeof(int)*clmn);
    for(i=0;i<clmn;i++)
    {
        xq[i]=0;
    }
    int c=0;
    int sum=0;
    for(i=0,c=0;i<clmn;i++)
    {
        for(j=0;j<y;j++)
        {
            /* the sum number of points */
            sum+=rs[j][i];
        }
        xq[c++]=sum;
    }

    int ** yp=(int **)malloc(sizeof(int *)*clmn);
    for(i=0;i<clmn;i++)
    {
        yp[i]=(int *)malloc(sizeof(int)*y);
        for(j=0;j<y;j++)
        {
            yp[i][j]=0;
        }
    }
#ifdef DEBUG_1
        printf("sum:%d\ny axis partition: [ ",sum);
#endif
    for(j=0;j<y;j++)
    {

        int sum=0;
        for(i=0;i<clmn;i++)
        {

            sum+=rs[j][i];
            yp[i][j]=sum;

        }
#ifdef DEBUG_1
            printf("%d->%d\t",j,yp[clmn-1][j]);
#endif

    }
#ifdef DEBUG_1
        printf(" ] \n");
#endif

    *Hq=0.00;
    for(i=0;i<y;i++)
    { 
        int sti=yp[clmn-1][i];
        int st=xq[clmn-1];
        float p1=(float)sti/st;
        if(p1)
        {
            (*Hq)-=(float)p1*log(p1);
        }
    }

    int t=0;
    int s=0;
    int k=clmn;
    for(t=2;t<=k;t++)
    {
        for(s=1;s<t;s++)
        {
            for(i=0;i<y;i++)
            {
                /*test F l=2*/
                int m=xq[t-1];
                int jl=xq[s-1];
                int jr=xq[t-1]-xq[s-1];
                int c1=yp[s-1][i];
                int c2=yp[t-1][i]-yp[s-1][i];

                float p1=(float)c1/m;
                float p2=(float)c2/m;
                float p3=(float)c1/jl;
                float p4=(float)c2/jr;
                if(c1>0)
                {
                    Hpq[s][t]+=(float)p1*log(p3);
                }
                if(c2>0)
                {
                    Hpq[s][t]+=(float)p2*log(p4);
                }

                int sl=xq[t-1]-xq[s-1];
                int il=yp[t-1][i]-yp[s-1][i];
                p1=(float)sl/m;
                p2=(float)il/sl;
                if(il>0)
                {
                    Hst[s][t]+=p2*log(p2);
                }
            }
            int m=xq[t-1];
            int sl1=xq[s-1];
            int sl=xq[t-1]-xq[s-1];
            float p=(float)sl/m;
            Hst[s][t]=(float)Hst[s][t]*p;
            Ai[s][t]=(float)sl1/m;
        }
    }
#ifdef DEBUG_1
        printf("HQ:%f\n",*Hq);
#endif
    for(i=0;i<clmn;i++)
    {
        free(yp[i]);
        yp[i]=NULL;
    }
    free(yp);
    yp=NULL;
    free(xq);
    xq=NULL;
}

/* grid resoultuion ,number of points in a cell */
void gdrs(Points *D ,int ** rs ) 
{
    int i=0;
    for(i=0;i< D->len;i++)
    {
        rs[D->grid[i].rn][D->grid[i].cn]+=1;
#ifdef DEBUG_1
            printf("i:%d\trn:%d\tcn:%d\trs:%d\n",i,D->grid[i].rn,D->grid[i].cn,rs[D->grid[i].rn][D->grid[i].cn]);
#endif
    }

}

/* get super clumps  */
int eqpX (Points *D,data_type * Dx, data_type *Dy, int l, int x, int y, int f)
{

    int i=0;
    int clmn=f*x;
    float scz=((float)l/clmn);
    int c=0;
    int sci=0;

#ifdef DEBUG_1
        printf("Desired size:%f\n",scz);
        printf("Clumps partition: [ ");
#endif
    while(i<l)
    {       
        int t=0;
        int sc=1;       
//        float T=(Dx[i].val);
        D->grid[Dx[i].pos].cn=c;
        int SMR=1;
        int SMX=1;
        for(t=1;i+t<l;t++)
        {
            if(!smr(D,Dx,i,i+t))
            {
                SMR=0;
            }
            if(!smx(Dx,i,i+t))
            {
                if(!SMR)
                {
                    break;
                }
                SMX=0;
            }
            if(!SMX&&!smr(D,Dx,i,i+t))
            {
                break;
            }
        }
        sc=t;
#ifdef eqpx
        printf("now t :%d,i+t:%d\n",t,i+t);
#endif 
        /* whether to cut down the current clump */
        if(!SMX&&Dx[i+t-1].val==Dx[i+t].val)
        {
            int j=0;
            for(j=i+t-1;j>i;)
            {
                if((Dx[i+t-1].val)==(Dx[j].val))
                {
                    j-=1;
                    sc-=1;
                }
                else
                {
                    break;
                }
            }
        }
#ifdef eqpx
        printf("after cutting sc:%d,i+sc:%d,val:%f\n",sc,i+sc,Dx[i+sc].val);
#endif 
        /*update grid's cloumn */
        int idx=1;
        while(idx<=sc)
        {
            D->grid[Dx[i+idx-1].pos].cn=c;
            idx+=1;
        }
        sci+=sc;
        if((fabs(sci+sc-scz)>=fabs(sci-scz))&&(sci))
        {
            sci=0;
            if(clmn-c-1==0)
            {
                scz=l;
            }
            else scz=(float)((l-i)/(clmn-c-1));
            c++;
        }
        i+=sc;
    }
#ifdef DEBUG_1
        printf("%d ]\n",i-1);   
#endif
    return (c);
}

/* check if all the points in one clump are in the same row */
int smr (Points *D,data_type *sdx,int a,int b)
{
    if(D->grid[sdx[a].pos].rn!=D->grid[sdx[b].pos].rn)
    {
        return 0;
    }
    return 1;

}
int smx (data_type *sdx,int a,int b)
{
    if(sdx[a].val==sdx[b].val)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
/* eqpY : equipartition the y axis      */
void eqpY (Points *D, data_type * Dy ,int l, int y )
{
    int i=0;
    int s=0;
    int c=0;
    int bi=0;
    float bsz=(float)l/y;
    int np=0;
#ifdef DEBUG_eqpy
    printf("Initial bsz:%f\n",bsz);
#endif
    while(i<l)
    {
        s=0;
        for(s=0;s+i<l;)
        {
            if(Dy[i].val==Dy[i+s].val)
            {
                s+=1;
            }
            else
                break;
        }
#ifdef DEBUG_eqpy
        printf("bi:%d,s:%d,bsz:%f,abs(bi+s-bsz):%f\n",bi,s,bsz,fabs(bi+s-bsz));
        printf("bi:%d,bsz:%f,abs(bi-bsz):%f",bi,bsz,fabs(bi-bsz));
#endif
        if(fabs(bi+s-bsz)>=fabs(bi-bsz)&&bi)
        {
            bi=0;
            c++;
            bsz=(float)(l-np)/(y-c);
#ifdef DEBUG_eqpy
            printf("break point:c:%d\tbsz:%f\ti:%dnp:%d,s:%d,val:%f\n",c,bsz,i,np,s,Dy[i].val);
#endif
        }
        int k=0;
        for(k=0;k<s;k++)
        {
            D->grid[Dy[i+k].pos].rn=c;
        }
        np+=s;
        bi+=s;
        i+=s;
    }
#ifdef DEBUG_eqpy
    i=0;
    while(i<l)
    {
        printf("i:%d\ty:%f\trn:%d\n",i,Dy[i].val,D->grid[Dy[i].pos].rn);
        i+=1;
    }
#endif
}

/* check the y axis partition */
void vyp ( int y , int * yp )
{
    int i=0;
    printf("Index\tPosition\tNumber\n");
    for(i=0;i<y;i++)
    {
        printf("%d\t%d\t",i,yp[i]);
        if(i>0)
        {
            printf("%d\n",yp[i]-yp[i-1]);           
        }
        else
        {
            printf("%d\n",yp[i]+1);
        }
    }

}

/* generate a test data set*/
void gd (Points * D )
{
    int i=0;
    while(i < D->len)
    {
        D->x[i].pos=i;
        D->y[i].pos=i;

        D->x[i].val=i*PI/2.00;
        D->y[i].val=sin(D->x[i].val);

        D->x[i].val=EXACT(D->x[i].val);
        D->y[i].val=EXACT(D->y[i].val);
        if(Debug>LEVEL_1)
        {
            printf("%d:\t%f\t%f\n",i,D->x[i].val,D->y[i].val);
        }
        i++;
    }
    /*
       for(j=1,i=0;i<=23;i++,j++)
       {
       D->y[i].val=0;
       if(Debug>LEVEL_1)
       {
       printf("%d->%d:\t%f\t%f\n",j,i,D->x[i].val,D->y[i].val);
       }
       }
     */
    if(CHECK_DATA==1)
    {
        i=0;
        printf("Index,Value\n");
        for(i=0;i<D->len;i++)
        {
            printf("%f,%f\n",D->x[i].val,D->y[i].val);
        }
    }
}

/*  fill DES_DB with SRC_DA*/
void fu (int len ,data_type *SRC_DA,data_type *DES_DB )
{
    int i=0;
    for(i=0;i<len;i++)
    {
        DES_DB[i].val=SRC_DA[i].val;
        DES_DB[i].pos=SRC_DA[i].pos;
    }
}

/* view the data x or y  */
void vd (Points *D,data_type * sdx, int l)
{
    int i=0;
    puts("\n");
    for(i=0;i<l;i++)
    {
        int flag=0;
        if(i+1<l)
        {

            if(sdx[i].val==sdx[i+1].val)
            {
                flag=1;
            }
        }
        printf("%d\t%lf\t%lf\t%d\n",i,sdx[i].val,D->x[sdx[i].pos].val,flag);
    }
}

/* round the float number  */
int rnd ( float x )
{
    return floor((x*10+4)/10.00);
}

/* quick sort an array 
   q_sort , qs , p_qs, swap */
void q_sort ( data_type * sd, int l )
{
    int     left=0;
    int right=l-1;
    qs(sd ,left , right );

}
void qs (data_type *sd ,int left , int right )
{
    if(left<right)
    {
        int m=p_qs(sd,left,right);
        qs(sd,left,m-1);
        qs(sd,m+1,right);
    }

}
int p_qs (data_type *sd,int left,int right)
{
    double t=sd[left].val;
    int b=left,i;
    for(i=left;i<=right;i++)
    {
        if(sd[i].val<t)
        {
            b++;
            swap(sd,b,i);

        }
    }
    swap(sd,b,left);
    return b;
}
void swap (data_type * sd,int a,int b)
{
    data_type temp=sd[a];
    sd[a]=sd[b];
    sd[b]=temp;
}

/* read a pair of data from the source file */
int read_pair(char *filename,int xa,int yb,data_type *x,data_type *y)
{
    FILE * src=fopen(filename,"r");
    if(!src)
    {
        printf("File %s open error!\n",filename);
        exit(1);
    }

    rp(src,xa,yb,x,y);

    fclose(src);
    return 0;
}

/* count the how many lines in the file */
int count_inst(char * filename)
{
    FILE * src= fopen(filename,"r");
    if(!src)
    {
        printf("File %s open error!\n",filename);
        usage();
        exit(1);
    }

    int inst=0;
    char *tmp=(char *)malloc(sizeof(char)*bufsize);
    fseek(src,0,SEEK_SET);
    fgets(tmp,bufsize,src);
    while(fgets(tmp,bufsize,src))
    {
        ++inst;
    }
    fclose(src);
    return inst;
}

/*count the number of attributes*/
int count_atr(char *filename)
{
    FILE * src =fopen(filename,"r");
    char *l=(char *)malloc(sizeof(char)*bufsize);

    if(!src)
    {
        puts("File open error!");
        usage();
        exit(1);
    }
    int atrn = 0;
    fgets(l,bufsize,src);
    if(fgets(l,bufsize,src))
    {
        char *tp=l;
        atrn=0;
        while(!(*tp=='\0'||*tp=='\n'))
        {
            while(!isdigit(*tp)&&!(*tp=='\0'||*tp=='\n'))
            {
                tp++;

            }
            if(*tp=='\0'||*tp=='\n')
            {
                free(l);
                fclose(src);
                return atrn;
            }
            atrn++;
            strtod(tp,&tp);
            tp++;
        }
    }
    free(l);
    fclose(src);
    return atrn;
}

/* child function of read data */
void rp(FILE * src,int xa ,int yb, data_type *x ,data_type *y)
{
    int i=0;
    int j=0;
    char *lc=(char *)malloc(sizeof(char)*bufsize);
    char *tp;
    float tpn;
    tpn=0.00;

    fseek(src,0,SEEK_SET);
    fgets(lc,bufsize,src);
    while(fgets(lc,bufsize,src))
    {     
        i=0;
        tp=lc;
        while(*tp!='\0'&&*tp!='\n')
        {
            tpn=strtod(tp,&tp);
            if(i==xa)
            {
                x[j].val=tpn;
                x[j].pos=j;
            }
            if(i==yb)
            {
                y[j].val=tpn;
                y[j].pos=j;
            }
            tp++;
            i++;
        }
        j++;
    }
    free(lc);
    lc=NULL;
}

int ckpa(int argc,char *argv[],int atrn)
{
    if(ck_n(argc,7)&&ck_f(argv[2])&&ck_i(atoi(argv[3]),atoi(argv[4]),atrn)&&ck_e(atof(argv[5])) )
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

