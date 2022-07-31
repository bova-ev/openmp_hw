#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1048

int main (int argc, char *argv[]) 
{
    int nthreads, tid, i, j;
    double **a;

    /**SOLUTION:
     * Matrix a is too big to be allocated inside every thread using private.
     * We need to allocate this matrix inside parallel section.
     */
    #pragma omp parallel shared(nthreads) private(i, j, tid, a)
    {
        a = (double**)calloc(N, sizeof(double*));

        int k = 0;
        for(k = 0; k < N; k++)
            a[k] = (double*)calloc(N, sizeof(double));

        tid = omp_get_thread_num();
        if (tid == 0) 
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }
        printf("Thread %d starting...\n", tid);

        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++)
                a[i][j] = tid + i + j;

        printf("Thread %d done. Last element= %f\n", tid, a[N-1][N-1]);

        for(k = 0; k < N; k++)
            free(a[k]);
        free(a);
    } 
}
