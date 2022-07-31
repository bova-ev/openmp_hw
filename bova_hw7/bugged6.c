#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

float a[VECLEN], b[VECLEN];

float dotprod()
{
    int i, tid;
    float sum;
    //We move declaration of parallel section into dotprod() function with
    //private "tid" and shared everything else.
    #pragma omp parallel private(tid) shared(a, b, i, sum)
    {
        tid = omp_get_thread_num();
        #pragma omp for reduction(+:sum)
        for (i = 0; i < VECLEN; i++)
        {
            sum = sum + (a[i] * b[i]);
            printf("  tid= %d i=%d\n", tid, i);
        }
    }

    //We add return to this function because it must return float value (which
    // should be "sum").
    return sum;
}


int main (int argc, char *argv[])
{
    int i;
    float sum;

    for (i = 0; i < VECLEN; i++)
        a[i] = b[i] = 1.0 * i;
    sum = 0.0;

    //Moving declaration into function and returning dotprod() into "sum" variable.

    //#pragma omp parallel shared(sum)
    sum = dotprod();

    printf("Sum = %f\n",sum);
}
