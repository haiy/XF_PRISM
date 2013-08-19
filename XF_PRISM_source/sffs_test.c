#include "sffs.h"
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



