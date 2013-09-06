#include"para_chk.h"

/* Example usage.
int main(int argc,char * argv[])
{
    if(!para_chk(argc,argv,10))
    {
        exit(0);
    }
    return 1;
}
*/

/* interface of parameter check */
int para_chk(int argc,char *argv[],int atrn)
{
    if(argc<5||argc>7)
    {
        printf("Parameter error!\n");
        usage();
        return 0;
    }
    if(ck_m(argv[1])==0)
    {
        if(ck_mic(argc,argv,atrn))
        {
            return 1;
        }

    }
    if(ck_m(argv[1])==1)
    {

        if(ck_micfs(argc,argv))
        {
            return 2;
        }
    }
    if(ck_m(argv[1])==2)
    {
        usage();
        return 0;
    }
    return 0;
}

/* mode check */
int ck_m(char *mode)
{
    if(strcmp(mode,"-MIC")==0)
    {
        return 0;
    }
    else if(strcmp(mode,"-MICFS")==0)
    {
        return 1;
    }
    else
    {
        puts("Program Mode Error !");
        return 2;
    }
}
int ck_mic(int argc,char *argv[], int atrn)
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
int ck_micfs(int argc,char *argv[])
{
    if(ck_n(argc,5)&&ck_f(argv[2])&&ck_e(atof(argv[3])) )
    {
        return 1;
    }
    else 
    {
        return 0;
    }
}

/* check the number of parameters */
int ck_n(int argc,int der)
{
    if(argc!=der)
    {
            printf("Number of parameters error! Expected %d !",der);
            return 0;
    }
    else 
    {
        return 1;
    }
}

/*check the filename*/
int ck_f(char *filename)
{
    FILE *tp=fopen(filename,"r");
    if(!tp)
    {
            printf("File %s open error!\n",filename);
            fclose(tp);
            tp=NULL;
            return 0;
    }
    else 
    {
        return 1;
    }
}

/*check the index of attributes*/
int ck_i(int xa,int yb,int atrn)
{
    if( ( (xa > atrn) || (yb > atrn) )
                    ||  ( xa < 0) || (yb < 0) ) 
    {
            printf("Input attributes indexes %d %d not between [0,%d].\n",xa,yb,atrn);
            return 0;
    }
    else 
    {
        return 1;
    }
}

/* check the exponential index */
int ck_e(float e)
{
    if( (e >1) || (e < 0) )
    {
            printf("exponential index %f not between [0,1]\n",e);
            return 0;
    }
    else 
    {
        return 1;
    }
}

/* parameters explanation */
void usage()
{
    puts("Usage: ./micfs mode file_name mode_specified_parameters");
    puts("Example 1:  ./micsf -MIC path/to/file 2 3 0.6 15");
    puts("\t-MIC means the program will in the mic only mode");
    puts("\t2 and 3 are the indexes of attributes to be analyzed.");
    puts("\t0.6 is the expotional index of B.");
    puts("\t15 is the  clumps factor.");
    puts("Example 2:  ./micsf -MICFS path/to/file  0.6 15");
    puts("\t-MICFS means the program will in the subset features selection mode");
    puts("\t0.6 is the expotional index of B.");
    puts("\t15 is the  clumps factor.");
    puts("\tBefore run the program,all the parameters must be specified right and clearly according to your own situation.");
}
