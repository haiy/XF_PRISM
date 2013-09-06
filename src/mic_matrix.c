#include"mic_matrix.h"
void fill_mic_matrix(char *fnm,mic_matrix *M,float exp,float f)
{
	M->atrn=count_atr(fnm);
	M->class_idx=M->atrn-1;
#ifdef debug_fill_mic
	printf("atrn:%d\tclass_idx:%d\n",M->atrn,M->class_idx);
#endif
	M->mic=read_mic_matrix(fnm);
}

/*
从mic存储文件中读入mic矩阵
Tue Apr 23 15:01:18 CST 2013
*/
float ** read_mic_matrix(char *fnm)
{
	int atrn=count_atr(fnm);
	float **mic_matrix=(float **)malloc(sizeof(float*)*(atrn));
    if(mic_matrix==NULL)
    {
        printf("Memory error mic_matrix!\n");
    }
	int i=0;
	int j=0;
	mic_matrix[0]=NULL;
	for(i=1;i<=atrn-1;i++)
	{
		mic_matrix[i]=(float *)malloc(sizeof(float)*(i));
        if(mic_matrix[i]==NULL)
        {
            printf("Memory error mic_matrix!\n");
        }
		for(j=0;j<i;j++)
		{
            scanf("%f",&mic_matrix[i][j]);
		}
	}
    return mic_matrix;
}

float ** cal_mic_matrix(char *fnm,float alpha,float f)
{
	fct file_content;
	file_content.fn=fnm;
	read_content(&file_content);
	Points * D=(Points *)malloc(sizeof(Points));
	int atrn=count_atr(fnm);
	D->len=count_inst(fnm);
	D->x=(data_type *)malloc(sizeof(data_type)*D->len);
	D->y=(data_type *)malloc(sizeof(data_type)*D->len);
	D->grid=(label_type *)malloc(sizeof(label_type)*D->len);
	float **mic_matrix=(float **)malloc(sizeof(float*)*(atrn));
	int i=0;
	int j=0;
	mic_matrix[0]=NULL;
	for(i=1;i<=atrn-1;i++)
	{
		mic_matrix[i]=(float *)malloc(sizeof(float)*(i));
		for(j=0;j<i;j++)
		{
			mic_matrix[i][j]=mic_ct_mem(file_content,D,i,j,alpha,f);
		}
	}
	free(D->grid);
	free(D->x);
	free(D->y);
	free(D);
	for(i=0;i<atrn;i++)
	{
		free(file_content.ct[i]);
	}
	free(file_content.ct);
	return mic_matrix;
}

/*calculate the merit of the  current feature list*/
float cal_merit(int *lst,int l,mic_matrix M)
{ 
	int i=0;
	int j=0;
	float rfc=0.00;
	float rff=0.00;
#ifdef debug_cal_merit
	printf("Debug cal_merit with %d members.\n",l);
	//check_arry(lst,l);
	printf("merit1:%f\tmerit2:%f\tlst_i[%d]:%d,lst_j[%d]:%d\n",M.mic[M.class_idx][lst[i] ],M.mic[M.class_idx][lst[j] ],i,lst[i],j,lst[j] );
#endif
	if(l==0)
	{
		return 0;
	}
    if(l==1)
    {
        return M.mic[M.class_idx][lst[i]];
    }
    /* 计算方式，还是按照和mic值计算时相似的思路，只计算下三角矩阵 */
	for(i=1;i<l;i++)
	{
		rfc+=M.mic[ M.class_idx ][ lst[i] ];
#ifdef debug_cal_merit_deep
		printf("lst[%d]:%d,rfc,mic:%f\n",i,lst[i],M.mic[ MAX(lst[i],M.class_idx) ][ MIN(lst[i],M.class_idx) ]);
#endif
		for(j=0;j<i;j++)
		{
#ifdef debug_cal_merit_deep
				printf("cal_merit->lst[%d]:%d\tlst[%d]:%d\n",i,lst[i],j,lst[j] );
#endif
                if (lst[i]>lst[j])
                {
                    rff+=M.mic[ lst[i] ][ lst[j] ];
                }
                else
                {
                    rff+=M.mic[ lst[j] ][ lst[i] ];
                }
		}
	}
    rfc=(float)(rfc/l);
    rff=(float)(rff/(float)(l*(l-1)));
	float ret=(float)(rfc*l)/sqrt(l+l*(l-1)*(rff));
#ifdef debug_cal_merit
	printf("final rff :%f,rfc :%f, ret:%f\n",rff,rfc,ret);
#endif
	return ret;
}

void check_mic_matrix(mic_matrix M)
{
	int i=0;
	int j=0;
	puts("Begin mic_matrix check:");
	printf("class_idx:%d\tatrn:%d\n",M.class_idx,M.atrn);
	for(i=0;i<M.atrn;i++)
	{
		for(j=0;j<i;j++)
		{
			printf("%d:%d->%f\t",i,j,M.mic[i][j]);
		}
		puts("");
	}
	puts("mic_matrix check over!");

}
void check_arry(int * l,int a)
{
	int i=0;
	while(i<a)
	{
		printf("%d\t",l[i++]);
	}
	puts("Check OVER!!");
}
