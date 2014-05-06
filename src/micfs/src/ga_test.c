#include"ga.h"
#include"mic_matrix.h"

void check_pop(population pop)
{
    printf("Pop has %d mebs.\n",pop.n);
    int i=0;
    do
    {
        int j=0;
        do
        {
            printf("%d",pop.m[i].chro[j]);
        }while(++j<pop.m[i].l);
        printf("\tsig:%f\n",pop.m[i].sig);
    }while(++i<pop.n);
}
