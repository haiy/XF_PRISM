#include"read_content.h"

float ** read_content(fct *file)
{
	FILE * src =fopen(file->fn,"r");
	char *l=(char *)malloc(sizeof(char)*bufsize);

	//计算行数
	int line_numb=0;
	while(fgets(l,bufsize,src))
	{
		line_numb++;
	}
	file->inst=line_numb;

	//printf("Total %d lines.\n",line_numb);
	//计算属性个数
	int atrn = 0;
	fseek(src,0,SEEK_SET);
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
			atrn++;
			strtod(tp,&tp);
			tp++;
		}
	}
	file->atrn=atrn;
	//printf("Total %d features.\n",atrn);

	//文件内容存储方式，数组的每一行为一个属性
	//每一列是一个实例
	float **content=(float **)malloc(sizeof(float *)*atrn);
	int i=0;
	for(i=0;i<atrn;i++)
	{
		content[i]=(float*)malloc(sizeof(float)*(line_numb-1));
	}
	fseek(src,0,SEEK_SET);
	fgets(l,bufsize,src);
	char *tp=NULL;
	i=0;
	while(fgets(l,bufsize,src))
	{     
		int j=0;
		tp=l;
		while(*tp!='\0'&&*tp!='\n')
		{
			content[j][i]=strtod(tp,&tp);
			//    printf("content[%d][%d]:%f\t",j,i,content[j][i]);
			tp++;
			j++;
		}
		//     puts("");
		i++;
	}
	free(l);
	l=NULL;
	fclose(src);
	file->ct=content;
	return content;
}

float mic_ct_mem(fct file_content,Points *D,int a ,int b,float alpha,int f)
{
	int i=0;
	for(i=0;i<file_content.inst-1;i++)
	{
		D->x[i].val=file_content.ct[a][i];
		D->y[i].val=file_content.ct[b][i];
		D->x[i].pos=i;
		D->y[i].pos=i;
	}
	float result=cal_mic(D,alpha,f);
    if(EXACT(result)>EXACT(1.0000))
    {
		result=0.00;
    }
	return result;
}

void cal_mic_ctmem(int argc,char *argv[])
{
	fct file_content;
	file_content.fn=argv[1];
	read_content(&file_content);
	Points * D=(Points *)malloc(sizeof(Points));
	D->len=count_inst(argv[1]);
	//printf("%d instances\n",D->len);
	D->x=(data_type *)malloc(sizeof(data_type)*D->len);
	D->y=(data_type *)malloc(sizeof(data_type)*D->len);
	D->grid=(label_type *)malloc(sizeof(label_type)*D->len);
	float result=mic_ct_mem(file_content,D,atoi(argv[2]),atoi(argv[3]),atof(argv[4]),atof(argv[5]));
	printf("result:%f\n",result);
	int i=0;
	for(i=0;i<file_content.atrn;i++)
	{
		free(file_content.ct[i]);
	}
	free(file_content.ct);
	free(D->x);
	D->x=NULL;
	free(D->y);
	D->y=NULL;
	free(D->grid);
	D->grid=NULL;
	free(D);
	D=NULL;
}
