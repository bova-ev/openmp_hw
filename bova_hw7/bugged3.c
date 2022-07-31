#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 50

int main (int argc, char **argv) 
{
    int i, nthreads, tid, section;
    float a[N], b[N], c[N];
    void print_results(float array[N], int tid, int section);

    for (i = 0; i < N; i++)
        a[i] = b[i] = i * 1.0;

    #pragma omp parallel private(c, i, tid, section)
    {
        tid = omp_get_thread_num();
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

        #pragma omp barrier
        printf("Thread %d starting...\n", tid);
        #pragma omp barrier

        #pragma omp sections nowait
        {
            #pragma omp section
            {
                printf("section 1 id = %d\n", tid);
                section = 1;
                for (i = 0; i < N; i++)
                    c[i] = a[i] * b[i];
                print_results(c, tid, section);
            }

            #pragma omp section
            {
                printf("section 2 id = %d\n", tid);
                section = 2;
                for (i = 0; i < N; i++)
                    c[i] = a[i] + b[i];
                print_results(c, tid, section);
            }
        }

        #pragma omp barrier
        printf("Thread %d exiting...\n",tid);
    }
}

void print_results(float array[N], int tid, int section) 
{
    int i, j;

    j = 1;
    #pragma omp critical
    {
        printf("\nThread %d did section %d. The results are:\n", tid, section);
        for (i = 0; i < N; i++)
        {
            printf("%e  ", array[i]);
            j++;
            if (j == 6)
            {
                printf("\n");
                j = 1;
            }
        }
        printf("\n");
    }

    /**SOLUTION:
     * This function is used inside of section which is not executable by every thread.
     * Threads which does not execute sections does not meet this barrier, thus, leaving
     * section executors in infinite loop.
     */
    //#pragma omp barrier
    printf("Thread %d done and synchronized.\n", tid); 
}
