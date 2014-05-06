#include"mic_matrix.h"
void fill_mic_matrix(char *fnm,mic_matrix *M,float exp,float f)
{
    M->atrn=count_atr(fnm);
    M->class_idx=M->atrn-1;
#ifdef debug_fill_mic
    printf("atrn:%d\tclass_idx:%d\n",M->atrn,M->class_idx);
#endif
    M->mic=cal_mic_matrix(fnm,exp,f);
}

/* calculate all the pairs of mic values */
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
    puts("");
}

/* calculate all the mic values betweem the first column and 
   the target column ,the start and the end columns are both
   included.
 */
float **cal_mic_section(char * fnm,int start_col,int end_col,float alpha,float f)
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
    if(end_col<start_col)
    {
        int tp=start_col;
        start_col=end_col;
        end_col=tp;
    }
    if(end_col>atrn)
    {
        printf("Desired column out of index.\n");
        exit(1);
    }
    int i=0;
    int j=0;
    for(i=start_col;i<=end_col;i++)
    {
        for(j=0;j<i;j++)
        {
            float a=mic_ct_mem(file_content,D,i,j,alpha,f);
            printf("%f\t",a);
        }
        printf("\n");
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
    return NULL;
}
